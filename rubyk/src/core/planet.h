#ifndef _PLANET_H_
#define _PLANET_H_
#include "group.h"
#include "command.h"
#include <csignal>
#include <fstream>

#include "mutex.h"
#include <sys/timeb.h> // ftime
#include <queue>
#include "ordered_list.h"
#include "event.h"

// oscit
#include "oscit/root.h"

class ClassFinder;

#define CLASS_ROOT "/class"
#define DEFAULT_OBJECTS_LIB_PATH "/usr/local/lib/rubyk"

// is 2 [ms] too long ? Testing needed.
#define RUBYK_SLEEP_MS 2.0
#define ONE_SECOND 1000.0
#define ONE_MINUTE (60.0*ONE_SECOND)


class Planet : public Group
{
public:
  Planet() : mRoot(this)
  {
    init(gNilValue);
  }
  
  Planet(uint pPort) : mRoot(this)
  {
    init(gNilValue);
    mRoot.open_port(pPort);
  }
  
  Planet(int argc, char * argv[]) : mRoot(this)
  {
    // TODO: get port from command line
    init(gNilValue);
    
    if (argc > 1) {
      std::ifstream in(argv[1], std::ios::in);
      std::ostringstream oss;
      oss << in.rdbuf();
      in.close();

      Command  * fCmd;
      fCmd = new Command(std::cin, std::cout); // we use new because cleanup code is not executed for this thread due to opengl glutMainLoop
      fCmd->set_planet(this);
      fCmd->set_silent(true);
      unlock(); // so the commands are directly processed
        oss << "\n";
        fCmd->parse(oss.str());
      lock();

      fCmd->close();
      delete fCmd;
    }
  }
  
  virtual ~Planet ();
  
  virtual bool init (const Value& p);
  
  virtual void clear()
  {
    mRoot.clear();
    mPendingLinks.clear();
  }
  
  void open_port(uint pPort)
  {
    mRoot.open_port(pPort);
  }
  
  /** ======== META ========= */
  
  virtual const Value inspect(const Value& val)
  { 
    return String("Planet");
  }
  
  virtual void bang(const Value& val) {}
  
  /** Run until quit (through a command or signal). */
  void run()
  {
    set_thread_this(this);
    
    signal(SIGTERM, Planet::term); // register a SIGTERM handler
    signal(SIGINT,  Planet::term);

    while (do_run());
  }
  
  oscit::Root * root()
  {
    return &mRoot;
  }
  
  /** Create pending links (called on new object creation). */
  const Value create_pending_links ();
  
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
  bool do_run ();

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

  /** Create a new thread that will run the method given as template parameter. Use NEW_THREAD(klass, method) if you prefer. */
  template <class T, void(T::*Tmethod)(void)>
  pthread_t new_thread (T * pReceiver)
  {
    Event * e = (Event*)new TCallEvent<T, Tmethod>(mCurrentTime, pReceiver);
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
  
  /** Return the class finder (create one if needed). */
  ClassFinder * classes();
  
  /** Create a new object from a class name. Calls "/class/ClassName/new". */
  const Value new_object(const char *       pUrl, const char *       pClass, const Value& pParams);

  /** Create a new object from a class name. Calls "/class/ClassName/new". */
  const Value new_object(const std::string& pUrl, const std::string& pClass, const Value& pParams);

  /** Create a new link between two slots. */
  const Value create_link(const std::string& pFrom, const std::string& pFromPort, const std::string& pToPort, const std::string& pTo);

  /** Remove a link between two slots. */
  const Value remove_link(const std::string& pFrom, const std::string& pFromPort, const std::string& pToPort, const std::string& pTo);
  
  
  /** =========  PROXY METHODS ========= (used by commands) */
  
  /** Execute the default operation for an object. */
  Value call (const char* pUrl)
  {
    return mRoot.call(std::string(pUrl), gNilValue);
  }

  /** Execute the default operation for an object. */
  Value call (std::string& pUrl)
  {
    return mRoot.call(pUrl, gNilValue);
  }

  /** Execute the default operation for an object. */
  Value call (const char* pUrl, const Value& val)
  {
    return mRoot.call(std::string(pUrl), val);
  }

  /** Execute the default operation for an object. */
  Value call (const std::string& pUrl, const Value& val)
  {
    return mRoot.call(pUrl, val);
  }
  
  /** Return a pointer to the object located at pUrl. NULL if not found. */
  oscit::Object * find(const std::string& pUrl)
  {
    return mRoot.find(pUrl);
  }

  /** Return a pointer to the object located at pUrl. NULL if not found. */
  oscit::Object * find(const char * pUrl)
  {
    return mRoot.find(std::string(pUrl));
  }

  /** Return a pointer to the object located at pUrl. NULL if not found. */
  oscit::Object * find(const String& pUrl)
  {
    return mRoot.find(pUrl.string());
  }
  
public:
  std::list<oscit::Call> mPendingLinks;        /**< List of pending connections waiting for variable assignements. */
  time_t mCurrentTime;                    /**< Current logical time in [ms] since reference. */
  static pthread_key_t sThisKey;   /**< Key to retrieve 'this' value from a running thread. */
  
protected:
  /** Actual adoption. Adopt objects in the new namespace (branch). */
  virtual void do_adopt(Object * pObject)
  {
    mRoot.do_adopt(pObject);
  }
  
private:
  static void term(int sig)
  {
    ((Planet*)thread_this())->quit();
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
  
  oscit::Root  mRoot;                     /**< Processing tree. */
  
  Mutex mMutex;                           /**< "Do not mess with me" mutex lock. */
  int mCommandSchedPoclicy;               /**< Thread's original scheduling priority (all commands get this). */ 
  struct sched_param mCommandThreadParam; /**< Scheduling parameters for commands (lower then rubyk). */
  struct timeb mTimeRef;                  /**< Time reference. All times are [ms] from this reference. It's the root's birthdate ! */
  bool mQuit;                             /**< Internal flag to tell running threads to quit. */

  std::queue<Command *>   mCommands;      /**< Command line / editors. FIXME: it seems these should live in oscit space ... (not sure). */

  /** Events ! */
  OrderedList<Event*>     mEventsQueue;   /**< Ordered event list. */
  std::deque<Node*>       mLoopedNodes;   /**< List of methods to call on every loop. */
  
};

#endif // _PLANET_H_