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
#define WORKER_SLEEP_MS 2.0
#define ONE_SECOND 1000.0
#define ONE_MINUTE (60.0*ONE_SECOND)

class Worker : public Thread
{
public:
  Worker(Root *root) : root_(root), quit_(false) {
    ftime(&time_ref_); // set time reference to now (my birthdate).
    current_time_ = real_time();
  }
  
  /** Run until quit (through a command or signal). */
  void run() {
    start_using_signals<Worker, &Worker::do_run>(this, NULL);
  }
  
  Root *root() { return root_; }
  
  /** Get current real time in [ms] since reference. */
  time_t real_time() {
    struct timeb t;
    ftime(&t);
    return ((t.time - time_ref_.time) * 1000) + t.millitm - time_ref_.millitm;
  }
  
  /** Add an event to the event queue. The server is responsible for deleting the event. */
  void register_event(Event *e) { 
    if (!quit_ || e->forced_) events_queue_.push(e); // do not accept new events while we are trying to quit.
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
  
  /** Received a SIGTERM. */
  void terminate(Thread *thread) {}
  
 public:
  time_t current_time_;                    /**< Current logical time in [ms] since reference. */
  
 private:
  void init();
  
  /** Main loop, returns false when quit loop should stop (quit). */
  void do_run(Thread *thread);
  
  /** Realtime related stuff. */
  
  /** Trigger events with a time older or equal to the current time. */
  void pop_events ();

  /** Empty events queue. */
  void pop_all_events ();

  /** Trigger loop events. These are typically the IO 'read/write' of the IO nodes. */
  void trigger_loop_events ();
  
  Thread thread_;                           /**< Manage running thread. */
  
  Root *root_;                              /**< Root tree. */
  
  struct timeb time_ref_;                   /**< Time reference. All times are [ms] from this reference.
                                                 It's the worker's birthdate ! */
  bool quit_;                               /**< Internal flag to tell running threads to quit. */


  /** Events ! */
  OrderedList<Event*>     events_queue_;    /**< Ordered event list. */
  std::deque<Node*>       looped_nodes_;    /**< List of methods to call on every loop. */
  
};

#endif // _WORKER_H_