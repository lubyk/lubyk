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
#ifndef LUBYK_INCLUDE_ZMQ_CONTEXT_H_
#define LUBYK_INCLUDE_ZMQ_CONTEXT_H_


#include "dub/dub.h"

#include <stdlib.h> // rand()
#include <time.h>   // time()
#include <assert.h> // assert()
#include <signal.h> // signal(), SIG_DFL, ...

#include "zmq.h"
#include "lubyk/msgpack.h"
#include "lubyk/time_ref.h"
#include "zmq/Socket.h"

using namespace lubyk;

namespace zmq {

/** ZeroMQ context.
 *
 * @dub register:'Context_core'
 *      string_format: %%f
 *      string_args: self->count()
 */
class Context {
  friend class Socket;

  /** Context use by zmq::Socket.
   */
  void *zmq_context_;

  /** Counts the number of zmq::Socket depending on the
   * socket.
   */
  size_t zmq_context_refcount_;
public:
  Context();
  ~Context();
};
} // zmq

#endif // LUBYK_INCLUDE_ZMQ_CONTEXT_H_

