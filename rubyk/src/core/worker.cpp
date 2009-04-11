#include "worker.h"
#include "node.h"

bool Worker::init (const Value &p) {   
  // force build of "/class"
  ftime(&time_ref_); // set time reference to now (my birthdate).
  current_time_ = real_time();
  high_priority();
  lock();           // we get hold of everything, releasing resources when we decide (Worker is the master).
  quit_ = false;
  
  return true;
}


/** Called by commands and other low priority threads. */
void Worker::normal_priority()
{
  pthread_t id = pthread_self(); // this is a command thread

  if (pthread_setschedparam(id, command_sched_policy_, &command_thread_param_)) {
    fprintf(stderr, "Could not set command thread priority to %i.\n", command_thread_param_.sched_priority);
  }
}

/** Called during startup to increase thread priority. */
void Worker::high_priority()
{
  struct sched_param param;
  int policy;
  pthread_t id = pthread_self();

  // save original scheduling parameters
  pthread_getschedparam(id, &command_sched_policy_, &command_thread_param_);
  
  // set to high priority
  param = command_thread_param_;
  param.sched_priority = 47; // magick number for Mac OS X
  policy = SCHED_RR;         // round robin
  
  
  if (pthread_setschedparam(id, policy, &param)) {
    fprintf(stderr, "Could not set thread priority to %i.\n", param.sched_priority);
  }
}

bool Worker::do_run()
{ 
  struct timespec sleeper;
  sleeper.tv_sec  = 0; 
  sleeper.tv_nsec = WORKER_SLEEP_MS * 1000000;
  
  mMutex.unlock(); // ok, others can do things while we sleep
  nanosleep (&sleeper, NULL); // FIXME: only if no loop events ?
  mMutex.lock();
  
  current_time_ = real_time();
  
  // execute events that must occur on each loop (io operations)
  trigger_loop_events();
  
  // trigger events in the queue
  pop_events();
  return !quit_;
}

void Worker::pop_events()
{
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
    (*it)->bang(gBangValue);
  }
}


