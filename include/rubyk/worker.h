/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

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

#ifndef RUBYK_INCLUDE_RUBYK_WORKER_H_
#define RUBYK_INCLUDE_RUBYK_WORKER_H_
/*
Structure of a running planet:

The Planet is an oscit::Root:
Root   <- Planet

It contains a single Worker that is passed to subnodes as context
Planet <>--- Worker
*/

#include <csignal>
#include <fstream>
#include <queue>

#include "oscit/mutex.h"
#include "rubyk/c_ordered_list.h"
#include "rubyk/event.h"



// is 2 [ms] too long ? Testing needed.
// 0.01 = 10 [us] = 0.00001 [s] = 100'000 [Hz] = 100 [kHz]
#define WORKER_SLEEP_MS 200 //0.01
#define ONE_SECOND 1000.0
#define ONE_MINUTE (60.0*ONE_SECOND)

class Worker : public Thread {
public:
  Worker(Root *root) : current_time_(0), root_(root) {}

  virtual ~Worker() {
    kill();
  }

  /** Run until quit (through a command or signal). */
  void start() {
    start_thread<Worker, &Worker::start_worker>(this, NULL);
  }

  /** Set if the planet should be running (only used with direct loop control). */
  void should_run(bool should_run) {
    should_run_ = should_run;
  }

  Root *root() { return root_; }

  /** Add an event to the event queue. The server is responsible for deleting the event. */
  void register_event(Event *event) {
    Event *top_event;
    if (event->when_ < time_ref_.elapsed()) {
      // FIXME: we should find other ways to detect an flood...
      miss_event(event);
    } else if (should_run_ || event->forced_) {
      // do not accept new events while we are trying to quit.
      events_queue_.push(event);
      if (events_queue_.get(&top_event) && top_event == event) {
        // our event is now on top of the list, we must interrupt sleep
        reload_queue();
      }
    }
  }

  template<class T, void(T::*Tmethod)(const Value&)>
  void register_event(time_t when, T *receiver, const Value &parameter) {
    register_event(new TEvent<T, Tmethod>(when, receiver, parameter));
  }

  /** Register a node as needing constant bangs. */
  void register_looped_node(Node *node);

  /** Remove a node from the 'constant bang' list. */
  void free_looped_node(Node *node);

  /** Remove all events related to a given node before the node dies. */
  void free_events_for(Node *node) {
    Event *e;
    ScopedLock lock(events_queue_);
    LinkedList<Event*> *it = events_queue_.begin();

    // find element
    while(it) {
      e = it->obj;
      if (e->uses_receiver(node)) {
        if (e->forced_) e->trigger();
        it = events_queue_.remove(e);
      } else
        it = it->next;
    }
  }

  /** @internal.
   * Testing only: run a single loop.
   */
  bool loop();

 public:
  /** Current time in [ms] since reference.
   */
  time_t current_time() {
    return time_ref_.elapsed();
  }

 //private:
  // FIXME: remove ?
  time_t current_time_;

 private:
  void init();

  /** Setup thread priority and start main loop.
   */
  void start_worker(Thread *thread);

  /** Main loop.
   */
  void run();

  /** Interrupt current sleep and force loop to run again.
   * This must be called from within a scoped lock.
   */
  void reload_queue();

  /** Signal handler to restart loop on event registration.
   */
  static void reload(int sig);

  /** Realtime related stuff. */
  /** Method executed when an Event is registering too fast.
   */
  void miss_event(const Event *event);

  /** Trigger events with a time older or equal to the current time. */
  void pop_events ();

  /** Empty events queue. */
  void pop_all_events ();

  /** Trigger loop events. These are typically the IO 'read/write' of the IO nodes. */
  void trigger_loop_events ();

  Root *root_;                              /**< Root tree. */

  /** Time reference. All times are [ms] from this reference.
   * It's the worker's birthdate !
   */
  TimeRef time_ref_;

  /** Ordered event list.
   */
  COrderedList<Event*>  events_queue_;

  /** Flag to mark if we need to reload the queue.
   */
  bool sleeping_;

  /** Only allow interrupts durring sleep.
   */
  Mutex sleep_lock_;

  /** List of methods to call on every loop.
   */
  std::deque<Node*>       looped_nodes_;
};

#endif // RUBYK_INCLUDE_RUBYK_WORKER_H_