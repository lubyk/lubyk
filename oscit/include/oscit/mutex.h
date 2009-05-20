#ifndef OSCIT_INCLUDE_OSCIT_MUTEX_H_
#define OSCIT_INCLUDE_OSCIT_MUTEX_H_
#include <pthread.h>
#include <cstdio>

#include "oscit/non_copyable.h"
#include "oscit/typed.h"

namespace oscit {

/** Simple wrapper class for a POSIX fast mutex. */
class Mutex : public Typed, private NonCopyable {
public:
  TYPED("Mutex")
  
  Mutex() { 
    pthread_mutex_init(&mutex_, NULL);
  }
  
  virtual ~Mutex() {
    pthread_mutex_destroy(&mutex_);
  }
  
  /** If the mutex is locked by another thread, waits until it is unlocked, lock and continue.
    * If the mutex is not locked. Lock and continue.
    * If this thread locked it: bang! */
  inline void lock() {
    pthread_mutex_lock(&mutex_);
  }
  
  /** Release the lock so others can work on the data. */
  inline void unlock() {
    pthread_mutex_unlock(&mutex_);
  }
 private:
  pthread_mutex_t mutex_;
};

class ScopedLock {
public:
  ScopedLock(Mutex *mutex) : mutex_ptr_(mutex) {
    mutex_ptr_->lock();
  }
  
  ScopedLock(Mutex &mutex) : mutex_ptr_(&mutex) {
    mutex_ptr_->lock();
  }
  
  ~ScopedLock() {
    mutex_ptr_->unlock();
  }
 private:
  Mutex *mutex_ptr_;
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_MUTEX_H_
