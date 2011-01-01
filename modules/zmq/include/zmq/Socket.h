/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/
#ifndef RUBYK_INCLUDE_ZMQ_SOCKET_H_
#define RUBYK_INCLUDE_ZMQ_SOCKET_H_

#include "../vendor/include/zmq.h"
#include "zmq/msgpack_zmq.h"

#include "rubyk.h"

#include <stdlib.h> // rand()
#include <time.h>   // time()
#include <string>

using namespace rubyk;

typedef int LuaStackSize;

namespace zmq {

/** Listen for incoming messages on a given port.
 *
 * @dub lib_name:'Socket_core'
        string_format:'%%s'
 *      string_args:'(*userdata)->location()'
 */
class Socket : public LuaCallback
{
  Thread thread_;
  void *context_;
  void *socket_;
  std::string location_;
public:
  Socket(rubyk::Worker *worker, int type)
    : LuaCallback(worker) {
    // FIXME: make sure we do not need more the 1 io_threads.
    context_ = zmq_init(1);
    socket_  = zmq_socket(context_, type);
  }

  ~Socket() {
    kill();
    zmq_close(socket_);
    zmq_term(context_);
  }

  void setsockopt(int type, const char *filter = NULL) {
    if (filter) {
      if (zmq_setsockopt(socket_, type, filter, strlen(filter)))
        throw Exception("Could not set socket option %i (filter: '%s').", type, filter);
    } else {
      if (zmq_setsockopt(socket_, type, NULL, 0))
        throw Exception("Could not set socket option %i (empty filter).", type);
    }
  }

  /** Bind a service to a location like "tcp://\*:5500"
   */
  void bind(const char *location) {
    if (zmq_bind(socket_, location))
      throw Exception("Could not bind to '%s'", location);
    location_ = location; // store last connection for info string
  }

  /** Bind to a random port.
   * @return bound port or raise on failure
   * FIXME: when Dub is fixed with overloaded member functions, use 'bind'
   */
  int bind_to_random_port(int min_port = 2000, int max_port = 20000, int retries = 100) {
    static const int buf_size = 50;
    srand((unsigned)time(0));
    char buffer[buf_size];
    for(int i = 0; i < retries; ++i) {
      int port = min_port + ((float)rand() * (max_port - min_port))/RAND_MAX;
      snprintf(buffer, buf_size, "tcp://*:%i", port);
      if (!zmq_bind(socket_, buffer)) {
        // success
        location_ = buffer;
        return port;
      }
    }
    throw Exception("Could not bind to any port in range '%i-%i' (%i retries).", min_port, max_port, retries);
  }

  /** Connect to a server.
   */
  void connect(const char *location) {
    if (zmq_connect(socket_, location))
      throw Exception("Could not connect to '%s'.", location);
    location_ = location; // store last connection for info string
  }

  /** Receive a message (blocks).
   * For a server, this should typically be used inside the loop.
   * We pass the lua_State to avoid mixing thread contexts.
   */
  LuaStackSize recv(lua_State *L) {
    zmq_msg_t msg;
    zmq_msg_init(&msg);

    // unlock while waiting
    { rubyk::ScopedUnlock unlock(worker_);
      if (zmq_recv(socket_, &msg, 0)) {
        zmq_msg_close(&msg);
        throw Exception("Could not receive.");
      }
    }

    int arg_size = msgpack_zmq_to_lua(L, &msg);
    zmq_msg_close(&msg);
    return arg_size;
  }

  /** Send a message packed with msgpack.
   * Varying parameters.
   */
  void send(lua_State *L) {
    zmq_msg_t msg;
    msgpack_lua_to_zmq(L, &msg, 1);
    if (zmq_send(socket_, &msg, 0)) {
      zmq_msg_close(&msg);
      throw Exception("Could not send message.");
    }
    zmq_msg_close(&msg);
  }

  /** Request = remote call.
   */
  LuaStackSize request(lua_State *L) {
    send(L);
    return recv(L);
  }

  /** Execute a loop in a new thread.
   */
  void loop(int lua_func_idx) {
    set_callback(lua_func_idx);
    thread_.start_thread<Socket, &Socket::run>(this, NULL);
  }

  /** Halt loop.
   */
  void quit() {
    // sets should_run to false but does not interrupt
    thread_.quit();
  }

  /** Interrupt loop.
   */
  void kill() {
    // get out of blocking recv
    thread_.send_signal(SIGINT);
  }

  const char *location() {
    return location_.c_str();
  }

  int port() {
    size_t pos = location_.rfind(":");
    if (pos != std::string::npos) {
      return atoi(location_.substr(pos + 1).c_str());
    } else {
      throw Exception("Could not get port from localtion '%s'.", location_.c_str());
    }
  }

private:
  void run(Thread *runner) {
    // L = LuaCallback's lua thread context
    runner->thread_ready();

    while(runner->should_run()) {
      // trigger callback
      { rubyk::ScopedLock lock(worker_);
        push_lua_callback();
        int status = lua_pcall(L, 0, 1, 0);
        if (status) {
          printf("Error in loop callback: %s\n", lua_tostring(L, -1));
          return;
        }

        if (lua_type(L, -1) == LUA_TBOOLEAN && !lua_toboolean(L, -1)) {
          // exit loop on return false
          return;
        }
      }
    }
  }
};
} // zmq

#endif // RUBYK_INCLUDE_ZMQ_SOCKET_H_