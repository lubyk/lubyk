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
  thread->thread_ready();
  high_priority();
  while (loop())
    ;
}

void Worker::pop_events() {
  Event * e;
  time_t realTime = current_time_;
  while( events_queue_.get(&e) && realTime >= e->when_) {
    current_time_ = e->when_;
    e->trigger();
    delete e;
    events_queue_.pop();
  }
  current_time_ = realTime;
}

void Worker::pop_all_events() {
  Event * e;
  while( events_queue_.get(&e)) {
    current_time_ = e->when_;
    if (e->forced_) e->trigger();
    delete e;
    events_queue_.pop();
  }
}

void Worker::trigger_loop_events() {
  std::deque<Node *>::iterator it;
  std::deque<Node *>::iterator end = looped_nodes_.end();
  for(it = looped_nodes_.begin(); it < end; it++) {
    (*it)->bang(gNilValue);
  }
}


