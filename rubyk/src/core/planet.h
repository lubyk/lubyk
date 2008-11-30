#ifndef _RUBYK_H_
#define _RUBYK_H_
#include "thash.h"
#include "ordered_list.h"
#include "link.h"
#include "mutex.h"
#include "group.h"
#include <list>
#include <queue>
#include <sys/timeb.h> // ftime
#include <iostream>

// is 2 [ms] too long ? Testing needed.
#define RUBYK_SLEEP_MS 2.0
#define ONE_SECOND 1000.0
#define ONE_MINUTE (60.0*ONE_SECOND)

class Value;
class Action;
class Command;

/////////////// GLWINDOW HACK ///////////
extern bool   gQuitGl; /**< Used to tell thread to exit. */
/////////////////////////////////////

class Planet : public Group
{
public:
  Planet();
  
  virtual ~Planet();
  
  Node * create_instance(const std::string& pVariable, const std::string& pClass, const Value& p, std::ostream * pOutput);
  
  void create_link(const std::string& pFrom, unsigned int pFromPort, unsigned int pToPort, const std::string& pTo);
  
  void remove_link(const std::string& pFrom, unsigned int pFromPort, unsigned int pToPort, const std::string& pTo);
  
  /** Get a pointer to the instance with the given name. */
  bool Planet::get_instance(Node ** pResult, const std::string& pName);
  
  /** Close the door and leave... */
  void quit()
  { mQuit = true; gQuitGl = true; }
  
  /** Main loop. Returns false when rubyk has to quit. */
  bool run();
  
  virtual void bang (const Value& sig)
  {}
  
  virtual bool set (const Value& p)
  { return true; }
  
  virtual void spy()
  { bprint(mSpy, mSpySize, "rubyk");  }
  
  /** Add an event to the event queue. The server is responsible for deleting the event. */
  void register_event(BaseEvent * e)
  { 
    if (!mQuit || e->mForced) mEventsQueue.push(e); // do not accept new events while we are trying to quit.
  }
  
  /** Create a new thread that will start by executing the BaseEvent. 
    * @return pthread_t of the new thread (NULL if it fails).
    */
  pthread_t create_thread(BaseEvent * e);
  
  /** Register a node as needing constant bangs. */
  void register_looped_node(Node * pNode);
  
  /** Remove a node from the 'constant bang' list. */
  void free_looped_node(Node * pNode);
  
  /** Remove all events related to a given node before the node dies. */
  void free_events_for(Node * pNode);

  /** Get current real time in [ms] since reference. */
  time_t real_time()
  {
    struct timeb t;
    ftime(&t);
    return ((t.time - mTimeRef.time) * 1000) + t.millitm - mTimeRef.millitm;
  }
  
  /** Starts new threads for commands. */
  void listen_to_command(Command& pCommand);
  
  /** Used by commands to get hold of the server to execute commands. */
  void lock()
  { mMutex.lock(); }

  /** Commands are finished. */
  void unlock()
  { mMutex.unlock(); }
  
  /** Set command thread to normal priority. */
  void normal_priority ();
  
  /** Passed as argument to thread creation (executed in new thread). */
  static void * start_thread(void * pEvent);
  
public:
  time_t mCurrentTime; /**< Current logical time in [ms] since reference. */
  
private:
  
  /** Set rubyk thread to high priority. */
  void high_priority ();
  
  /** Try to create links. */
  void create_pending_links ();
  
  /** Trigger events with a time older or equal to the current time. */
  void pop_events ();
  
  /** Empty events queue. */
  void pop_all_events ();
  
  /** Trigger loop events. These are typically the IO 'read/write' of the IO nodes. */
  void trigger_loop_events ();
  
  THash<std::string, Node *> mInstances; /**< Reference to the objects in the system (dictionary). */
  bool mQuit;                           /**< Internal flag to tell running threads to quit. */
  std::list<Link> mPendingLinks;        /**< List of pending connections waiting for variable assignements. */
  //std::list<Link> mLinksSet;
  
  /** Events ! */
  OrderedList<BaseEvent*> mEventsQueue; /**< Ordered event list. */
  std::deque<Node*>       mLoopedNodes; /**< List of nodes to call on every loop. */
  
  struct timeb mTimeRef;                /**< Time reference. All times are [ms] from this reference. */
  
  int mCommandSchedPoclicy;             /**< Thread's original scheduling priority (all commands get this). */ 
  struct sched_param mCommandThreadParam; /**< Scheduling parameters for commands (lower then rubyk). */
  
  Mutex mMutex;                         /**< "Do not mess with me" mutex lock. */
  std::queue<Command *> mCommands;      /**< Command line / editors. */
};

#endif