#ifndef _WORKER_H_
#define _WORKER_H_
/*
Structure of a running planet:

The Planet is an oscit::Root:
Root   <- Planet

It contains a single Worker that is passed to subnodes as context
Planet <>--- Worker
*/
#include "ordered_list.h"
#include "event.h"

#include "oscit/mutex.h"

#include <csignal>
#include <fstream>
#include <sys/timeb.h> // ftime
#include <queue>

// is 2 [ms] too long ? Testing needed.
// 0.01 = 10 [us] = 0.00001 [s] = 100'000 [Hz] = 100 [kHz]
#define WORKER_SLEEP_MS 0.01
#define ONE_SECOND 1000.0
#define ONE_MINUTE (60.0*ONE_SECOND)

class Worker : public Thread
{
public:
  Worker(Root *root) : root_(root) {
    ftime(&time_ref_); // set time reference to now (my birthdate).
    current_time_ = real_time();
  }
  
  virtual ~Worker() {
    kill();
  }
  
  /** Run until quit (through a command or signal). */
  void start() {
    this->Thread::start<Worker, &Worker::do_start>(this, NULL);
  }
  
  /** Set if the planet should be running (only used with direct loop control). */
  void should_run(bool should_run) {
    should_run_ = should_run;
  }
  
  Root *root() { return root_; }
  
  /** Get current real time in [ms] since reference. */
  time_t real_time() {
    struct timeb t;
    ftime(&t);
    return ((t.time - time_ref_.time) * 1000) + t.millitm - time_ref_.millitm;
  }
  
  /** Add an event to the event queue. The server is responsible for deleting the event. */
  void register_event(Event *event) {
    if (event->when_ < current_time_ + WORKER_SLEEP_MS) {
      miss_event(event);
    } else if (should_run_ || event->forced_) {
      events_queue_.push(event); // do not accept new events while we are trying to quit.
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
    Event * e;
    LinkedList<Event*> * it   = events_queue_.begin();

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
  
  /** Run a single step, returns false when quit loop should stop (quit).
   *  This method can be used if you want to handle the loop yourself.
   */
  inline bool loop() {
    struct timespec sleeper;
    sleeper.tv_sec  = 0; 
    sleeper.tv_nsec = WORKER_SLEEP_MS * 1000000; // 1'000'000

    unlock(); // ok, others can do things while we sleep
      nanosleep(&sleeper, NULL); // FIXME: only if no loop events ?
    lock();

    current_time_ = real_time();

    // execute events that must occur on each loop (io operations)
    trigger_loop_events();

    // trigger events in the queue
    pop_events();
    
    return should_run_;
  }
  
 public:
  /** Current logical time in [ms] since reference.
   */
  time_t current_time_;
  
 private:
  void init();
  
  /** Main loop. The call to do_run will hang until quit. */
  void do_start(Thread *thread);
  
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
  
  struct timeb time_ref_;                   /**< Time reference. All times are [ms] from this reference.
                                                 It's the worker's birthdate ! */
  
  /** Events ! */
  OrderedList<Event*>     events_queue_;    /**< Ordered event list. */
  std::deque<Node*>       looped_nodes_;    /**< List of methods to call on every loop. */
  
};

#endif // _WORKER_H_