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
#ifndef LUBYK_INCLUDE_ZMQ_CONSTANTS_H_
#define LUBYK_INCLUDE_ZMQ_CONSTANTS_H_

#include "../vendor/include/zmq.h"

#include "lubyk.h"

#include <string>

namespace zmq {

/** List of zmq socket types for 'client' sockets.
 */
enum SocketTypes {
  PAIR = ZMQ_PAIR,
  PUB  = ZMQ_PUB,
  SUB  = ZMQ_SUB,
  REQ  = ZMQ_REQ,
  REP  = ZMQ_REP,
  XREQ = ZMQ_XREQ,
  XREP = ZMQ_XREP,
  PULL = ZMQ_PULL,
  PUSH = ZMQ_PUSH,
};

/** List of zmq socket options to be set with setsockopt.
 */
enum SocketOptions {
  HWM          = ZMQ_HWM,
  SWAP         = ZMQ_SWAP,
  AFFINITY     = ZMQ_AFFINITY,
  IDENTITY     = ZMQ_IDENTITY,
  SUBSCRIBE    = ZMQ_SUBSCRIBE,
  UNSUBSCRIBE  = ZMQ_UNSUBSCRIBE,
  RATE         = ZMQ_RATE,
  RECOVERY_IVL = ZMQ_RECOVERY_IVL,
  MCAST_LOOP   = ZMQ_MCAST_LOOP,
  SNDBUF       = ZMQ_SNDBUF,
  RCVBUF       = ZMQ_RCVBUF,
  RCVMORE      = ZMQ_RCVMORE,
  FD           = ZMQ_FD,
  EVENTS       = ZMQ_EVENTS,
  TYPE         = ZMQ_TYPE,
  LINGER       = ZMQ_LINGER,
  RECONNECT_IVL= ZMQ_RECONNECT_IVL,
  BACKLOG      = ZMQ_BACKLOG,
};

} // zmq

#endif // LUBYK_INCLUDE_ZMQ_CONSTANTS_H_