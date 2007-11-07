#ifndef _MUTEX_H_
#define _MUTEX_H_
#include <pthread.h>
#include <cstdio>

/** Simple wrapper class for POSIX mutex. */
class Mutex
{
public:
  Mutex()
  { 
    pthread_mutex_init(&mMutex, NULL);
  }
  
  virtual ~Mutex()
  { pthread_mutex_destroy(&mMutex); }
  
  /** If the mutex is locked by another thread, waits until it is unlocked, lock and continue.
    * If the mutex is not locked. Lock and continue.
    * If this thread locked it, then it' really stupid and we print a bad warning. */
  void lock()
  { 
    fflush(stdout);
    if (pthread_mutex_lock(&mMutex)) {
      fprintf(stderr, "This thread is really silly, it's trying to lock it's own lock !\n");
    } 
  }
  
  /** Release the lock so others can work on the data. */
  void unlock()
  {
    fflush(stdout);
    if (pthread_mutex_unlock(&mMutex)) {
      fprintf(stderr, "This thread is really silly, it's trying to unlock a lock it does not own !\n");
    }
  }
private:
  pthread_mutex_t mMutex;
};

#endif