#include "rubyk.h"
#include "node.h"
#include "command.h"
#include "class.h"
#include <pthread.h>


Signal gNilSignal(NilSignal);   // globals defined declared in rubyk_signal.h
Signal gBangSignal(BangSignal); // globals defined declared in rubyk_signal.h

Rubyk::Rubyk() : mInstances(200), mQuit(false), mCurrentTime(0), mCanRegister(true)
{
  mMutex.lock();    // we get hold of everything, releasing resources when we decide (I'm the master).
  ftime(&mTimeRef); // set time reference to now (my birthdate).
  
  high_priority();
}

Rubyk::~Rubyk()
{
  std::vector<std::string>::iterator it;
  std::vector<std::string>::iterator end = mInstances.end();
  Node * node;
  Command * child;  
  
  while(!mCommands.empty()) {
    child = mCommands.front();
    // join
    child->close(); // joins pthread
    mCommands.pop();
  }
  
  pop_all_events();
  
  for(it = mInstances.begin(); it < end; it++) {
    if (mInstances.get(&node, *it)) {
      delete node; // destroy node
    }
  }
}

/** Called during startup to increase thread priority. */
void Rubyk::high_priority()
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

void Rubyk::normal_priority()
{
  pthread_t id = pthread_self(); // this is a command thread

  if (pthread_setschedparam(id, mCommandSchedPoclicy, &mCommandThreadParam)) {
    fprintf(stderr, "Could not set command thread priority to %i.\n", mCommandThreadParam.sched_priority);
  }
}

void Rubyk::listen_to_command (Command& pCommand)
{
  int ret;
  pthread_t id;
  pCommand.set_server(*this);
  
  ret = pthread_create( &id, NULL, &Command::call_do_listen, &pCommand);
  pCommand.set_thread_id(id);
  // FIXME: check for error from 'ret'
  
  mCommands.push(&pCommand);
}



Node * Rubyk::create_instance (const std::string& pVariable, const std::string& pClass, const Params& p, std::ostream * pOutput)
{
  Node * node = Class::create(this, pVariable, pClass, p, pOutput);
  Node * previous;

  if (node) {
    if (mInstances.get(&previous, node->name())) {
      delete previous; // kill the node pointed by variable name 
    }
      
    mInstances.set(node->name(), node);
    create_pending_links();
  }
  return node;
}


void Rubyk::create_link(const std::string& pFrom, unsigned int pFromPort, unsigned int pToPort, const std::string& pTo)
{
  //std::cout << "pending " << pFrom << "("<< pFromPort << ")" << " --> " << pTo << "("<< pToPort << ")" << std::endl;
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
        //std::cout << "link " << fromNode->name() << "("<< fromPort->mId+1 << ")" << " --> " << toNode->name() << "("<< toPort->mId+1 << ")" << std::endl;
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
  sleeper.tv_nsec = SLEEP_MICRO_S * 10000;
  
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

void Rubyk::pop_events()
{
  BaseEvent * e;
  long double realTime = mCurrentTime;
  while( mEventsQueue.get(&e) && realTime >= e->mTime) {
    mCurrentTime = e->mTime;
    e->mIsBang ? ((Node*)e->mNode)->bang(gNilSignal) : e->trigger();
    delete e;
    mEventsQueue.pop();
  }
  mCurrentTime = realTime;
}

void Rubyk::pop_all_events()
{
  BaseEvent * e;
  while( mEventsQueue.get(&e)) {
    mCurrentTime = e->mTime;
    if (e->mForced)
      e->mIsBang ? ((Node*)e->mNode)->bang(gNilSignal) : e->trigger();
    delete e;
    mEventsQueue.pop();
  }
}

void Rubyk::register_looped_node(Node * pNode)
{
  free_looped_node(pNode);
  mLoopedNodes.push_back(pNode);
}

void Rubyk::trigger_loop_events()
{
  std::deque<Node *>::iterator it;
  std::deque<Node *>::iterator end = mLoopedNodes.end();
  for(it = mLoopedNodes.begin(); it < end; it++) {
    (*it)->bang(gNilSignal);
  }
}

void Rubyk::free_looped_node(Node * pNode)
{  
  std::deque<Node*>::iterator it;
  std::deque<Node*>::iterator end = mLoopedNodes.end();
  for(it = mLoopedNodes.begin(); it < end; it++) {
    if (*it == pNode) {
      mLoopedNodes.erase(it);
      break;
    }
  }
}

void Rubyk::free_events_for(Node * pNode)
{  
  BaseEvent * e;
  LinkedList<BaseEvent*> * it   = mEventsQueue.begin();
  LinkedList<BaseEvent*> * prev = NULL;
  
  // find element
  while(it) {
    e = it->obj;
    if (e->uses_node(pNode)) {
      if (e->mForced)
        e->mIsBang ? ((Node*)e->mNode)->bang(gNilSignal) : e->trigger();
      it = mEventsQueue.remove(e);
    } else
      it = it->next;
  }
}