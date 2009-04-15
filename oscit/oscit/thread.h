#ifndef _THREAD_H_
#define _THREAD_H_
#include "oscit/mutex.h"

#include <csignal>
#include <fstream>
namespace oscit {

class Thread : public Mutex
{
 public:
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
    
    owner_     = owner;
    parameter_ = parameter;
    pthread_create( &thread_id_, NULL, &start_thread<T,Tmethod>, (void*)this);
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
    
    owner_     = owner;
    parameter_ = parameter;
    pthread_create( &thread_id_, NULL, &start_thread<T,Tmethod>, (void*)this);
  }
  
  inline bool run() {
    return should_run_;
  }
  
  /** Kill thread. */
  void kill() {
    if (thread_id_) {
      pthread_kill(thread_id_, SIGTERM);
      pthread_join(thread_id_, NULL);
      thread_id_ = NULL;
    }
  }
  
  /** Tell thread to stop after current loop. */
  void stop() {
    should_run_ = false;
  }
  
  /** Set thread priority to high. */
  void high_priority();
  
  /** Set thread priority to normal. */
  void normal_priority();
  
 public:
  static pthread_key_t sThisKey;   /**< Key to retrieve 'this' value from a running thread. */
  void                 *parameter_; /**< Any parameter that the started method could use. */
  
 private:
  
  /** Get "this" (used in static callbacks). */
  static Thread * thread_this() {
   return (Thread*) pthread_getspecific(sThisKey);
  }

  /** Set 'this' value for the current thread (used by Rubyk when starting a new thread). */
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
    (owner->*Tmethod)(thread);
    
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
    (owner->*Tmethod)();
    
    return NULL;
  }
  
  /** Thread should stop. */
  static void terminate(int sig) {
    thread_this()->stop();
  }
  
  void      *owner_;
  
  pthread_t thread_id_;
  int normal_sched_policy_;                /**< Thread's original scheduling priority (normal_priority). */ 
  struct sched_param normal_thread_param_; /**< Scheduling parameters for commands (lower). */
  
 protected:
  bool      should_run_;
};

} // oscit
#endif // _THREAD_H_