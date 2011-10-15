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

#ifndef LUBYK_INCLUDE_LUBYK_RECURSIVE_MUTEX_H_
#define LUBYK_INCLUDE_LUBYK_RECURSIVE_MUTEX_H_
#include <pthread.h>
#include <assert.h>
#include <cstdio>
#include <string.h> // strerror

#include "lubyk/mutex.h"
#include "lubyk/non_copyable.h"

namespace lubyk {

/** Recursive Mutex: experimental DO NOT USE !
 */
class RMutex : private NonCopyable
{
  Mutex mutex_;
  int counter_;
  pthread_t owner_;
public:
  RMutex()
   : counter_(0),
     owner_(NULL) {}

  ~RMutex() {}

  /** If the mutex is locked by another thread, waits until it is unlocked, lock and continue.
    * If the mutex is not locked. Lock and continue.
    * If this thread locked it: bang! */
  inline void lock() {
    pthread_t id = pthread_self();
    if (owner_ == id) {
      ++counter_;
    } else {
      mutex_.lock();
      counter_ = 1;
      owner_   = id;
    }
  }

  /** Release the lock so others can work on the data. */
  inline void unlock() {
    if (--counter_ == 0) {
      owner_ = NULL;
      mutex_.unlock();
    }
  }

protected:
  /** Used for debugging and testing. */
  int lockCount() const {
    return counter_;
  }
};

/** FIXME: replace with a non-recursive mutex: we do not need recursion any
 * more !
 */
/** Scoped recursive Mutex Lock for exclusive access to a resource.
 */
class ScopedLock : private NonCopyable {
  RMutex *mutex_ptr_;
public:
  ScopedLock(RMutex *mutex) : mutex_ptr_(mutex) {
    mutex_ptr_->lock();
  }

  ScopedLock(RMutex &mutex) : mutex_ptr_(&mutex) {
    mutex_ptr_->lock();
  }

  ~ScopedLock() {
    mutex_ptr_->unlock();
  }
};


/** Scoped recursive Mutex Unlock to release exclusive access to a resource.
 */
class ScopedUnlock : private NonCopyable
{
  RMutex *mutex_ptr_;
public:
  ScopedUnlock(RMutex *mutex) : mutex_ptr_(mutex) {
    mutex_ptr_->unlock();
  }

  ScopedUnlock(RMutex &mutex) : mutex_ptr_(&mutex) {
    mutex_ptr_->unlock();
  }

  ~ScopedUnlock() {
    mutex_ptr_->lock();
  }
};


} // lubyk

#endif // LUBYK_INCLUDE_LUBYK_RECURSIVE_MUTEX_H_
