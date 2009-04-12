#include "oscit/thread.h"

namespace oscit {
  
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
                             // FIXME: other magick numbers for Linux, Windows
  param.sched_priority = 47; // magick number for Mac OS X
  policy = SCHED_RR;         // round robin
  
  
  if (pthread_setschedparam(id, policy, &param)) {
    fprintf(stderr, "Could not set thread priority to %i.\n", param.sched_priority);
  }
}

} // oscit