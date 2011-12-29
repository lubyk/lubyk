/*
  ==============================================================================

   This file is part of the LUBYK library (http://lubyk.org)
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

#ifndef LUBYK_INCLUDE_LUBYK_THREAD_H_
#define LUBYK_INCLUDE_LUBYK_THREAD_H_

// TODO: refactor all this to move all thread specific code into 'cpp' file !
#include <errno.h>   // errno
#include <string.h>  // strerror
#include <csignal>
#include <fstream>
#include <semaphore.h>

#include "assert.h"

#include "lubyk/mutex.h"
#include "lubyk/condition.h"

//#error "SHOULD NOT BE NEEDED"
namespace lubyk {

class Thread : public Mutex {
 public:
  Thread();

  virtual ~Thread();

  /** Start a new thread with a class method. */
  template<void(*Tmethod)(Thread*)>
  void startThread(void *parameter = NULL) {
    ScopedSLock lock(condition_);
    if (isRunning()) {
      fprintf(stderr, "Trying to start thread when it is already running ! (in Thread::start)");
      return;
    }

    owner_      = NULL;
    static_method_ = Tmethod;
    parameter_  = parameter;
    should_run_ = true;


    pthread_create( &thread_id_, NULL, &s_start_static_thread, (void*)this);

    // make sure thread is properly started (signals registered) in case we die right after
    // TODO: we could use timedwait if something goes wrong during thread initialization... ?
    condition_.wait();
  }

  /** Start a new thread with a static function. */
  void startThread(void (*static_method)(Thread*), void *parameter = NULL) {
    ScopedSLock lock(condition_);
    if (isRunning()) {
     fprintf(stderr, "Trying to start thread when it is already running ! (in Thread::start)");
     return;
    }

    owner_      = NULL;
    static_method_ = static_method;
    parameter_  = parameter;
    should_run_ = true;

    pthread_create( &thread_id_, NULL, &s_start_static_thread, (void*)this);

    // make sure thread is properly started (signals registered) in case we die right after
    condition_.wait();
  }

  /** Start a new thread with the given parameter. The class should check if it
   *  should stop using a typical "while (thread->run())". If the thread is interrupted with
   *  a SIGTERM, the class's terminate() method is called. */
  template<class T, void(T::*Tmethod)(Thread*)>
  void startThread(T *owner, void *parameter = NULL) {
    ScopedSLock lock(condition_);
    if (isRunning()) {
      fprintf(stderr, "Trying to start thread when it is already running ! (in Thread::start)");
      return;
    }

    owner_      = owner;
    parameter_  = parameter;
    should_run_ = true;

    pthread_create( &thread_id_, NULL, &s_startThread<T,Tmethod>, (void*)this);

    // make sure thread is properly started (signals registered) in case we die right after
    condition_.wait();
  }

  /** Start a new thread with the given parameter. The class should check if it
   *  should stop using a typical "while (thread->run())". If the thread is interrupted with
   *  a SIGTERM, the class's terminate() method is called. */
  template<class T, void(T::*Tmethod)()>
  void startThread(T *owner, void *parameter = NULL) {
    ScopedSLock lock(condition_);
    if (isRunning()) {
      fprintf(stderr, "Trying to start thread when it is already running ! (in Thread::start)");
      return;
    }

    owner_      = owner;
    parameter_  = parameter;
    should_run_ = true;

    pthread_create( &thread_id_, NULL, &s_startThread<T,Tmethod>, (void*)this);

    // make sure thread is properly started (signals registered) in case we die right after
    condition_.wait();
  }

  inline bool shouldRun() {
    return should_run_;
  }

  /** Kill thread (do not make this a virtual).
   */
  void kill() {
    should_run_ = false;
    if (!thread_id_) return;  // not running

    if (!pthread_equal(thread_id_, pthread_self())) {
      // pthread_kill(thread_id_, SIGTERM);
      pthread_cancel(thread_id_);
      pthread_join(thread_id_, NULL);
    }
    thread_id_ = NULL;
  }

  /** Send a signal to the running thread.
   */
  void sendSignal(int sig) {
    assert(!pthread_equal(thread_id_, pthread_self()));
    pthread_kill(thread_id_, sig);
  }

  void registerSignal(int sig) {
    assert(pthread_equal(thread_id_, pthread_self()));
    signal(sig, terminate);
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

  /** Return true if the thread is running.
   */
  bool isRunning() {
    return thread_id_ != NULL;
  }

  /** Set thread priority to high. */
  void highPriority();

  /** Set thread priority to normal. */
  void normalPriority();

  /** Sleep for a given number of milliseconds.
   */
  static void millisleep(double milliseconds);

  /** This method should be called by started thread when it has
   * properly started an it is ready. The creating thread locks until
   * this method is called.
   */
  void threadReady() {
    // signals installed, we can free parent thread
    condition_.signal();
  }

  /** Get "this" (used in static callbacks).
   */
  static Thread *thread_this() {
   return (Thread*) pthread_getspecific(sThisKey);
  }

  /** Key to retrieve 'this' value from a running thread.
   */
  static pthread_key_t sThisKey;

  void *parameter_; /**< Any parameter that the started method could use. */

 private:

  /** Set 'this' value for the current thread so we can
   * find our object back.
   */
  static void setThreadThis(Thread *thread) {
   pthread_setspecific(sThisKey, (void*)thread);
  }

  /** Static function to start a new thread
   * from a class method or static function.
   */
  static void * s_start_static_thread(void *thread_ptr) {
    Thread * thread = (Thread*)thread_ptr;
     // begin of new thread
    setThreadThis(thread);

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    // removing comment below = force cancelation to occur NOW, whatever the program is doing.
    // pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    thread->should_run_ = true;

    (*thread->static_method_)(thread);

    thread->thread_id_ = NULL; // not running anymore
    return NULL;
  }

  /** Static function to start a new thread. */
  template<class T, void(T::*Tmethod)(Thread*)>
  static void *s_startThread(void *thread_ptr) {
    Thread * thread = (Thread*)thread_ptr;
     // begin of new thread
    setThreadThis(thread);

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    //pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    T *owner = (T*)thread->owner_;
    thread->should_run_ = true;

    (owner->*Tmethod)(thread);

    thread->thread_id_ = NULL; // not running anymore
    return NULL;
  }

  /** Static function to start a new thread. */
  template<class T, void(T::*Tmethod)()>
  static void * s_startThread(void *thread_ptr) {
    Thread * thread = (Thread*)thread_ptr;
     // begin of new thread
    setThreadThis(thread);

    T *owner = (T*)thread->owner_;
    thread->should_run_ = true;

    (owner->*Tmethod)();

    thread->thread_id_ = NULL; // not running anymore
    return NULL;
  }

  /** Thread should stop. */
  static void terminate(int sig) {
    thread_this()->quit();
  }

  void *owner_;
  void (*static_method_)(Thread*);

  pthread_t thread_id_;
  int normal_sched_policy_;                /**< Thread's original scheduling policy (normal_priority). */
  struct sched_param normal_thread_param_; /**< Scheduling parameters for commands (lower). */

 protected:
  bool should_run_;

 private:
  Condition condition_;

};

} // core

#endif // LUBYK_INCLUDE_LUBYK_THREAD_H_
