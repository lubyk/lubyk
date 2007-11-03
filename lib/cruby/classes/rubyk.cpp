#include "rubyk.h"
#include "class.h"

Rubyk::Rubyk() : mInstances(200), mQuit(false), mCurrentTime(0)
{
  ftime(&mTimeRef); // set time reference to now.
}

Rubyk::~Rubyk()
{
  std::vector<std::string>::iterator it;
  std::vector<std::string>::iterator end = mInstances.end();
  Node * node;
  
  for(it = mInstances.begin(); it < end; it++) {
    if (mInstances.get(&node, *it))
      delete node; // destroy node
  }
}

Node * Rubyk::create_instance (const std::string& pVariable, const std::string& pClass, const Params& p)
{
  Node * node = Class::create(this, pClass, p);
  Node * previous;
  std::string varName;

  if (node) {
    if (pVariable != "")
      node->set_name(pVariable);
    
    
    if (mInstances.get(&previous, node->name()))
      delete previous; // kill the node pointed by variable name
      
    mInstances.set(node->name(), node);
    create_pending_links();
  }
  return node;
}


void Rubyk::create_link(const std::string& pFrom, unsigned int pFromPort, unsigned int pToPort, const std::string& pTo)
{
  mPendingLinks.push_back(Link(pFrom,pFromPort,pToPort,pTo));
  create_pending_links();
}

// FIXME: on node deletion/replacement, remove all pending links related to this node ?.
void Rubyk::create_pending_links()
{
  std::list<Link>::iterator it,end;
  Node * fromNode, * toNode;
  Slot * fromPort, * toPort;
  end = mPendingLinks.end();
  it=mPendingLinks.begin();
  while(it != end) {
    if ((mInstances.get(&fromNode, it->mFrom)) && (mInstances.get(&toNode, it->mTo))) {
      if ((fromPort = fromNode->outlet(it->mFromPort)) && (toPort = toNode->inlet(it->mToPort))) {
        fromPort->connect(toPort);
        // remove from pending links
        it = mPendingLinks.erase(it);
      } else {
        // bad ports (we could remove from list...)
        it++;
      }
    } else {
      // instances not created yet
      it++;
    }
  }
}

bool Rubyk::get_instance(Node ** pResult, const std::string& pName)
{
  return mInstances.get(pResult, pName);
}

bool Rubyk::run()
{ 
  struct timespec sleeper;
  sleeper.tv_sec  = 0; 
  sleeper.tv_nsec = SLEEP_MS * 10000000;
  nanosleep (&sleeper, NULL);
  mCurrentTime = real_time();
  // execute events that must occur on each loop (io operations)
  //trigger_loop_events();
  // trigger events in the queue
  pop_events();
  return !mQuit;
}

void Rubyk::pop_events()
{
  BaseEvent * e;
  long double realTime = mCurrentTime;
  while(((e = mEventList.top()) != NULL) && realTime >= e->mTime) {
    mCurrentTime = e->mTime;
    e->trigger();
    delete e;
    mEventList.pop();
  }
  mCurrentTime = realTime;
}

void Rubyk::trigger_loop_events()
{
  
}