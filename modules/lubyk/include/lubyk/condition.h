/*
  ==============================================================================

   This file is part of the LUBYK library (http://lubyk.org)
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

#ifndef LUBYK_INCLUDE_LUBYK_CONDITION_H_
#define LUBYK_INCLUDE_LUBYK_CONDITION_H_

// TODO: refactor all this to move all thread specific code into 'cpp' file !
#include <errno.h>   // errno
#include <string.h>  // strerror
#include <pthread.h>
#include <csignal>
#include <fstream>
#include <semaphore.h>

#include "assert.h"

#include "lubyk/mutex.h"

namespace lubyk {

/** A class to ease multi-threading. The Condition helps lock a given thread
 * while it waits for another to finish some work.
 *
 * Typical usage is:
 * 1. thread1 cond->lock()
 * 2. start thread2
 * 3. thread1 cond->wait()
 * 3. in thread2 cond->signal()
 * 4. thread1 cond->unlock()
 */
class Condition : public Mutex {
  pthread_cond_t cond_;
 public:
  Condition() {
    if (pthread_cond_init(&cond_, NULL)) {
      fprintf(stderr, "Could not create lubyk::Condition\n");
    };
  }

  ~Condition() {
    if (pthread_cond_destroy(&cond_)) {
      fprintf(stderr, "Could not destroy lubyk::Condition\n");
    };
  }

  /** This makes the calling thread sleep until 'signal' is called. It should
   * be called after having been locked by the thread that created the Condition.
   */
  void wait() {
    if (pthread_cond_wait(&cond_, &mutex_)) {
      fprintf(stderr, "Could not call wait() on lubyk::Condition\n");
    }
  }

  /** Release any waiting thread.
   */
  void signal() {
    // the lock/unlock ensures that signal is not called before wait()
    lock();
    unlock();
    if (pthread_cond_signal(&cond_)) {
      fprintf(stderr, "Could not call signal() on lubyk::Condition\n");
    }
  }
};

} // lubyk

#endif // LUBYK_INCLUDE_LUBYK_CONDITION_H_
