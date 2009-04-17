#ifndef _OSCIT_MUTEX_H_
#define _OSCIT_MUTEX_H_
#include "oscit/typed.h"
#include <pthread.h>
#include <cstdio>

namespace oscit {

/** Simple wrapper class for POSIX mutex. */
class Mutex : public Typed
{
public:
  TYPED("Mutex")
  
  Mutex() : mutex_owner_(NULL) { 
    pthread_mutex_init(&mutex_, NULL);
  }
  
  virtual ~Mutex() {
    pthread_mutex_destroy(&mutex_);
  }
  
  /** If the mutex is locked by another thread, waits until it is unlocked, lock and continue.
    * If the mutex is not locked. Lock and continue.
    * If this thread locked it, ignore. */
  void lock() { 
    fflush(stdout);
    pthread_t current_thread_id = pthread_self();
    if (mutex_owner_ != current_thread_id) {
      // only try to lock if we do not own the current lock (avoid deadlock)
      pthread_mutex_lock(&mutex_);
      mutex_owner_ = current_thread_id;
    }
  }
  
  /** Release the lock so others can work on the data. */
  void unlock() {
    fflush(stdout);
    if (mutex_owner_ == pthread_self()) {
      // only unlock a lock we own
      pthread_mutex_unlock(&mutex_);
      mutex_owner_ = NULL;
    }
  }
 private:
  pthread_t mutex_owner_;
  pthread_mutex_t mutex_;
};

} // oscit
#endif // _OSCIT_MUTEX_H_