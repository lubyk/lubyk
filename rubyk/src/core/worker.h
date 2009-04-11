#ifndef _WORKER_H_
#define _WORKER_H_
/*
Structure of a running planet:

The Planet is an oscit::Root:
Root   <- Planet

It contains a single Worker that is passed to subnodes as context
Planet <>--- Worker
*/
#include "group.h"
#include "command.h"
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

class Worker : public Mutex
{
public:
  Worker(Root *root) : root_(root) {
    init();
  }
  
  /** Run until quit (through a command or signal). */
  void run() {
    lock(); // we get hold of the lock
      set_thread_this(this);
    
      signal(SIGTERM, Worker::term); // register a SIGTERM handler
      signal(SIGINT,  Worker::term);

      while (do_run());
    unlock();
  }
  
  Root *root() { return root_; }
  
  /** Create pending links (called on new object creation). */
  const Value create_pending_links ();
  
  /** Get current real time in [ms] since reference. */
  time_t real_time() {
    struct timeb t;
    ftime(&t);
    return ((t.time - time_ref_.time) * 1000) + t.millitm - time_ref_.millitm;
  }

  /** Set command thread to normal priority. */
  void normal_priority ();

  /** Main loop, returns false when quit loop should stop (quit). */
  bool do_run();

  /** Close the door and leave... */
  void quit() { 
    quit_ = true; //gQuitGl = true; 
  }

  /** Add an event to the event queue. The server is responsible for deleting the event. */
  void register_event(Event * e)
  { 
    if (!quit_ || e->forced_) events_queue_.push(e); // do not accept new events while we are trying to quit.
  }

  /** Remove all events related to a given node before the node dies. */
  void free_events_for(Node * node)
  {  
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

  /** Create a new thread that will run the method given as template parameter. Use NEW_THREAD(klass, method) if you prefer. */
  template <class T, void(T::*Tmethod)(void)>
  pthread_t new_thread(T * pReceiver) {
    Event * e = (Event*)new TEvent<T, Tmethod>(current_time_, pReceiver);
    return create_thread(e);
  }

  /** Create a new thread that will start by executing the BaseEvent. 
    * @return pthread_t of the new thread (NULL if it fails).
    */
  pthread_t create_thread(Event * e)
  {
    pthread_t id = NULL;
    if (!sThisKey) pthread_key_create(&sThisKey, NULL); // create a key to find 'this' object in new thread
    pthread_create( &id, NULL, &start_thread, (void*)e);
    return id;
  }

  /** Get "this" (used in static callbacks). */
  static void * thread_this()
  {
    return pthread_getspecific(sThisKey);
  }

  /** Set 'this' value for the current thread (used by Rubyk when starting a new thread). */
  static void set_thread_this(void * pObj)
  {
    pthread_setspecific(sThisKey, pObj);
  }

  /** Static method called in new thread. */
  static void * start_thread(void * pEvent)
  {
    // in new thread...

    Event * e = (Event*)pEvent;

    // Set receiver as "this" for new thread.
    set_thread_this(e->receiver());
    e->trigger();
    return NULL;
  }
  
 public:
  time_t current_time_;                    /**< Current logical time in [ms] since reference. */
  static pthread_key_t sThisKey;   /**< Key to retrieve 'this' value from a running thread. */
  
 private:
  void init();
  
  static void term(int sig) {
    ((Worker*)thread_this())->quit();
  }
  
  /** Realtime related stuff. */

  /** Set rubyk thread to high priority. */
  void high_priority ();
  
  /** Trigger events with a time older or equal to the current time. */
  void pop_events ();

  /** Empty events queue. */
  void pop_all_events ();

  /** Trigger loop events. These are typically the IO 'read/write' of the IO nodes. */
  void trigger_loop_events ();
  
  Root *root_;                              /**< Root tree. */
  
  int command_sched_policy_;                /**< Thread's original scheduling priority (all commands get this). */ 
  struct sched_param command_thread_param_; /**< Scheduling parameters for commands (lower then rubyk). */
  struct timeb time_ref_;                   /**< Time reference. All times are [ms] from this reference.
                                                 It's the worker's birthdate ! */
  bool quit_;                               /**< Internal flag to tell running threads to quit. */


  /** Events ! */
  OrderedList<Event*>     events_queue_;    /**< Ordered event list. */
  std::deque<Node*>       looped_nodes_;    /**< List of methods to call on every loop. */
  
};

#endif // _WORKER_H_