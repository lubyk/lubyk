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
#include "lubyk/worker.h"
#include "lubyk/cocoa.h"

using namespace lubyk;

class Worker::Implementation : public lubyk::Thread {
public:
  Implementation() {
    // create a thread that will run NSApp
    startThread<Implementation, &Implementation::run>(this, NULL);
  }

  ~Implementation() {
    CFRunLoopStop(CFRunLoopGetCurrent());
    //[[NSRunLoop currentRunLoop] halt];
  }

  void run(Thread *thread) {
    //  release calling thread semaphore
    thread_ready();
    ScopedPool pool;
    printf("Run Loop\n");
    [NSApplication sharedApplication];
    [NSApp run];
    //[[NSRunLoop currentRunLoop] configureAsServer];
    //[[NSRunLoop currentRunLoop] run];
  }
};

Worker::Worker(lua_State *L)
 : lua_(L),
  zmq_context_(NULL),
  zmq_context_refcount_(0) {
  //impl_ = new Worker::Implementation;
  Mutex::lock();
}

Worker::~Worker() {
  //delete impl_;
  unlock();
}

void Worker::run() {
  printf("Run App\n");
  ScopedUnlock unlock(this);
  [NSApplication sharedApplication];
  [NSApp run];
}