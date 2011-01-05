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
#include "rubyk/msgpack.h"

#include "rubyk.h"

#include <stdlib.h> // rand()
#include <time.h>   // time()
#include <string>

using namespace rubyk;

namespace zmq {

/** Listen for incoming messages on a given port.
 *
 * @dub lib_name:'Socket_core'
        string_format:'%%s (%%s)'
 *      string_args:'(*userdata)->location(), (*userdata)->type()'
 */
class Socket : LuaCallback
{
  void *socket_;
  std::string location_;
  Thread *thread_;
  int type_;
public:
  Socket(rubyk::Worker *worker, int type, bool create_sock = true)
   : LuaCallback(worker),
     thread_(NULL),
     type_(type) {

    if (!worker->zmq_context_) {
      // initialize zmq context
      worker->zmq_context_ = zmq_init(10);
    }
    ++worker->zmq_context_refcount_;

    if (create_sock) {
      socket_ = zmq_socket(worker->zmq_context_, type_);
    } else {
      socket_ = NULL;
    }
  }

  ~Socket() {
    if (thread_) {
      thread_->kill();
      delete thread_;
    }

    zmq_close(socket_);
    if (!--worker_->zmq_context_refcount_) {
      zmq_term(worker_->zmq_context_);
      worker_->zmq_context_ = NULL;
    }
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

    int arg_size = msgpack_bin_to_lua(L, zmq_msg_data(&msg), zmq_msg_size(&msg));
    zmq_msg_close(&msg);
    return arg_size;
  }

  /** Send a message packed with msgpack.
   * Varying parameters.
   */
  void send(lua_State *L) {
    msgpack_sbuffer *buffer;

    msgpack_lua_to_bin(L, &buffer, 1);

    zmq_msg_t msg;
    zmq_msg_init_data(&msg, buffer->data, buffer->size, free_msgpack_msg, buffer);

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

  /** Return a string representing the socket type.
   */
  const char *type() const {
    switch(type_) {
      case ZMQ_PAIR: return "PAIR";
      case ZMQ_PUB : return "PUB" ;
      case ZMQ_SUB : return "SUB" ;
      case ZMQ_REQ : return "REQ" ;
      case ZMQ_REP : return "REP" ;
      case ZMQ_XREQ: return "XREQ";
      case ZMQ_XREP: return "XREP";
      case ZMQ_PULL: return "PULL";
      case ZMQ_PUSH: return "PUSH";
      default: return "???";
    }
  }
  /* =========================== Threading ====================
   * We add the threading code inside the Socket to ease memory
   * management and make sure Thread is killed before Socket is
   * garbage collected.
   */

   /** @internal: DO NOT USE.
    */
   void set_callback(lua_State *L) {
     if (socket_) throw Exception("Socket already created: cannot set callback.");

     set_lua_callback(L);
     thread_   = new Thread();
     thread_->start_thread<Socket, &Socket::run>(this, NULL);
   }

   void quit() {
     if (thread_) thread_->quit();
   }

   void kill() {
     if (thread_) thread_->kill();
   }

   void join() {
     rubyk::ScopedUnlock unlock(worker_);
     if (thread_) thread_->join();
   }

   bool should_run() {
     if (thread_) return thread_->should_run();
     return false;
   }
 private:
   void run(Thread *runner) {
     // Create socket in new thread
     socket_ = zmq_socket(worker_->zmq_context_, type_);


     runner->thread_ready();

     rubyk::ScopedLock lock(worker_);

     push_lua_callback();

     // lua_ = LuaCallback's thread state
     // first argument is self
     int status = lua_pcall(lua_, 1, 0, 0);

     if (status) {
       printf("Error starting Socket function: %s\n", lua_tostring(lua_, -1));
     }
   }
};
} // zmq

#endif // RUBYK_INCLUDE_ZMQ_SOCKET_H_