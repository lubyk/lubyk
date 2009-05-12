#ifndef OSCIT_INCLUDE_OSCIT_THREAD_H_
#define OSCIT_INCLUDE_OSCIT_THREAD_H_

#include <csignal>
#include <fstream>
#include <sys/timeb.h> // ftime
#include "assert.h"

#include "oscit/mutex.h"

namespace oscit {

class Thread : public Mutex
{
 public:
  TYPED("Mutex.Thread")

  Thread() : owner_(NULL), thread_id_(NULL), should_run_(false) {
    if (!sThisKey) pthread_key_create(&sThisKey, NULL); // create a key to find 'this' object in new thread
  }

  virtual ~Thread() {
    kill();
  }

  /** Start a new thread with the given parameter. The class should check if it
   *  should stop using a typical @while (thread->run())@. If the thread is interrupted with
   *  a SIGTERM, the class's terminate() method is called. */
  template<class T, void(T::*Tmethod)(Thread*)>
  void start(T *owner, void *parameter = NULL) {
    if (thread_id_) {
      fprintf(stderr, "Trying to start thread when it is already running ! (in Thread::start)");
      return;
    }

    owner_      = owner;
    parameter_  = parameter;
    should_run_ = true;

    lock();  // unlock in new thread
    pthread_create( &thread_id_, NULL, &start_thread<T,Tmethod>, (void*)this);

    // make sure thread is properly started (signals registered) in case we die right after
    lock();    // wait for created thread to unlock
    unlock();  // created thread can run
  }

  /** Start a new thread with the given parameter. The class should check if it
   *  should stop using a typical @while (thread->run())@. If the thread is interrupted with
   *  a SIGTERM, the class's terminate() method is called. */
  template<class T, void(T::*Tmethod)()>
  void start(T *owner, void *parameter = NULL) {
    if (thread_id_) {
      fprintf(stderr, "Trying to start thread when it is already running ! (in Thread::start)");
      return;
    }

    owner_      = owner;
    parameter_  = parameter;
    should_run_ = true;

    lock();  // unlock in new thread
    pthread_create( &thread_id_, NULL, &start_thread<T,Tmethod>, (void*)this);

    // make sure thread is properly started (signals registered) in case we die right after
    lock();    // wait for created thread to unlock
    unlock();  // created thread can run
  }

  inline bool should_run() {
    return should_run_;
  }

  /** Kill thread (do not make this a virtual).
   */
  void kill() {
    if (!thread_id_) return;  // not running
    assert(!pthread_equal(thread_id_, pthread_self()));

    pthread_kill(thread_id_, SIGTERM);
    pthread_join(thread_id_, NULL);
    thread_id_ = NULL;
  }

  /** Wait for thread to finish. */
  void join() {
    if (thread_id_) {
      // TODO: check we do not join on self ?
      pthread_join(thread_id_, NULL);
      thread_id_ = NULL;
    }
  }

  /** Tell thread to quit (called from within own thread or when we can wait for next loop).
   */
  void quit() {
    should_run_ = false;
  }

  /** Set thread priority to high. */
  void high_priority();

  /** Set thread priority to normal. */
  void normal_priority();

  static void millisleep(float milliseconds) {
    struct timespec sleeper;
    sleeper.tv_sec  = 0;
    sleeper.tv_nsec = (unsigned int)(milliseconds * 1000000.0);
    nanosleep (&sleeper, NULL);
  }

 public:
  static pthread_key_t sThisKey;   /**< Key to retrieve 'this' value from a running thread. */
  void                 *parameter_; /**< Any parameter that the started method could use. */

 private:

  /** Get "this" (used in static callbacks). */
  static Thread * thread_this() {
   return (Thread*) pthread_getspecific(sThisKey);
  }

  /** Set 'this' value for the current thread so we can
   * find our object back.
   */
  static void set_thread_this(Thread *thread) {
   pthread_setspecific(sThisKey, (void*)thread);
  }

  /** Static function to start a new thread. */
  template<class T, void(T::*Tmethod)(Thread*)>
  static void * start_thread(void *thread_ptr) {
    Thread * thread = (Thread*)thread_ptr;
     // begin of new thread
    set_thread_this(thread);

    signal(SIGTERM, terminate); // register a SIGTERM handler
    signal(SIGINT,  terminate);

    T *owner = (T*)thread->owner_;
    thread->should_run_ = true;

    // signals installed, we can free parent thread
    thread->unlock();  // --> parent can continue

    thread->lock();
      (owner->*Tmethod)(thread);
    thread->unlock();
    return NULL;
  }

  /** Static function to start a new thread. */
  template<class T, void(T::*Tmethod)()>
  static void * start_thread(void *thread_ptr) {
    Thread * thread = (Thread*)thread_ptr;
     // begin of new thread
    set_thread_this(thread);

    signal(SIGTERM, terminate); // register a SIGTERM handler
    signal(SIGINT,  terminate);

    T *owner = (T*)thread->owner_;
    thread->should_run_ = true;

    // signals installed, we can free parent thread
    thread->unlock();  // --> parent can continue

    thread->lock();
      (owner->*Tmethod)();
    thread->unlock();
    return NULL;
  }

  /** Thread should stop. */
  static void terminate(int sig) {
    thread_this()->quit();
  }

  void      *owner_;

  pthread_t thread_id_;
  int normal_sched_policy_;                /**< Thread's original scheduling priority (normal_priority). */
  struct sched_param normal_thread_param_; /**< Scheduling parameters for commands (lower). */

 protected:
  bool should_run_;
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_THREAD_H_
