/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher - Buma (http://teti.ch).

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

#ifndef RUBYK_INCLUDE_RUBYK_TIMER_H_
#define RUBYK_INCLUDE_RUBYK_TIMER_H_

#include "rubyk/thread.h"
#include "rubyk/mutex.h"
#include "rubyk/time_ref.h"

namespace rubyk {

template<class T, void(T::*Tmethod)()>
class Timer {
public:
  Timer(T *owner, time_t interval = 0)
      : owner_(owner),
        interval_(interval),
        last_interval_(interval),
        running_(false) {}

  ~Timer() {
    // we could let Thread cancel the loop in ~Thread, but we
    // prefer to ensure we don't access stale data.
    stop();
  }

  void start(time_t interval) {
    ScopedLock lock(mutex_);
    last_interval_ = interval_;
    interval_ = interval;
    if (running_) {
      should_run_ = false;
      interrupt();
      thread_.join();
    }
    thread_.start_thread<Timer, &Timer::start_thread>(this, NULL);
  }

  void start() {
    ScopedLock lock(mutex_);
    if (running_) {
      should_run_ = false;
      interrupt();
      thread_.join();
    }
    thread_.start_thread<Timer, &Timer::start_thread>(this, NULL);
  }

  void stop() {
    ScopedLock lock(mutex_);
    if (running_) {
      should_run_ = false;
      interrupt();
      thread_.join();
    }
  }

  void set_interval(time_t interval) {
    ScopedLock lock(mutex_);
    last_interval_ = interval_;
    interval_ = interval;
    if (running_) {
      interrupt();
    }
  }

  bool running() const {
    return running_;
  }

private:
  void run() {
    time_t wait_duration;
    time_t now;
    time_t next_fire = time_ref_.elapsed(); // this is our anchor
    struct timespec sleeper;

    while (should_run_) {
      { ScopedLock lock(mutex_);

        now = time_ref_.elapsed();

        // This can be used to ensure test...
        // printf("logical: %li real: %li wait: %li\n", next_fire, now, wait_duration);

        if (next_fire > now) {
          // interrupted
          // we do not fire on interval change
          next_fire -= last_interval_;
        } else {
          (owner_->*Tmethod)();
        }

       /* Timer sync issues:
        * We compute the new logical fire and sleep to sync real trigger as close as we can.
        */
        next_fire += interval_;

        wait_duration = next_fire - now;
        if (wait_duration < 0) {
          if (interval_ > 0) {
            // this can happen if we changed interval_ during sleep
            // next_fire + k * interval_ > now
            int k = ((now - next_fire) / interval_) + 1; // +1 because we need to respect '>'
            // skip 'k' occurences
            next_fire += k * interval_;
            wait_duration = next_fire - now;
          } else {
            // run as fast as we can
            // we could limit this to 1ms...
            wait_duration = 0;
          }
        }

        sleeper.tv_sec  = wait_duration / 1000;
        sleeper.tv_nsec = (wait_duration % 1000) * 1000000; // 1'000'000
      }

      nanosleep(&sleeper, NULL);
    }

    running_ = false;
  }

  static void interrupt_sleep(int sig) {
    // we need to reregister signal
    signal(SIGUSR1, Timer::interrupt_sleep);
    // we just interrupted sleep, let the loop continue
    return;
  }

  /** Must be called within a loop lock.
   */
  void interrupt() {
    thread_.send_signal(SIGUSR1);
  }

  void start_thread(Thread *runner) {
    signal(SIGUSR1, Timer::interrupt_sleep);
    should_run_ = true;
    running_ = true;
    runner->high_priority();
    runner->thread_ready();
    run();
  }

  /** Object to trigger.
   */
  T *owner_;

  /** Loop interval.
   */
  time_t interval_;

  /** We need this to anchor next_fire in case interval
   * changes.
   */
  time_t last_interval_;

  /** We want to hold very precise timing.
   */
  TimeRef time_ref_;

  /** Protect internals.
   */
  Mutex mutex_;

  /** Running thread.
   */
  Thread thread_;

  /** Flag indicating if the thread is actually running.
   */
  bool running_;

  /** Flag indicating that we want the timer to fire or die.
   */
  bool should_run_;
};

} // rubyk

#endif // RUBYK_INCLUDE_RUBYK_TIMER_H_