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
#ifndef RUBYK_INCLUDE_ZMQ_RECEIVE_H_
#define RUBYK_INCLUDE_ZMQ_RECEIVE_H_

#include "../vendor/include/zmq.h"

#include "rubyk.h"

#include <string>

using namespace rubyk;
typedef int LuaStackSize;

namespace zmq {

/** Listen for incoming messages on a given port.
 *
 * @dub string_format:'%%s'
 *      string_args:'(*userdata)->location()'
 */
class ReceiveSocket : public LuaCallback
{
  Thread thread_;
  void *context_;
  void *socket_;
  std::string location_;
public:

  ReceiveSocket(rubyk::Worker *worker, int type, int lua_func_idx)
    : LuaCallback(worker, lua_func_idx) {
    // FIXME: make sure we do not need more the 1 io_threads.
    context_ = zmq_init(1);
    socket_  = zmq_socket(context_, type);

    thread_.start_thread<ReceiveSocket, &ReceiveSocket::receive>(this, NULL);
  }

  ~ReceiveSocket() {
    thread_.kill();
    zmq_close(socket_);
    zmq_term(context_);
  }

  void connect(const char *location) {
    // TODO: make sure we can zmq_connect while in recv
    zmq_connect(socket_, location);
    location_ = location; // store last connection for info string
  }

  void setsockopt(int type, const char *filter = NULL) {
    if (filter) {
      zmq_setsockopt(socket_, type, filter, strlen(filter));
    } else {
      zmq_setsockopt(socket_, type, NULL, 0);
    }
  }

  const char *location() {
    return location_.c_str();
  }

private:
  void receive(Thread *runner) {
    runner->thread_ready();
    while(runner->should_run() && receive_message())
      ;
  }

  bool receive_message() {
    zmq_msg_t message;
    zmq_msg_init(&message);
    int status = zmq_recv(socket_, &message, 0);
    if (status) return false;

    // trigger callback
    { rubyk::ScopedLock lock(worker_);
      push_lua_callback();

      // push string on Lua stack
      lua_pushlstring(worker_->lua_, (const char *)zmq_msg_data(&message), zmq_msg_size(&message));

      int status = lua_pcall(worker_->lua_, 1, 1, 0);
      if (status) {
        printf("Error in receive callback: %s\n", lua_tostring(worker_->lua_, -1));
      }

      // clear stack
      lua_settop(worker_->lua_, 0);
    }
    return true;
  }
};
} // zmq

#endif // RUBYK_INCLUDE_ZMQ_RECEIVE_H_