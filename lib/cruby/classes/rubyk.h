#ifndef _RUBYK_H_
#define _RUBYK_H_
#include "hash.h"
#include "ordered_list.h"
#include "link.h"
#include "event.h"
#include "mutex.h"
#include <list>
#include <queue>
#include <sys/timeb.h> // ftime
#include <iostream>

// 50 ms wait
#define SLEEP_MS 1

class Params;
class Action;
class Node;
class Command;

class Rubyk
{
public:
  Rubyk();
  
  virtual ~Rubyk();
  
  Node * create_instance(const std::string& pVariable, const std::string& pClass, const Params& p, std::ostream * pOutput);
  
  void create_link(const std::string& pFrom, unsigned int pFromPort, unsigned int pToPort, const std::string& pTo);
  
  /** Get a pointer to the instance with the given name. */
  bool Rubyk::get_instance(Node ** pResult, const std::string& pName);
  
  /** Close the door and leave... */
  void quit()
  { mQuit = true; }
  
  /** Main loop. Returns false when rubyk has to quit. */
  bool run();
  
  /** Add an event to the event queue. The server is responsible for deleting the event. */
  void register_event(BaseEvent * pEvent)
  { mEventsQueue.push(pEvent); }
  
  time_t mCurrentTime; /**< Current logical time in [ms] since reference. */

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
  
private:
  
  /** Try to create links. */
  void create_pending_links ();
  
  /** Trigger events with a time older or equal to the current time. */
  void pop_events ();
  
  /** Trigger loop events. These are typically the IO 'read/write' of the IO nodes. */
  void trigger_loop_events ();
  
  bool mQuit;
  
  /** Semaphore for commands registration. */
  bool mCanRegister;
  
  /** Reference to the objects in the system. */
  Hash<std::string, Node *> mInstances;
  
  /** List of pending connections waiting for variable assignements. */
  std::list<Link> mPendingLinks;
  //std::list<Link> mLinksSet;
  
  /** Events ! */
  OrderedList<BaseEvent*> mEventsQueue; /**< Ordered event list. */
  
  /** Time reference. All times are [ms] from this reference. */
  struct timeb mTimeRef;
  
  /** Do not mess with me mutex lock. */
  Mutex mMutex;
  
  /** Commands. */
  std::queue<Command *> mCommands;
};

#endif