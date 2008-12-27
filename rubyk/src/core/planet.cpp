#include "planet.h"
#include "class_finder.h"
#include "node.h"
#include "command.h"

bool Planet::init (const Value& p)
{   
  // force build of "/class"
  mPlanet = this;
  classes();
  ftime(&mTimeRef); // set time reference to now (my birthdate).
  mCurrentTime = real_time();
  high_priority();
  lock();           // we get hold of everything, releasing resources when we decide (Planet is the master).
  mQuit = false;
  
  return true;
}

Planet::~Planet ()
{
  Command * child;

  while(!mCommands.empty()) {
    child = mCommands.front();
    // join
    child->close(); // joins pthread
    mCommands.pop();
  }
  clear();
}

/** Return the class listing Object (create one if needed). */
ClassFinder * Planet::classes()
{
  oscit::Object * obj = find(CLASS_ROOT);
  if (!obj) {
    // create ClassFinder
    ClassFinder * cf = mRoot.adopt(new ClassFinder(std::string(CLASS_ROOT).substr(1, std::string::npos), DEFAULT_OBJECTS_LIB_PATH));
    return cf;
  }
  return TYPE_CAST(ClassFinder, obj);
}


/** Create a new object from a class name. Calls "/class/ClassName/new". */
const Value Planet::new_object(const char * pUrl, const char * pClass, const Value& pParams)
{
  Hash h;
  h.set_key("url", String(pUrl));
  h.set_key("params", pParams);
  Value res = call(std::string(classes()->url()).append("/").append(pClass).append("/new"), h);
  return res;
}

/** Create a new object from a class name. Calls "/class/ClassName/new". */
const Value Planet::new_object(const std::string& pUrl, const std::string& pClass, const Value& pParams)
{  
  Hash h;
  h.set_key("url", String(pUrl));
  h.set_key("params", pParams);
  Value res = call(std::string(classes()->url()).append("/").append(pClass).append("/new"), h);
  return res;
}

const Value Planet::create_link(const std::string& pFrom, const std::string& pFromPort, const std::string& pToPort, const std::string& pTo)
{
  //std::cout << "pending " << pFrom << "("<< pFromPort << ":" << pFromPort.length() << ")" << " --> " << pTo << "("<< pToPort << ")" << std::endl;
  
  std::string url(pFrom);
  if (pFromPort != "")
    url.append("/out/").append(pFromPort).append("/link");
  else
    url.append("/out/link"); // link from first outlet
    
  String param(pTo);
  
  if (pToPort != "")
    param.append("/in/").append(pToPort);
  else
    param.append("/in"); // link to first inlet
  
  // try to create link
  Value res = call(url, param);
  if (res.is_error()) mPendingLinks.push_back(oscit::Call(url,param));
  return res;
}

const Value Planet::remove_link(const std::string& pFrom, const std::string& pFromPort, const std::string& pToPort, const std::string& pTo)
{ 
  
  std::string url(pFrom);
  if (pFromPort != "")
    url.append("/out/").append(pFromPort).append("/unlink");
  else
    url.append("/out/unlink"); // unlink first outlet
    
  String param(pTo);
  
  if (pToPort != "")
    param.append("/in/").append(pToPort);
  else
    param.append("/in"); // unlink first inlet
    
  // try to remove link
  Value res = call(url, param);
  return res;
}

// FIXME: on node deletion/replacement, remove/move all pending links related to this node ?.
const Value Planet::create_pending_links()
{
  std::list<oscit::Call>::iterator it,end;
  end = mPendingLinks.end();
  it  = mPendingLinks.begin();
  Value res;
  
  while(it != end) {
    //std::cout << "PENDING " << it->mUrl << " => " << it->mParam << std::endl;
    res = call(it->mUrl, it->mParam);
    if (res.is_string()) {
      it = mPendingLinks.erase(it);  // call succeeded
    } else
      it++;
  }
  // return list of created links ?
  return gNilValue;
}


/** Called by commands and other low priority threads. */
void Planet::normal_priority()
{
  pthread_t id = pthread_self(); // this is a command thread

  if (pthread_setschedparam(id, mCommandSchedPoclicy, &mCommandThreadParam)) {
    fprintf(stderr, "Could not set command thread priority to %i.\n", mCommandThreadParam.sched_priority);
  }
}

/** Called during startup to increase thread priority. */
void Planet::high_priority()
{
  struct sched_param param;
  int policy;
  pthread_t id = pthread_self();

  // save original scheduling parameters
  pthread_getschedparam(id, &mCommandSchedPoclicy, &mCommandThreadParam);
  
  // set to high priority
  param = mCommandThreadParam;
  param.sched_priority = 47; // magick number for Mac OS X
  policy = SCHED_RR;         // round robin
  
  
  if (pthread_setschedparam(id, policy, &param)) {
    fprintf(stderr, "Could not set thread priority to %i.\n", param.sched_priority);
  }
}


/** Start listening to a command. */
void Planet::listen_to_command (Command& pCommand)
{
  int ret;
  pthread_t id;
  pCommand.set_planet(this);

  ret = pthread_create( &id, NULL, &Command::call_do_listen, &pCommand);
  pCommand.set_thread_id(id);
  // FIXME: check for error from 'ret'

  mCommands.push(&pCommand);
}

bool Planet::do_run()
{ 
  struct timespec sleeper;
  sleeper.tv_sec  = 0; 
  sleeper.tv_nsec = RUBYK_SLEEP_MS * 1000000;
  
  mMutex.unlock(); // ok, others can do things while we sleep
  nanosleep (&sleeper, NULL); // FIXME: only if no loop events ?
  mMutex.lock();
  
  mCurrentTime = real_time();
  
  // execute events that must occur on each loop (io operations)
  trigger_loop_events();
  
  // trigger events in the queue
  pop_events();
  return !mQuit;
}

void Planet::pop_events()
{
  Event * e;
  time_t realTime = mCurrentTime;
  while( mEventsQueue.get(&e) && realTime >= e->mTime) {
    mCurrentTime = e->mTime;
    e->trigger();
    delete e;
    mEventsQueue.pop();
  }
  mCurrentTime = realTime;
}

void Planet::pop_all_events()
{
  Event * e;
  while( mEventsQueue.get(&e)) {
    mCurrentTime = e->mTime;
    if (e->mForced) e->trigger();
    delete e;
    mEventsQueue.pop();
  }
}

void Planet::trigger_loop_events()
{
  std::deque<Node *>::iterator it;
  std::deque<Node *>::iterator end = mLoopedNodes.end();
  for(it = mLoopedNodes.begin(); it < end; it++) {
    (*it)->bang(gBangValue);
  }
}


