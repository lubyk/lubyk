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
#ifndef RUBYK_INCLUDE_RK_SOCKET_H_
#define RUBYK_INCLUDE_RK_SOCKET_H_

#include "rubyk.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>  // getaddrinfo

#include <errno.h>  // errno
#include <string.h> // strerror

#include <string>


using namespace rubyk;

typedef int LuaStackSize;

namespace rk {

/** Listen for incoming messages on a given port.
 *
 * @dub lib_name:'Socket_core'
        string_format:'%%s:%%d --> %%s:%%d'
 *      string_args:'(*userdata)->local_host(), (*userdata)->local_port(), (*userdata)->remote_host(), (*userdata)->remote_port()'
 */
class Socket : public LuaCallback
{
  Thread thread_;
  int socket_fd_;
  std::string local_host_;
  int local_port_;
  std::string remote_host_;
  int remote_port_;
public:
  Socket(rubyk::Worker *worker, int port = 0, int lua_func_idx = -1)
    : LuaCallback(worker),
      socket_fd_(-1),
      local_host_("*"),
      local_port_(-1),
      remote_host_("?"),
      remote_port_(-1) {

    if (lua_func_idx != -1) {
      loop(lua_func_idx);
    }
  }

  ~Socket() {
    kill();
    if (socket_fd_ != -1) {
      close(socket_fd_);
    }
  }

  /** Bind socket to a specific interface.
   * @return bound port
   */
  int bind(const char *localhost = NULL, int port = 0) {
    char port_str[10];
    snprintf(port_str, 10, "%i", port);

    if (localhost) {
      local_host_ = localhost;
      if (local_host_ == "" || local_host_ == "*") {
        // bind to any interface
        localhost = NULL;
        local_host_ = "*";
      }
    }

    if (socket_fd_ != -1) {
      close(socket_fd_);
    }

    // bind
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));

    // we do not care if we get an IPv4 or IPv6 address
    hints.ai_family = AF_UNSPEC;
    // TCP
    hints.ai_socktype = SOCK_STREAM;
    // fill our own IP
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port_str, &hints, &res)) {
      throw Exception("Could not getaddrinfo for %s:%i.", local_host_.c_str(), port);
    }

    // we use getaddrinfo to stay IPv4/IPv6 agnostic
    socket_fd_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (socket_fd_ == -1) {
      throw Exception("Could not create socket for %s:%i (%s).", local_host_.c_str(), port, strerror(errno));
    }

    // bind to port
    if (::bind(socket_fd_, res->ai_addr, res->ai_addrlen)) {
      throw Exception("Could not bind socket to %s:%i (%s).", local_host_.c_str(), port, strerror(errno));
    }

    if (port == 0) {
      // get bound port
      struct sockaddr sa;
      memset(&sa, 0, sizeof(struct sockaddr));
      // length has to be in a variable
      socklen_t sa_len = sizeof(sa);
      if (getsockname(socket_fd_, &sa, &sa_len)) {
        throw Exception("Could not get bound port (%s).", strerror(errno));
      }
      if (sa.sa_family == AF_INET) {
        local_port_ = ntohs(((struct sockaddr_in *)&sa)->sin_port);
      } else {
        local_port_ = ntohs(((struct sockaddr_in6 *)&sa)->sin6_port);
      }
    } else {
      local_port_ = port;
    }

    freeaddrinfo(res);

    return local_port_;
  }

  void connect(const char *host, int port) {
    // TODO
    remote_host_ = host;
    remote_port_ = port;
  }

  /** Receive a message (blocks).
   * For a server, this should typically be used inside the loop.
   * We pass the lua_State to avoid mixing thread contexts.
   */
  LuaStackSize recv(lua_State *L) {
    // TODO
    return 0;
    // unlock while waiting
    //{ rubyk::ScopedUnlock unlock(worker_);
    //  if (zmq_recv(socket_, &msg, 0)) {
    //    zmq_msg_close(&msg);
    //    throw Exception("Could not receive.");
    //  }
    //}

    //int arg_size = msgpack_zmq_to_lua(L, &msg);
    //return arg_size;
  }

  /** Send a message packed with msgpack.
   * Varying parameters.
   */
  void send(lua_State *L) {
    // TODO
    // zmq_msg_t msg;
    // msgpack_lua_to_zmq(L, &msg, 1);
    // if (zmq_send(socket_, &msg, 0)) {
    //   zmq_msg_close(&msg);
    //   throw Exception("Could not send message.");
    // }
    // zmq_msg_close(&msg);
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
    set_lua_callback(lua_func_idx);
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

  const char *local_host() const {
    return local_host_.c_str();
  }

  int local_port() const {
    return local_port_;
  }

  const char *remote_host() const {
    return remote_host_.c_str();
  }

  int remote_port() const {
    return remote_port_;
  }

  int port() const {
    return local_port_;
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
} // rk

#endif // RUBYK_INCLUDE_RK_SOCKET_H_