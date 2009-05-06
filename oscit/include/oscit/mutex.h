#ifndef OSCIT_INCLUDE_OSCIT_MUTEX_H_
#define OSCIT_INCLUDE_OSCIT_MUTEX_H_
#include "oscit/typed.h"
#include <pthread.h>
#include <cstdio>

namespace oscit {

/** Simple wrapper class for a POSIX fast mutex. */
class Mutex : public Typed
{
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
  void lock() {
    pthread_mutex_lock(&mutex_);
  }
  
  /** Release the lock so others can work on the data. */
  void unlock() {
    pthread_mutex_unlock(&mutex_);
  }
 private:
  pthread_mutex_t mutex_;
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_MUTEX_H_
