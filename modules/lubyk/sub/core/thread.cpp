/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
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

#include "lubyk/thread.h"

#include <errno.h>   // errno
#include <string.h>  // strerror
#include <csignal>
#include <fstream>
#include <semaphore.h>
#include <fcntl.h>   // O_CREAT

#include "assert.h"
#include "string.h"  // strerror


#ifdef __macosx__
#define SCHED_HIGH_PRIORITY 47
#endif

#ifdef __linux__
#define SCHED_HIGH_PRIORITY 42
#endif

pthread_key_t lubyk::Thread::sThisKey = NULL;

namespace lubyk {

Thread::Thread() : owner_(NULL), thread_id_(NULL), should_run_(false) {
  if (!sThisKey) pthread_key_create(&sThisKey, NULL); // create a key to find 'this' object in new thread
}

Thread::~Thread() {
  kill();
}

/** Called by commands and other low priority threads. */
void Thread::normalPriority()
{
  pthread_t id = pthread_self();

  if (pthread_setschedparam(id, normal_sched_policy_, &normal_thread_param_)) {
    fprintf(stderr, "Could not set command thread priority to %i.\n", normal_thread_param_.sched_priority);
  }
}

/** Called during startup to increase thread priority. */
void Thread::highPriority() {
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


void Thread::millisleep(double milliseconds) {
  struct timespec sleeper;
  time_t seconds = milliseconds / 1000;
  sleeper.tv_sec  = seconds;
  sleeper.tv_nsec = (unsigned int)((milliseconds - seconds * 1000) * 1000000.0);
  nanosleep(&sleeper, NULL);
}

} // core
