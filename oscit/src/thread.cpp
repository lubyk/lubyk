#include "errno.h"   // errno
#include "string.h"  // strerror
#include <sys/timeb.h> // ftime

#include "oscit/thread.h"

#ifdef __macosx__
#define SCHED_HIGH_PRIORITY 47
#endif

#ifdef __linux__
#define SCHED_HIGH_PRIORITY 42
#endif

namespace oscit {

struct TimeRef : public timeb {};

/** Called by commands and other low priority threads. */
void Thread::normal_priority()
{
  pthread_t id = pthread_self();

  if (pthread_setschedparam(id, normal_sched_policy_, &normal_thread_param_)) {
    fprintf(stderr, "Could not set command thread priority to %i.\n", normal_thread_param_.sched_priority);
  }
}

/** Called during startup to increase thread priority. */
void Thread::high_priority() {
  struct sched_param param;
  int policy;
  pthread_t id = pthread_self();

  // save original scheduling parameters
  pthread_getschedparam(id, &normal_sched_policy_, &normal_thread_param_);
  
  // set to high priority
  param = normal_thread_param_;
  param.sched_priority = SCHED_HIGH_PRIORITY;  // magick number
  policy = SCHED_RR;                           // realtime, round robin
  
  
  if (pthread_setschedparam(id, policy, &param)) {
    fprintf(stderr, "Could not set thread priority to %i (%s). You might need to run the application as super user.\n", param.sched_priority, strerror(errno));
  }
}


void Thread::millisleep(float milliseconds) {
  struct timespec sleeper;
  sleeper.tv_sec  = 0;
  sleeper.tv_nsec = (unsigned int)(milliseconds * 1000000.0);
  nanosleep (&sleeper, NULL);
}

time_t Thread::real_time(const TimeRef *time_ref) {
  TimeRef t;
  ftime(&t);
  return ((t.time - time_ref->time) * 1000) + t.millitm - time_ref->millitm;
}

TimeRef *Thread::new_time_ref() {
  TimeRef *time_ref = new TimeRef;
  ftime(time_ref);
  return time_ref;
}

TimeRef *Thread::delete_time_ref(TimeRef *time_ref) {
  delete time_ref;
  return NULL;
}
} // oscit
