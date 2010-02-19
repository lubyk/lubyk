/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

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

#include "worker.h"
#include "node.h"


void Worker::register_looped_node(Node *node) {
  free_looped_node(node);
  looped_nodes_.push_back(node);
}


void Worker::miss_event(const Event *event) {
  fprintf(stderr, "Not registering event from %s: (trigger too soon: %li [ms]).\n",
                  event->node()->do_inspect().str().c_str(),
                  (event->when_ - current_time_));
}

void Worker::free_looped_node(Node *node) {
  std::deque<Node*>::iterator it;
  std::deque<Node*>::iterator end = looped_nodes_.end();
  for(it = looped_nodes_.begin(); it < end; it++) {
    if (*it == node) {
      looped_nodes_.erase(it);
      break;
    }
  }
}

void Worker::start_worker(Thread *thread) {
  signal(SIGUSR1, Worker::reload); // register a SIGTERM handler
  thread->high_priority();
  thread->thread_ready();
  run();
}

void Worker::reload_queue() {
  if (sleeping_) {
    ScopedLock lock(sleep_lock_);
    send_signal(SIGUSR1); // SIGUSR1
  }
}

void Worker::run() {
  ScopedLock lock(sleep_lock_);
  Event *next_event;
  time_t wait_duration;
  struct timespec sleeper;
  sleeping_ = false;

  while (should_run_) {
    // ======== setup sleep duration
    sleeping_ = true;
      if (events_queue_.get(&next_event)) {
        wait_duration = next_event->when_ - time_ref_.elapsed();
        sleeper.tv_sec  = wait_duration / 1000;
        sleeper.tv_nsec = (wait_duration % 1000) * 1000000; // 1'000'000
      } else {
        // sleep forever
        sleeper.tv_sec  = 1000;
        sleeper.tv_nsec = 0;
      }
    { ScopedUnlock unlock(sleep_lock_);
      // this is the only place where we can be interrupted
      nanosleep(&sleeper, NULL);
    }
    sleeping_ = false;

    // ======== worker's job loop ====
    current_time_ = time_ref_.elapsed();

    // execute events that must occur on each loop (io operations)
    trigger_loop_events();

    // trigger events in the queue
    pop_events();
  }
}

// Used for testing only
bool Worker::loop() {
  current_time_ = time_ref_.elapsed();

  // execute events that must occur on each loop (io operations)
  trigger_loop_events();

  // trigger events in the queue
  pop_events();
  return should_run_;
}

void Worker::reload(int sig) {
  signal(SIGUSR1, Worker::reload); // register a SIGTERM handler
  //((Worker*)thread_this())->run();
  return;
}

void Worker::pop_events() {
  Event *e;
  time_t real_time = current_time_;
  while( events_queue_.get(&e) && real_time >= e->when_) {
    // FIXME: we have a possible race condition here
    // could we extend COrderedList<Event*> with get_and_pop(real_time, &e) ?
    current_time_ = e->when_;
    e->trigger();
    delete e;
    events_queue_.pop();
  }
  current_time_ = real_time;
}

void Worker::pop_all_events() {
  Event *e;
  while( events_queue_.get_and_pop(&e)) {
    current_time_ = e->when_;
    if (e->forced_) e->trigger();
    delete e;
  }
}

void Worker::trigger_loop_events() {
  std::deque<Node *>::iterator it;
  std::deque<Node *>::iterator end = looped_nodes_.end();
  for(it = looped_nodes_.begin(); it < end; it++) {
    (*it)->bang(gNilValue);
  }
}


