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
#ifndef RUBYK_INCLUDE_ZMQ_CONSTANTS_H_
#define RUBYK_INCLUDE_ZMQ_CONSTANTS_H_

#include "../vendor/include/zmq.h"

#include "rubyk.h"

#include <string>

namespace zmq {

/** List of zmq socket types for receiving sockets.
 */
enum ReceiveSocketTypes {
  //ZMQ_PAIR
  //ZMQ_PUB
  SUB = ZMQ_SUB,
  //ZMQ_REQ
  //ZMQ_REP
  //ZMQ_XREQ
  //ZMQ_XREP
  PULL = ZMQ_PULL,
  //ZMQ_PUSH
};

/** List of zmq socket types for sending sockets.
 */
enum SendSocketTypes {
  //ZMQ_PAIR
  PUB = ZMQ_PUB,
  //ZMQ_SUB
  //ZMQ_REQ
  //ZMQ_REP
  //ZMQ_XREQ
  //ZMQ_XREP
  //ZMQ_PULL
  PUSH = ZMQ_PUSH,
};

/** List of zmq socket options to be set with setsockopt.
 */
enum SocketOptions {
  //ZMQ_HWM 1
  //ZMQ_SWAP 3
  //ZMQ_AFFINITY 4
  //ZMQ_IDENTITY 5
  SUBSCRIBE   = ZMQ_SUBSCRIBE,
  UNSUBSCRIBE = ZMQ_UNSUBSCRIBE,
  //ZMQ_RATE 8
  //ZMQ_RECOVERY_IVL 9
  //ZMQ_MCAST_LOOP 10
  //ZMQ_SNDBUF 11
  //ZMQ_RCVBUF 12
  //ZMQ_RCVMORE 13
};

} // zmq

#endif // RUBYK_INCLUDE_ZMQ_CONSTANTS_H_