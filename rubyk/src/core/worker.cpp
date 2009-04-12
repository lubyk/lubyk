#include "worker.h"
#include "node.h"


void Worker::register_looped_node(Node *node)
{
  free_looped_node(node);
  looped_nodes_.push_back(node);
}

void Worker::free_looped_node(Node *node)
{  
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
    while(!quit_) {
      struct timespec sleeper;
      sleeper.tv_sec  = 0; 
      sleeper.tv_nsec = WORKER_SLEEP_MS * 1000000;
  
      unlock(); // ok, others can do things while we sleep
        nanosleep(&sleeper, NULL); // FIXME: only if no loop events ?
      lock();
  
      current_time_ = real_time();
  
      // execute events that must occur on each loop (io operations)
      trigger_loop_events();
  
      // trigger events in the queue
      pop_events();
    }
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

void Worker::pop_all_events()
{
  Event * e;
  while( events_queue_.get(&e)) {
    current_time_ = e->when_;
    if (e->forced_) e->trigger();
    delete e;
    events_queue_.pop();
  }
}

void Worker::trigger_loop_events()
{
  std::deque<Node *>::iterator it;
  std::deque<Node *>::iterator end = looped_nodes_.end();
  for(it = looped_nodes_.begin(); it < end; it++) {
    (*it)->bang(gNilValue);
  }
}


