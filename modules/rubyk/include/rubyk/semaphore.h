/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher - Buma (http://teti.ch).

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

#ifndef RUBYK_INCLUDE_RUBYK_SEMAPHORE_H_
#define RUBYK_INCLUDE_RUBYK_SEMAPHORE_H_

#include <assert.h>
#include <semaphore.h>
#include <errno.h>   // errno
#include <string.h>  // strerror
#include "rubyk/non_copyable.h"
#include "rubyk/mutex.h"

namespace rubyk {

class Semaphore : private NonCopyable {
public:
  Semaphore(int resource_count = 0);

  ~Semaphore();

  /** Get resource or wait until it is available.
   */
  inline void acquire() {
    sem_wait(semaphore_);
  }

  /** Release resource.
   */
  inline void release() {
    sem_post(semaphore_);
  }

  /** Acquire multiple resources at the same time.
   */
  inline void acquire(size_t count) {
    assert(count <= resource_count_);
    ScopedLock lock(acquire_mutex_);
    for (size_t i=0; i < count; ++i) {
      acquire();
    }
  }

  /** Release multiple resources at the same time.
   */
  inline void release(size_t count) {
    assert(count <= resource_count_);
    for (size_t i=0; i < count; ++i) {
      release();
    }
  }

  /** Acquire all resources protected by this semaphore
   */
  inline void acquire_all() {
    acquire(resource_count_);
  }

  /** Release all resources previously acquired with a call to 'acquire_all'.
   */
  inline void release_all() {
    release(resource_count_);
  }

 private:
  size_t resource_count_;
  Mutex acquire_mutex_;
  sem_t *semaphore_;
};

}  // core

#endif  // RUBYK_INCLUDE_RUBYK_SEMAPHORE_H_