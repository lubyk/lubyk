#ifndef OSCIT_INCLUDE_OSCIT_THREAD_H_
#define OSCIT_INCLUDE_OSCIT_THREAD_H_

#include <csignal>
#include <fstream>
#include <semaphore.h>
#include "assert.h"

#include "oscit/mutex.h"

namespace oscit {

struct TimeRef;

class Thread : public Mutex
{
 public:
  TYPED("Mutex.Thread")

  Thread() : owner_(NULL), thread_id_(NULL), should_run_(false) {
    if (!sThisKey) pthread_key_create(&sThisKey, NULL); // create a key to find 'this' object in new thread
    sem_init(&semaphore_, 0, 0);
  }

  virtual ~Thread() {
    kill();
    sem_destroy(&semaphore_);
  }

  /** Start a new thread with the given parameter. The class should check if it
   *  should stop using a typical @while (thread->run())@. If the thread is interrupted with
   *  a SIGTERM, the class's terminate() method is called. */
  template<class T, void(T::*Tmethod)(Thread*)>
  void start_thread(T *owner, void *parameter = NULL) {
    if (thread_id_) {
      fprintf(stderr, "Trying to start thread when it is already running ! (in Thread::start)");
      return;
    }

    owner_      = owner;
    parameter_  = parameter;
    should_run_ = true;

    pthread_create( &thread_id_, NULL, &s_start_thread<T,Tmethod>, (void*)this);

    // make sure thread is properly started (signals registered) in case we die right after
    sem_wait(&semaphore_);
  }

  /** Start a new thread with the given parameter. The class should check if it
   *  should stop using a typical @while (thread->run())@. If the thread is interrupted with
   *  a SIGTERM, the class's terminate() method is called. */
  template<class T, void(T::*Tmethod)()>
  void start_thread(T *owner, void *parameter = NULL) {
    if (thread_id_) {
      fprintf(stderr, "Trying to start thread when it is already running ! (in Thread::start)");
      return;
    }

    owner_      = owner;
    parameter_  = parameter;
    should_run_ = true;

    pthread_create( &thread_id_, NULL, &s_start_thread<T,Tmethod>, (void*)this);

    // make sure thread is properly started (signals registered) in case we die right after
    sem_wait(&semaphore_);
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

  static void millisleep(float milliseconds);
  
  /** Get current real time in [ms] since reference.
   */
  static time_t real_time(const TimeRef *time_ref);
  

  static TimeRef *new_time_ref();

  static TimeRef *delete_time_ref(TimeRef *time_ref);
  
  /** This method should be called by started thread when it has
   * properly started an it is ready. The creating thread locks until
   * this method is called.
   */
  void thread_ready() {
    // signals installed, we can free parent thread
    sem_post(&semaphore_);
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
  static void * s_start_thread(void *thread_ptr) {
    Thread * thread = (Thread*)thread_ptr;
     // begin of new thread
    set_thread_this(thread);

    signal(SIGTERM, terminate); // register a SIGTERM handler
    signal(SIGINT,  terminate);

    T *owner = (T*)thread->owner_;
    thread->should_run_ = true;

    (owner->*Tmethod)(thread);

    return NULL;
  }

  /** Static function to start a new thread. */
  template<class T, void(T::*Tmethod)()>
  static void * s_start_thread(void *thread_ptr) {
    Thread * thread = (Thread*)thread_ptr;
     // begin of new thread
    set_thread_this(thread);

    signal(SIGTERM, terminate); // register a SIGTERM handler
    signal(SIGINT,  terminate);

    T *owner = (T*)thread->owner_;
    thread->should_run_ = true;

    (owner->*Tmethod)();
    
    return NULL;
  }

  /** Thread should stop. */
  static void terminate(int sig) {
    thread_this()->quit();
  }

  void      *owner_;

  pthread_t thread_id_;
  int normal_sched_policy_;                /**< Thread's original scheduling policy (normal_priority). */
  struct sched_param normal_thread_param_; /**< Scheduling parameters for commands (lower). */

 protected:
  bool should_run_;

 private:
  sem_t semaphore_;

};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_THREAD_H_
