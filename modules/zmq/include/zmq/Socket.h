/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
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
#ifndef LUBYK_INCLUDE_ZMQ_SOCKET_H_
#define LUBYK_INCLUDE_ZMQ_SOCKET_H_

#include "../vendor/include/zmq.h"
#include "lubyk/msgpack.h"

#include "lubyk.h"

#include <stdlib.h> // rand()
#include <time.h>   // time()
#include <string>

using namespace lubyk;

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
  /** Mutex used to enable zmq.REQ sharing between threads.
   */
  Mutex req_mutex_;
public:
  Socket(lubyk::Worker *worker, int type)
   : LuaCallback(worker),
     thread_(NULL),
     type_(type) {

    if (!worker->zmq_context_) {
      // initialize zmq context
      worker->zmq_context_ = zmq_init(4); // TODO: why 4 ?
    }
    ++worker->zmq_context_refcount_;

    socket_ = zmq_socket(worker->zmq_context_, type_);

    if (!socket_) {
      switch (errno) {
      case EINVAL:
        throw Exception("Invalid socket type: %i).", type_);
      case EMTHREAD:
        throw Exception("The maximum number of sockets within this context has been exceeded.");
      case EFAULT: // continue
      default:
        throw Exception("The provided context was not valid.");
      }
    }
  }

  ~Socket() {
    if (thread_) {
      kill();
      delete thread_;
    }

    zmq_close(socket_);
    if (!--worker_->zmq_context_refcount_) {
      zmq_term(worker_->zmq_context_);
      worker_->zmq_context_ = NULL;
    }
  }

  /** Set socket options.
   * Should NOT be used while in a request() or recv().
   */
  void setsockopt(int type, lua_State *L) {
    const void *ptr;
    size_t ptr_len;

    // different types for the option value
    int i;
    const char *str;

    if (lua_isnumber(L, 3)) {
      i = lua_tonumber(L, 3);
      ptr = &i;
      ptr_len = sizeof(i);
    } else if (lua_isstring(L, 3)) {
      str = lua_tostring(L, 3);
      ptr = str;
      ptr_len = strlen(str);
    } else {
      ptr = NULL;
      ptr_len = 0;
    }

    if (ptr) {
      if (zmq_setsockopt(socket_, type, ptr, ptr_len)) {
        if (lua_isstring(L, -1))
          throw Exception("Could not set socket option %i (value: '%s').", type, str);
        else
          throw Exception("Could not set socket option %i (value: '%i').", type, i);
      }
    } else {
      if (zmq_setsockopt(socket_, type, NULL, 0))
        throw Exception("Could not set socket option %i (no value).", type);
    }
  }

  /** Bind a service to a location like "tcp://\*:5500"
   * Should NOT be used while in a request() or recv().
   */
  void bind(const char *location) {
    if (zmq_bind(socket_, location)) {
      throw_bind_error(errno, location);
    }
    location_ = location; // store last connection for info string
  }

  /** Bind to a random port.
   * Should NOT be used while in a request() or recv().
   */
  int bind(int min_port = 2000, int max_port = 20000, int retries = 100) {
    // do not use rand() --> not random in higher bits
    srandom((unsigned)time(0));
    static const int buf_size = 50;
    char buffer[buf_size];
    for(int i = 0; i < retries; ++i) {
      int port = min_port + (max_port - min_port) * ((float)random()/RAND_MAX);
      snprintf(buffer, buf_size, "tcp://*:%i", port);
      if (!zmq_bind(socket_, buffer)) {
        // success
        location_ = buffer;
        return port;
      } else if (errno != EADDRINUSE) {
        throw_bind_error(errno, "*");
      }
    }
    throw Exception("Could not bind to any port in range '%i-%i' (%i retries).", min_port, max_port, retries);
  }

  /** Connect to a server.
   * It is safe to use this while in a request() but NOT while in a
   * send() or recv().
   */
  void connect(const char *location) {
    ScopedLock lock(req_mutex_);
    if (zmq_connect(socket_, location))
      throw Exception("Could not connect to '%s'.", location);
    location_ = location; // store last connection for info string
  }

  /** Receive a message (blocks).
   * Should NOT be used while already in a request() or recv().
   * For a server, this should typically be used inside the loop.
   * We pass the lua_State to avoid mixing thread contexts.
   */
  LuaStackSize recv(lua_State *L) {
    zmq_msg_t msg;
    zmq_msg_init(&msg);


    { // unlock worker
      ScopedUnlock unlock(worker_);

      if (zmq_recv(socket_, &msg, 0)) {
        zmq_msg_close(&msg);
        throw_recv_error(errno);
      }
      printf("[%p] zmq_recv\n", socket_);
    }

    int arg_size = msgpack_bin_to_lua(L, zmq_msg_data(&msg), zmq_msg_size(&msg));
    zmq_msg_close(&msg);
    return arg_size;
  }

  /** Send a message packed with msgpack.
   * Should NOT be used while in a request() or recv().
   * Varying parameters.
   */
  void send(lua_State *L) {
    msgpack_sbuffer *buffer;

    msgpack_lua_to_bin(L, &buffer, 1);

    zmq_msg_t msg;
    zmq_msg_init_data(&msg, buffer->data, buffer->size, free_msgpack_msg, buffer);

    if (zmq_send(socket_, &msg, 0)) {
      zmq_msg_close(&msg);
      throw_send_error(errno);
    }  
    printf("[%p] zmq_send\n", socket_);
    zmq_msg_close(&msg);
  }

  /** Request = remote call. Can be used by multiple threads.
   */
  LuaStackSize request(lua_State *L) {

    msgpack_sbuffer *buffer;

    msgpack_lua_to_bin(L, &buffer, 1);

    zmq_msg_t msg;
    zmq_msg_t recv_msg;
    zmq_msg_init_data(&msg, buffer->data, buffer->size, free_msgpack_msg, buffer);

    { // unlock worker
      ScopedUnlock unlock(worker_);
      // lock socket
      ScopedLock lock(req_mutex_);

      if (zmq_send(socket_, &msg, 0)) {
        zmq_msg_close(&msg);
        throw_send_error(errno);
      }
      printf("[%p] zmq_send\n", socket_);

      zmq_msg_close(&msg);

      zmq_msg_init(&recv_msg);

      if (zmq_recv(socket_, &recv_msg, 0)) {
        zmq_msg_close(&recv_msg);
        throw_recv_error(errno);
      }
      printf("[%p] zmq_recv\n", socket_);
    }

    int arg_size = msgpack_bin_to_lua(L, zmq_msg_data(&recv_msg), zmq_msg_size(&recv_msg));
    zmq_msg_close(&recv_msg);
    return arg_size;
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
      case ZMQ_PAIR: return "zmq.PAIR";
      case ZMQ_PUB : return "zmq.PUB" ;
      case ZMQ_SUB : return "zmq.SUB" ;
      case ZMQ_REQ : return "zmq.REQ" ;
      case ZMQ_REP : return "zmq.REP" ;
      case ZMQ_XREQ: return "zmq.XREQ";
      case ZMQ_XREP: return "zmq.XREP";
      case ZMQ_PULL: return "zmq.PULL";
      case ZMQ_PUSH: return "zmq.PUSH";
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
   set_lua_callback(L);
   thread_   = new Thread();
   thread_->start_thread<Socket, &Socket::run>(this, NULL);
  }

  void quit() {
   if (thread_) thread_->quit();
  }

  void kill() {
   req_mutex_.unlock(); // unlock if not locked should be ok
   if (thread_) thread_->kill();
  }

  void join() {
   ScopedUnlock unlock(worker_);
   if (thread_) thread_->join();
  }

  bool should_run() {
   if (thread_) return thread_->should_run();
   return false;
  }

private:

  void throw_bind_error(int err, const char *location) {
    switch(err) {
    case EPROTONOSUPPORT:
      throw Exception("The requested transport protocol is not supported (%s).", location);
    case ENOCOMPATPROTO:
      throw Exception("The requested transport protocol (%s) is not compatible with the socket type (%s).", location, type());
    case EADDRINUSE:
      throw Exception("The requested address is already in use (%s).", location);
    case EADDRNOTAVAIL:
      throw Exception("The requested address was not local (%s).", location);
    case ENODEV:
      throw Exception("The requested address specifies a nonexistent interface (%s). Did you mean connect ?.", location);
    case ETERM:
      throw Exception("The ZMQ context associated with the specified socket was terminated (%s).", location);
    case EFAULT: // continue
    default:
      throw Exception("The provided socket was not valid (%s).", location);
    }
  }

  void throw_recv_error(int err) {
    switch(err) {
    case EAGAIN:
      throw Exception("Non-blocking mode was requested and no messages are available at the moment.");
    case ENOTSUP:
      throw Exception("The zmq_recv() operation is not supported by this socket type (%s).", type());
    case EFSM:
      throw Exception("The zmq_recv() operation cannot be performed on this socket at the moment due to the socket not being in the appropriate state. This error may occur with socket types that switch between several states, such as ZMQ_REP (%s).", type());
    case ETERM:
      throw Exception("The ZMQ context associated with the specified socket was terminated.");
    case EFAULT: // continue
    default:
      throw Exception("The provided context was not valid (NULL).");
    }
  }

  void throw_send_error(int err) {
    switch(err) {
    case EAGAIN:
      throw Exception("Non-blocking mode was requested and no messages are available at the moment.");
    case ENOTSUP:
      throw Exception("The zmq_send() operation is not supported by this socket type (%s).", type());
    case EFSM:
      throw Exception("The zmq_send() operation cannot be performed on this socket at the moment due to the socket not being in the appropriate state. This error may occur with socket types that switch between several states, such as ZMQ_REP (%s).", type());
    case ETERM:
      throw Exception("The ZMQ context associated with the specified socket was terminated.");
    case EFAULT: // continue
    default:
      throw Exception("The provided context was not valid (NULL).");
    }
  }

  void run(Thread *runner) {

   runner->thread_ready();

   ScopedLock lock(worker_);

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

#endif // LUBYK_INCLUDE_ZMQ_SOCKET_H_