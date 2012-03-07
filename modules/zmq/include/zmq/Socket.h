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

#include "dub/dub.h"
#include "lubyk/msgpack.h"
#include "lubyk/time_ref.h"
#include "lubyk/worker.h"

#include "zmq.h"

#include <stdint.h>
#include <stdlib.h> // rand()
#include <time.h>   // time()
#include <string>

using namespace lubyk;

typedef struct ZMQ_Socket ZMQ_Socket;
namespace zmq {

class Poller;
/** ZeroMQ Socket.
 *
 * @dub push: pushobject
 *      register: Socket_core
 *      string_format: %%s (%%s)
 *      string_args: self->location(), self->type()
 */
class Socket : public dub::Thread
{
  friend class Poller;
  void *socket_;
  std::string location_;
  int type_;
  int send_flags_;

  /** Timer used for timeout in request.
   */
  TimeRef time_ref_;

  /** FIXME: Replace lubyk::Worker with a (lua ensured) zmq::Context singleton.
   */
  lubyk::Worker *worker_;
public:
  /** The worker is not provided by LuaObject before luaInit is called.
   * This call happens after full object construction. We need the worker
   * to set the zmq context, so we pass it as argument.
   */
  Socket(int type, lubyk::Worker *worker)
      : type_(type)
      , send_flags_(0) 
      , worker_(worker)
  {

    if (!worker->zmq_context_) {
      // initialize zmq context
      worker->zmq_context_ = zmq_init(1);
    }
    ++worker->zmq_context_refcount_;

    socket_ = zmq_socket(worker->zmq_context_, type_);

    if (!socket_) {
      if (!--worker->zmq_context_refcount_) {
        zmq_term(worker->zmq_context_);
        worker->zmq_context_ = NULL;
      }
      switch (errno) {
      case EINVAL:
        throw dub::Exception("Invalid socket type: %i).", type_);
      case EMTHREAD:
        throw dub::Exception("The maximum number of sockets within this context has been exceeded.");
      case EFAULT: // continue
      default:
        throw dub::Exception("The provided context was not valid.");
      }
    }
  }

  ~Socket() {
    zmq_close(socket_);
    if (!--worker_->zmq_context_refcount_) {
      zmq_term(worker_->zmq_context_);
      worker_->zmq_context_ = NULL;
    }
  }

  int fd() {
    int fd;
    size_t fd_size = sizeof(fd);
    if (zmq_getsockopt(socket_, ZMQ_FD, &fd, &fd_size)) {
      throw dub::Exception("Error while getting file descriptor (%s).", zmq_strerror(zmq_errno()));
    }
    return fd;
  }

  void setNonBlocking(bool non_blocking) {
    if (non_blocking) {
      send_flags_ |= ZMQ_NOBLOCK;
    } else {
      send_flags_ &= ~ZMQ_NOBLOCK;
    }
  }

  /** Set socket options.
   */
  void setsockopt(int type, lua_State *L) {
    uint64_t ui;
    int64_t  li;
    int      i;
    size_t   sz;
    const char *str;

    // <self> <type> <value>
    int status;
    switch(type) {
      case ZMQ_HWM:    // continue
      case ZMQ_SNDBUF: // continue
      case ZMQ_RCVBUF:
        ui = luaL_checknumber(L, 3);
        status = zmq_setsockopt(socket_, type, &ui, sizeof(ui));
        break;
      case ZMQ_SWAP:  // continue
      case ZMQ_RATE:  // continue
      case ZMQ_RECOVERY_IVL: // continue
      case ZMQ_RECOVERY_IVL_MSEC:
      case ZMQ_MCAST_LOOP:
        li = luaL_checknumber(L, 3);
        status = zmq_setsockopt(socket_, type, &li, sizeof(li));
        break;
      case ZMQ_LINGER: // continue
      case ZMQ_RECONNECT_IVL: // continue
      case ZMQ_RECONNECT_IVL_MAX:
        i = luaL_checknumber(L, 3);
        status = zmq_setsockopt(socket_, type, &i, sizeof(i));
        break;
      case ZMQ_SUBSCRIBE: // continue
      case ZMQ_UNSUBSCRIBE:
        if (lua_isstring(L, 3)) {
          str = lua_tolstring(L, 3, &sz);
        } else {
          str = NULL;
          sz  = 0;
        }
        status = zmq_setsockopt(socket_, type, str, sz);
        break;
      default:
        throw dub::Exception("Could not set socket option: unknown option %i.", type);
    }


    if (status) {
      throw dub::Exception("Could not set socket option %i (%s).", type, str, zmq_strerror(errno));
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
    throw dub::Exception("Could not bind to any port in range '%i-%i' (%i retries).", min_port, max_port, retries);
  }

  /** Connect to a server.
   */
  void connect(const char *location) {
    if (zmq_connect(socket_, location))
      throw dub::Exception("Could not connect to '%s'.", location);
    location_ = location; // store last connection for info string
  }

  /** Return true if the socket responds to the given event. This has
   * to be used when we do not use zmq_poll but some external poll/select
   * on the file descriptor.
   */
  bool hasEvent(int event) {
    uint32_t events;
    size_t ev_size = sizeof(events);
    if (zmq_getsockopt(socket_, ZMQ_EVENTS, &events, &ev_size)) {
      throw dub::Exception("Error while getting events (%s).", zmq_strerror(zmq_errno()));
    }
    return events & event;
  }

  /** Receive a message (blocks).
   * Should NOT be used while already in a request() or recv().
   * For a server, this should typically be used inside the loop.
   * We pass the lua_State to avoid mixing thread contexts.
   */
  LuaStackSize recv(lua_State *L) {
    zmq_msg_t recv_msg;
    zmq_msg_init(&recv_msg);

    if (zmq_recv(socket_, &recv_msg, ZMQ_NOBLOCK)) {
      zmq_msg_close(&recv_msg);
      // We always waitRead before we arrive here, so we should never get
      // EAGAIN.
      throw_recv_error(errno);
    }

    int arg_size;
    try {
      arg_size = msgpack_bin_to_lua(L, zmq_msg_data(&recv_msg), zmq_msg_size(&recv_msg));
    } catch (dub::Exception e) {
      zmq_msg_close(&recv_msg);
      throw;
    }
    zmq_msg_close(&recv_msg);
    return arg_size;
  }

  /** Send a message packed with msgpack.
   * Varying parameters.
   * Can throw a dub::Exception.
   * @return false in case the send buffer is full.
   */
  bool send(lua_State *L) {
    msgpack_sbuffer *buffer;

    try {
      msgpack_lua_to_bin(L, &buffer, 1);
    } catch (dub::Exception e) {
      printf("WHAT ? %s\n", e.what());
      throw;
    }

    zmq_msg_t msg;
    zmq_msg_init_data(&msg, buffer->data, buffer->size, free_msgpack_msg, buffer);

    if (zmq_send(socket_, &msg, send_flags_)) {
      zmq_msg_close(&msg);
      if (errno == EAGAIN) return false;
      throw_send_error(errno);
    }
    zmq_msg_close(&msg);
    return true;
  }

  /** Sends raw bytes without using msgpack.
   * @return false in case the send buffer is full.
   */
  bool rawSend(lua_State *L) {
    size_t size;
    const char *data = lua_tolstring(L, -1, &size);
    zmq_msg_t msg;
    zmq_msg_init_data(&msg, (void*)data, size, NULL, NULL);

    if (zmq_send(socket_, &msg, send_flags_)) {
      zmq_msg_close(&msg);
      if (errno == EAGAIN) return false;
      throw_send_error(errno);
    }
    zmq_msg_close(&msg);
    return true;
  }

  /** Request = remote call. Can be used by multiple threads.
   * Can throw dub::Exception.
   */
  LuaStackSize request(lua_State *L) {
    bool timed_out = false;
    msgpack_sbuffer *buffer;

    msgpack_lua_to_bin(L, &buffer, 1);

    zmq_msg_t msg;
    zmq_msg_t recv_msg;
    zmq_msg_init_data(&msg, buffer->data, buffer->size, free_msgpack_msg, buffer);

    //  Initialize poll set
    zmq_pollitem_t items[] = {
        { socket_,    0, ZMQ_POLLIN, 0 }
    };

    if (zmq_send(socket_, &msg, send_flags_)) {
      zmq_msg_close(&msg);
      // TODO: EAGAIN
      throw_send_error(errno);
    }

    zmq_msg_close(&msg);

    zmq_msg_init(&recv_msg);


    double start = time_ref_.elapsed();
    static const long total_timeout = 200000;
    long timeout = total_timeout;
    // Receive with (relatively short) timeout
    // [us] = 200ms
    while (1) {
      if (zmq_poll(items, 1, timeout) == -1) {
        zmq_msg_close(&recv_msg);
        throw_poll_error(errno);
      }

      if (!items[0].revents & ZMQ_POLLIN) {
        // timeout: no message
        double now = time_ref_.elapsed();
        if (now >= start + total_timeout/1000) {
          timed_out = true;
          break;
        } else {
          // poll can return before total_timeout
          // wait the rest of the timeout
          timeout = total_timeout - (now - start);
        }
      } else {
        if (zmq_recv(socket_, &recv_msg, 0)) {
          zmq_msg_close(&recv_msg);
          throw_recv_error(errno);
        }
        break;
      }
    }

    if (timed_out) {
      lua_pushnil(L);
      return 1;
    } else {
      int arg_size;
      try {
        arg_size = msgpack_bin_to_lua(L, zmq_msg_data(&recv_msg), zmq_msg_size(&recv_msg));
      } catch (dub::Exception e) {
        zmq_msg_close(&recv_msg);
        throw;
      }
      zmq_msg_close(&recv_msg);
      return arg_size;
    }
  }

  const char *location() {
    return location_.c_str();
  }

  int port() {
    size_t pos = location_.rfind(":");
    if (pos != std::string::npos) {
      return atoi(location_.substr(pos + 1).c_str());
    } else {
      throw dub::Exception("Could not get port from localtion '%s'.", location_.c_str());
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

private:

  void throw_bind_error(int err, const char *location) {
    switch(err) {
    case EPROTONOSUPPORT:
      throw dub::Exception("The requested transport protocol is not supported (%s).", location);
    case ENOCOMPATPROTO:
      throw dub::Exception("The requested transport protocol (%s) is not compatible with the socket type (%s).", location, type());
    case EADDRINUSE:
      throw dub::Exception("The requested address is already in use (%s).", location);
    case EADDRNOTAVAIL:
      throw dub::Exception("The requested address was not local (%s).", location);
    case ENODEV:
      throw dub::Exception("The requested address specifies a nonexistent interface (%s). Did you mean connect ?.", location);
    case ETERM:
      throw dub::Exception("The ZMQ context associated with the specified socket was terminated (%s).", location);
    case EFAULT: // continue
    default:
      throw dub::Exception("The provided socket was not valid (%s).", location);
    }
  }

  void throw_recv_error(int err) {
    switch(err) {
    case EAGAIN:
      throw dub::Exception("Non-blocking mode was requested and no messages are available at the moment.");
    case ENOTSUP:
      throw dub::Exception("The zmq_recv() operation is not supported by this socket type (%s).", type());
    case EFSM:
      throw dub::Exception("The zmq_recv() operation cannot be performed on this socket at the moment due to the socket not being in the appropriate state. This error may occur with socket types that switch between several states, such as ZMQ_REP (%s).", type());
    case ETERM:
      throw dub::Exception("The ZMQ context associated with the specified socket was terminated.");
    case EFAULT: // continue
    default:
      throw dub::Exception("The provided context was not valid (NULL).");
    }
  }

  void throw_poll_error(int err) {
    switch(err) {
    case ETERM:
      throw dub::Exception("At least one of the members of the items array refers to a socket whose associated ØMQ context was terminated.");
    case EFAULT:
      throw dub::Exception("At least one of the members of the items array refers to a socket belonging to a different application thread.");
    default:
      throw dub::Exception("The provided context was not valid (NULL).");
    }
  }

  void throw_send_error(int err) {
    switch(err) {
    case EAGAIN:
      throw dub::Exception("Non-blocking mode was requested and the message cannot be sent at the moment.");
    case ENOTSUP:
      throw dub::Exception("The zmq_send() operation is not supported by this socket type (%s).", type());
    case EFSM:
      throw dub::Exception("The zmq_send() operation cannot be performed on this socket at the moment due to the socket not being in the appropriate state. This error may occur with socket types that switch between several states, such as ZMQ_REP (%s).", type());
    case ETERM:
      throw dub::Exception("The ZMQ context associated with the specified socket was terminated.");
    case EFAULT: // continue
    default:
      throw dub::Exception("The provided context was not valid (NULL).");
    }
  }
};
} // zmq

#endif // LUBYK_INCLUDE_ZMQ_SOCKET_H_
