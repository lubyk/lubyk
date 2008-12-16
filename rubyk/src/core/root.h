#ifndef _ROOT_H_
#define _ROOT_H_
#include "object.h"
#include "call.h"
#include "mutex.h"
#include <sys/timeb.h> // ftime
#include <queue>
#include "ordered_list.h"
#include "event.h"

class ClassFinder;
class Command;
class Event;
class Node;

#define OBJECT_HASH_SIZE 10000
#define CLASS_ROOT "/class"
#define DEFAULT_OBJECTS_LIB_PATH "/usr/local/lib/rubyk"

// is 2 [ms] too long ? Testing needed.
#define RUBYK_SLEEP_MS 2.0
#define ONE_SECOND 1000.0
#define ONE_MINUTE (60.0*ONE_SECOND)


/** Call types. */
enum call_action_t {
  CallTrigger  = 1,
  CallInfo     = 2,
  CallInspect  = 4,
  CallNotFound = 8,
};

/** Root object. You can only start new trees with root objects. */
class Root : public Object
{
public:
  Root() : Object(""), mObjects(OBJECT_HASH_SIZE)
  {
    mRoot = this;
    mObjects.set(std::string(""), this);
    init();
  }
  
  Root(size_t pHashSize) : mObjects(pHashSize)
  {
    mRoot = this;
    mObjects.set(std::string(""), this);
    init();
  }
  
  virtual ~Root();
  
  void init();
  
  virtual void clear()
  {
    this->Object::clear();
    mPendingLinks.clear();
  }
  
  /** Execute the default operation for an object. */
  Value call (const char* pUrl)
  {
    return call(std::string(pUrl), gNilValue);
  }
  
  /** Execute the default operation for an object. */
  Value call (std::string& pUrl)
  {
    return call(pUrl, gNilValue);
  }
  
  /** Execute the default operation for an object. */
  Value call (const char* pUrl, const Value& val)
  {
    return call(std::string(pUrl), val);
  }
  
  /** Execute the default operation for an object. */
  Value call (const std::string& pUrl, const Value& val)
  {
    Object * target = NULL;
    size_t pos;
    std::string url = pUrl;
    call_action_t act;
    Value res;
    
    // FIXME: move the #info and #inspect into the method space ?
    
    if ( (pos = url.rfind("/#info")) != std::string::npos) {
      url = pUrl.substr(0, pos);
      act = CallInfo;
    } else if ( (pos = pUrl.rfind("/#inspect")) != std::string::npos) {
      url = pUrl.substr(0, pos);
      act = CallInspect;
    } else {
      act = CallTrigger;
    }
    
    while (!get(&target, url) && url != "") {
      act = CallNotFound;
      pos = url.rfind("/");
      if (pos == std::string::npos)
        url = "";
      else
        url = url.substr(0, pos);
      
    }
    
    if (!target) return not_found(pUrl, val);
    
    switch (act) {
      case CallTrigger:
        return target->trigger(val);
      case CallInfo:
        return String(target->mInfo);
      case CallInspect:
        return target->inspect(val);
      default:
        return target->not_found(pUrl, val);
    }
  }
  
  /** Notification of name/parent change from an object. */
  void register_object(Object * pObj)
  {
    // url/tree changed, make sure dict is in sync
    
    // 1. remove object
    if (pObj->mRoot) pObj->mRoot->unregister_object(pObj);
    
    // 2. add with new key
    mObjects.set(pObj->url(), pObj);
    
    // 4. this is the new root
    pObj->mRoot = this;
  }
  
  /** Unregister an object from tree. */
  void unregister_object(Object * pObj)
  {
    mObjects.remove_element(pObj);
  }
  
  /** Find a pointer to an Object from its url. Return false if the object is not found. */
  bool get (Object ** pResult, const std::string& pUrl)
  {
    return mObjects.get(pResult, pUrl);
  }
  
  /** Find a pointer to an Object from its url. Return false if the object is not found. */
  bool get (Object ** pResult, const char* pUrl)
  {
    return mObjects.get(pResult, std::string(pUrl));
  }
  
  /** Return a pointer to the object located at pUrl. NULL if not found. */
  Object * find(const std::string& pUrl)
  {
    Object * res = NULL;
    get(&res, pUrl);
    return res;
  }
  
  /** Return a pointer to the object located at pUrl. NULL if not found. */
  Object * find(const char * pUrl)
  {
    return find(std::string(pUrl));
  }
  
  /** Return a pointer to the object located at pUrl. NULL if not found. */
  Object * find(const String& pUrl)
  {
    return find(pUrl.string());
  }
  
  /** Return the class listing Object (create one if needed). */
  ClassFinder * classes();
  
  /** Create a new object from a class name. Calls "/class/ClassName/new". */
  const Value new_object(const char *       pUrl, const char *       pClass, const Value& pParams);
  
  /** Create a new object from a class name. Calls "/class/ClassName/new". */
  const Value new_object(const std::string& pUrl, const std::string& pClass, const Value& pParams);
  
  /** Create a new link between two slots. */
  const Value create_link(const std::string& pFrom, const std::string& pFromPort, const std::string& pToPort, const std::string& pTo);
  
  /** Remove a link between two slots. */
  const Value remove_link(const std::string& pFrom, const std::string& pFromPort, const std::string& pToPort, const std::string& pTo);
  
  /** Create pending links (called on new object creation). */
  const Value create_pending_links ();
  
  /** Runtime related methods. */
  
  
  /** Used by commands to get hold of the server to execute commands. */
  void lock()
  { mMutex.lock(); }

  /** Commands are finished. */
  void unlock()
  { mMutex.unlock(); }
  
  /** Get current real time in [ms] since reference. */
  time_t real_time()
  {
    struct timeb t;
    ftime(&t);
    return ((t.time - mTimeRef.time) * 1000) + t.millitm - mTimeRef.millitm;
  }
  
  /** Set command thread to normal priority. */
  void normal_priority ();
  
  /** Start listening to a command. */
  void listen_to_command (Command& pCommand);
  
  /** Main loop, returns false when quit loop should stop (quit). */
  bool run ();
  
  /** Close the door and leave... */
  void quit()
  { 
    mQuit = true; //gQuitGl = true; 
  }
  
  /** Add an event to the event queue. The server is responsible for deleting the event. */
  void register_event(Event * e)
  { 
    if (!mQuit || e->mForced) mEventsQueue.push(e); // do not accept new events while we are trying to quit.
  }
  
  /** Remove all events related to a given node before the node dies. */
  void free_events_for(Node * pNode)
  {  
    Event * e;
    LinkedList<Event*> * it   = mEventsQueue.begin();

    // find element
    while(it) {
      e = it->obj;
      if (e->uses_receiver(pNode)) {
        if (e->mForced) e->trigger();
        it = mEventsQueue.remove(e);
      } else
        it = it->next;
    }
  }
  
public:
  time_t mCurrentTime;                    /**< Current logical time in [ms] since reference. */
  
protected:
  THash<std::string, Object*> mObjects;   /**< Hash to find any object in the tree from its url. */
  
private:
  std::list<Call> mPendingLinks;        /**< List of pending connections waiting for variable assignements. */
  
  /** Realtime related stuff. */
  
  /** Set rubyk thread to high priority. */
  void high_priority ();
    
  /** Trigger events with a time older or equal to the current time. */
  void pop_events ();
  
  /** Empty events queue. */
  void pop_all_events ();
  
  /** Trigger loop events. These are typically the IO 'read/write' of the IO nodes. */
  void trigger_loop_events ();
  
  Mutex mMutex;                           /**< "Do not mess with me" mutex lock. */
  int mCommandSchedPoclicy;               /**< Thread's original scheduling priority (all commands get this). */ 
  struct sched_param mCommandThreadParam; /**< Scheduling parameters for commands (lower then rubyk). */
  struct timeb mTimeRef;                  /**< Time reference. All times are [ms] from this reference. It's the root's birthdate ! */
  bool mQuit;                             /**< Internal flag to tell running threads to quit. */
  
  std::queue<Command *> mCommands;        /**< Command line / editors. */
  
  /** Events ! */
  OrderedList<Event*>     mEventsQueue;   /**< Ordered event list. */
  std::deque<Node*>       mLoopedNodes;   /**< List of methods to call on every loop. */
};

#endif // _ROOT_H_