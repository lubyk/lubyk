#ifndef _THREAD_H_
#define _THREAD_H_
#include "oscit/mutex.h"

#include <csignal>
#include <fstream>
namespace oscit {

class Thread : public Mutex
{
 public:
  Thread() : owner_(NULL), thread_id_(NULL), use_signals_(false), quit_(false) {
    if (!sThisKey) pthread_key_create(&sThisKey, NULL); // create a key to find 'this' object in new thread
  }
  
  virtual ~Thread() {
    quit();
  }
  
  /** Start a new thread with the given parameter. The thread should check if it
   *  should stop using thread->run(). */
  template<class T, void(T::*Tmethod)(Thread*)>
  void start(T *owner, void *parameter) {
    owner_     = owner;
    parameter_ = parameter;
    pthread_create( &thread_id_, NULL, &start_thread<T,Tmethod>, (void*)this);
  }
  
  /** Start a new thread with the given parameter. If the thread is interrupted with
   *  a SIGTERM, it's quit() method is called. */
  template<class T, void(T::*Tmethod)(Thread*)>
  void start_using_signals(T *owner, void *parameter) {
    use_signals_ = true;
    owner_     = owner;
    parameter_ = parameter;
    pthread_create( &thread_id_, NULL, &start_thread_using_signals<T,Tmethod>, (void*)this);
  }
  
  inline bool run() {
    return !quit_;
  }
  
 public:
  static pthread_key_t sThisKey;   /**< Key to retrieve 'this' value from a running thread. */
  void                 *parameter_; /**< Any parameter that the started method could use. */
  
 private:

  void quit() {
   if (thread_id_) {
     if (use_signals_) {
       pthread_kill(thread_id_, SIGTERM);
     }
     quit_ = true;
     pthread_join(thread_id_, NULL);
     thread_id_ = NULL;
   }
  }

  /** Static function to start a new thread. */
  template<class T, void(T::*Tmethod)(Thread*)>
   static void * start_thread(void *thread) {
     // begin of new thread
     set_thread_this((Thread*)thread);

     signal(SIGTERM, term); // register a SIGTERM handler
     signal(SIGINT,  term);

     T *owner = (T*)((Thread*)thread)->owner_;
     
     (owner->*Tmethod)((Thread*)thread);
     
     return NULL;
   }
   
   
  /** Static function to start a new thread which will use signals to interupt work. Class 'T' should respond to "quit()". */
  template<class T, void(T::*Tmethod)(Thread*)>
  static void * start_thread_using_signals(void *thread) {
  // begin of new thread
  set_thread_this((Thread*)thread);

  signal(SIGTERM, terminate_owner<T>); // register a SIGTERM handler
  signal(SIGINT,  terminate_owner<T>);

  T *owner = (T*)((Thread*)thread)->owner_;

  (owner->*Tmethod)((Thread*)thread);

  return NULL;
  }

  
  /** Get "this" (used in static callbacks). */
  static Thread * thread_this() {
    return (Thread*) pthread_getspecific(sThisKey);
  }
  
  /** Set 'this' value for the current thread (used by Rubyk when starting a new thread). */
  static void set_thread_this(Thread *thread) {
    pthread_setspecific(sThisKey, (void*)thread);
  }
  
  /** Thread should stop. */
  static void term(int sig) {
    thread_this()->quit();
  }
  
  /** Thread should stop. */
  template<class T>
  static void terminate_owner(int sig) {
    ((T*)(thread_this()->owner_))->quit();
  }
  
  void      *owner_;
  pthread_t thread_id_;
  bool      use_signals_;
  bool      quit_;
};

} // oscit
#endif // _THREAD_H_