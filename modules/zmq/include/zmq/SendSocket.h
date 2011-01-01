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
#ifndef RUBYK_INCLUDE_ZMQ_SEND_SOCKET_H_
#define RUBYK_INCLUDE_ZMQ_SEND_SOCKET_H_

#include "../vendor/include/zmq.h"
#include "zmq/msgpack_zmq.h"

#include "rubyk.h"

#include <string>
#include <string.h>

using namespace rubyk;
typedef int LuaStackSize;

extern void msgpack_lua_to_zmq(lua_State *L, zmq_msg_t *msg, int start_index);

namespace zmq {

/** Send messages for incoming messages on a given port.
 *
 * @dub string_format:'%%s'
 *      string_args:'(*userdata)->location()'
 */
class SendSocket
{
  void *context_;
  void *socket_;
  std::string location_;
public:
  SendSocket(int type) {
    // FIXME: make sure we do not need more the 1 io_threads.
    context_ = zmq_init(1);
    socket_  = zmq_socket(context_, type);
  }

  ~SendSocket() {
    zmq_close(socket_);
    zmq_term(context_);
  }

  void bind(const char *location) {
    zmq_bind(socket_, location);
    location_ = location; // save last location for info string
  }

  void send(lua_State *L) {
    int rc;
    zmq_msg_t msg;
    msgpack_lua_to_zmq(L, &msg, 2);

    rc = zmq_send(socket_, &msg, 0);
    if (rc) {
      lua_pushstring(L, "Error sending message");
      lua_error(L);
      // never reached: FIXME: memory leak... (msg)
    }
    zmq_msg_close(&msg);
  }

  const char *location() {
    return location_.c_str();
  }
};
}

#endif // RUBYK_INCLUDE_ZMQ_SEND_SOCKET_H_