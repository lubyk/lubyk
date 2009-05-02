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

void Worker::do_run(Thread *thread) {
  high_priority();
  lock();
    while (loop())
      ;
  unlock();
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


