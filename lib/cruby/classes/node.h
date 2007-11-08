#ifndef _NODE_H_
#define _NODE_H_

#include "inlet.h"
#include "outlet.h"
#include "params.h"
#include "hash.h"
#include "rubyk.h"
#include "rubyk_signal.h"
#include "event.h"

#include <sstream>
#include <cstdio>
#include <vector>
#include <string>


#define START_SPY_BUFFER     20
#define MAX_CLASS_NAME       50
#define START_INSPECT_BUFFER (START_SPY_BUFFER + MAX_CLASS_NAME + 5 + 16)

/** Nodes do the actual work.
  * They receive messages from their inlets and pass new values to their outlets.
  */  
class Node
{
public:
  Node() : mClass(NULL), mServer(NULL), mTriggerPosition(0), mId(0), mSpySize(0), mInspectSize(0), mSpy(NULL), mInspect(NULL), mOutput(&std::cout)
  { 
    char buf[50];
    sIdCounter++;
    sprintf(buf,"_%i",sIdCounter);
    mName = std::string(buf);  // default variable name is 'id'
  }
  
  virtual ~Node() ;
  
  bool init( const Params& p) { return true; }
  
  /** Add an inlet with the given callback (used by Class during instantiation). */
  void add_inlet(inlet_method_t pCallback)
  {
    Inlet * s = new Inlet(this,pCallback);
    s->setId(mInlets.size()); /* first inlet has id 0 */
    mInlets.push_back(s);
  }
  
  /** Add an outlet with the given callback (used by Class during instantiation). */
  void add_outlet(outlet_method_t pCallback)
  {
    Outlet * s = new Outlet(this,pCallback);
    s->setId(mOutlets.size()); /* first inlet has id 0 */
    mOutlets.push_back(s);
  }
  
  void set_server(Rubyk * pServer)
  { mServer = pServer; }
  
  void set_class(Class * pClass)
  { mClass = pClass; }
  
  void set_output(std::ostream * pOutput)
  { mOutput = pOutput; }
  
  std::ostream& output()
  { return *mOutput; }
  
  void execute_method (const std::string& pMethod, const Params& p);
  
  /** Compute new values for each outlet and send values through connections. */
  void bang (void)
  {
    int i = mOutlets.size() - 1;
    // do not try to compute outlets if the node is in broken mode
    if (!mIsOK) return;
    while(i >= 0) {
      /** Send through rightmost outlet first. */
      mOutlets[i--]->compute_and_send();
    }
  }
  
  /** This is the method that is called from the command line with 'v1.bang'. */
  void do_bang ()
  {
    bang();
    *mOutput << inspect() << std::endl;
  }
  
  /** Used by 'editors' to display some information on the node. Should be overwridden by subclasses. */
  const char* get_spy() {
    spy();
    if (mSpy)
      return mSpy;
    else
      return "--";
  }
  
  /** Display the node's class, id and current state using 'spy'. */
  const char * inspect ();
  
  /** Used to sort outlet connections. A node with a high trigger position receives the value before
    * another node with a small trigger position, if they are both connected to the same outlet. */ 
  inline float trigger_position() { return mTriggerPosition; }
  
  /** Return inlet at the given position. First inlet is '1', not '0'. */
  Inlet  * inlet  (int slot_id) 
  {   
    std::vector<Inlet*>::size_type sz = mInlets.size();
    if (slot_id < 1 || slot_id > sz) return NULL;
    return mInlets[slot_id - 1]; 
  }
  
  /** Return outlet at the given position. First outlet is '1', not '0'. */
  Outlet * outlet (int slot_id) 
  { 
    std::vector<Outlet*>::size_type sz = mOutlets.size();
    if (slot_id < 1 || slot_id > sz) return NULL;
    return mOutlets[slot_id - 1];
  }
  
  void set_is_ok (bool pStatus) 
  { mIsOK = pStatus; }
  
  Class * klass () 
  { return mClass; }
  
  void set_name (const char* pName) 
  { if (pName[0] != '\0') mName = pName; }
  
  void set_name (const std::string& pName) 
  { if (pName != "") mName = pName; }
  
  
  //const std::string& variable_name () { return mName; }
  const char * name () { return mName.c_str(); }
  
  const char * class_name() const;
  
protected:
  
  /** Used by 'editors' to display some information on the node. Should be overwridden by subclasses. */
  virtual void spy() {
    // do nothing
  }
  
  /** Display help message. */
  virtual void help() {
    // do nothing
    *mOutput << "Does nothing.\n";
  }
  
  /** Print message for spies. */
  void spy_print(const char *fmt, ...);
  
  /** Print message for spies. */
  void inspect_print(const char *fmt, ...);
  
  // time in [ms]
  void Node::bang_me_in (time_t pTime)
  {
    BaseEvent * e = (BaseEvent *) new CallEvent<Node, &Node::bang>(mServer->mCurrentTime + pTime, this);

    mServer->register_event( e );
  }
  
  template <class T, void(T::*Tmethod)(void *)>
  void register_event (time_t pTime, void * data)
  {
    BaseEvent * e = (BaseEvent *) new Event<T, Tmethod>(mServer->mCurrentTime + pTime, (T*)this, data);
    e->mForced = false;
    mServer->register_event( e );
  }
  
  /** Register an event that must be run when the server quits. */
  template <class T, void(T::*Tmethod)(void *)>
  void register_forced_event (time_t pTime, void * data)
  {
    BaseEvent * e = (BaseEvent *) new Event<T, Tmethod>(mServer->mCurrentTime + pTime, (T*)this, data);
    e->mForced = true;
    mServer->register_event( e );
  }
  
  // ================ MEMBER DATA    ================= //
  
  long  mId;
  bool  mIsOK; /**< If something bad arrived to the node during initialization or edit, the node goes into
                 *  broken mode and mIsOK becomes false. In 'broken' mode, the node does nothing. */
                 
  float mTriggerPosition; /**< When sending signals from a particular slot, a node with a small mTriggerPosition 
                            *  will receive the signal after a node that has a greater mTriggerPosition. */
  /** Host server. */
  Rubyk * mServer;
  Class * mClass;
  
  std::vector<Inlet*>  mInlets;
  std::vector<Outlet*> mOutlets;
  
  char * mSpy;       /**< Buffer used to transmit node status through 'spy'. */
  int  mSpySize;
  char * mInspect; /**< Buffer used to transmit 'inspect'. 16=ptr info, 5= characters in format. */
  int  mInspectSize;
  
  std::string mName; /**< Global identifier ('v1', 'x', 'm43') */
  
  std::ostream * mOutput; /**< Used to pass command result. */
  
private:
  
  static unsigned int sIdCounter;  /**< Each object has a unique id. */
};

#endif