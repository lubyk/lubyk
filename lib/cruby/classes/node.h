#ifndef _NODE_H_
#define _NODE_H_

#include "inlet.h"
#include "outlet.h"
#include "params.h"
#include "hash.h"
#include "signal.h"
#include <iostream>

class Rubyk;
class Class;

#define START_SPY_BUFFER     20
#define MAX_CLASS_NAME       50
#define START_INSPECT_BUFFER (START_SPY_BUFFER + MAX_CLASS_NAME + 5 + 16)

extern long double gLogicalTime;

/** Nodes do the actual work.
  * They receive messages from their inlets and pass new values to their outlets.
  */  
class Node
{
public:
  Node() : mClass(NULL), mServer(NULL), mTriggerPosition(0), mId(0), mSpySize(0), mInspectSize(0), mSpy(NULL), mInspect(NULL)
  { 
    char buf[50];
    sIdCounter++;
    sprintf(buf,"_%i",sIdCounter);
    mVariableName = std::string(buf);  // default variable name is 'id'
  }
  
  virtual ~Node() ;
  
  bool init( const Params& pParams) { return true; }
  
  void set_server(Rubyk * pServer)
  { mServer = pServer; }
  
  void set_class(Class * pClass)
  { mClass = pClass; }
  
  void execute_method (const std::string& pMethod, const Params& pParams, std::ostream * pOutput) ;

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
  Inlet  * inlet  (int slot_id) { return mInlets[slot_id - 1]; }
  
  /** Return outlet at the given position. First outlet is '1', not '0'. */
  Outlet * outlet (int slot_id) { return mOutlets[slot_id - 1]; }
  
  void set_is_ok (bool pStatus) 
  { mIsOK = pStatus; }
  
  Class * klass () 
  { return mClass; }
  
  void set_variable_name (const std::string& pName) 
  { mVariableName = pName; }
  
  //const std::string& variable_name () { return mVariableName; }
  const char * variable_name () { return mVariableName.c_str(); }
  
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
  
  // ================ INLET / OUTLET ================= //
  /** Create a new inlet with the method as callback to set incoming value. */
  template <class T, void(T::*Tmethod)(const Signal& sig)>
  void make_inlet ()
  {
    Inlet * s = new Inlet(this,&cast_inlet_method<T, Tmethod>);
    s->setId(mInlets.size()); /* first inlet has id 0 */
    mInlets.push_back(s);
  }
  
  /** Create a new outlet with the method as callback to set compute the new value. */
  template <class T, void(T::*Tmethod)(Signal& sig)>
  void make_outlet ()
  {
    Outlet * s = new Outlet(this, &cast_outlet_method<T, Tmethod>);
    s->setId(mOutlets.size()); /* first outlet has id 0 */
    mOutlets.push_back(s);
  }
  
  // time in [ms]
  void bang_me_in (long double pTime);
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
  
  std::string mVariableName; /**< Global identifier ('v1', 'x', 'm43') */
  
  std::ostream * mOutput; /**< Used to pass command result. */
  
private:
  
  static unsigned int sIdCounter;  /**< Each object has a unique id. */

  // ================ INLET / OUTLET ================= //
  /** Return a function pointer from a pointer to method. */
  template <class T, void(T::*Tmethod)(const Signal& sig)>
  static void cast_inlet_method (void * receiver, const Signal& sig)
  {
    (((T*)receiver)->*Tmethod)(sig);
  }
  
  /** Return a function pointer from a pointer to method. */
  template <class T, void(T::*Tmethod)(Signal& sig)>
  static void cast_outlet_method (void * receiver, Signal& sig)
  {
    (((T*)receiver)->*Tmethod)(sig);
  }
};

#endif