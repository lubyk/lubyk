#ifndef _NODE_H_
#define _NODE_H_
#include "object.h"
#include "event.h"
#include "inlet.h"
#include "outlet.h"
#include "thash.h"
#include "values.h"

#include <sstream>
#include <cstdio>
#include <vector>
#include <string>
#include <list>
#include <pthread.h>

class Observer;

class Node : public Object
{
public:
  Node() 
  {
    // create a key to find 'this' object in a new thread
    if (!Node::sThisKey) pthread_key_create(&Node::sThisKey, NULL);
  }
  
  virtual ~Node();
  
  /** Class signature. */
  virtual uint type()
  {
    return H("Node");
  }
  
  /** Add an inlet with the given callback (used by Class during instantiation). */
  void register_inlet(Inlet * pInlet)
  {
    pInlet->set_id(mInlets.size()); /* first inlet has id 0 */
    mInlets.push_back(pInlet);
  }
  
  /** Add an outlet. (used by Class during instantiation).
    * Name not used for the moment. */
  void register_outlet(Outlet * pOutlet)
  {
    pOutlet->set_id(mOutlets.size()); /* first inlet has id 0 */
    mOutlets.push_back(pOutlet);
  }
  
  /** Remove inlet from mInlets list of callbacks. */
  void unregister_inlet(Inlet * pInlet)
  {
    std::vector<Inlet*>::iterator it;
    std::vector<Inlet*>::iterator end = mInlets.end();

    for(it = mInlets.begin(); it != end; it++) {
      if (*it == pInlet) {
        mInlets.erase(it);
        break;
      }
    }
  }
  
  /** Remove outlet from mOutlets list of callbacks. */
  void unregister_outlet(Outlet * pOutlet)
  {
    std::vector<Outlet*>::iterator it;
    std::vector<Outlet*>::iterator end = mOutlets.end();
    
    for(it = mOutlets.begin(); it != end; it++) {
      if (*it == pOutlet) {
        mOutlets.erase(it);
        break;
      }
    }
  }
  
  /** Add an observer that will be notified of node changes. */
  void register_observer(Observer * pObs)
  {
    mObservers.push_back(pObs);
  }
  
  /** Stop notifying an observer of node changes. */
  void unregister_observer(Observer * pObs)
  {
    std::list<Observer*>::iterator end = mObservers.end();
    for(std::list<Observer*>::iterator it = mObservers.begin(); it != end; it++) {
      if (*it == pObs) it = mObservers.erase(it);
    }
  }
  
  /** This method must be implemented in subclasses. It is used to do a basic setup with default parameters before these
    * are changed during runtime. */
  virtual bool init()
  { return true; }
  
  /** Set mIsOK flag. If this flag is not true, the node is considered "broken" and usually does not do any processing. */
  void set_is_ok (bool pStatus) 
  { mIsOK = pStatus; }
  
  /** This method must be implemented in subclasses. It's the place where most
    * of the work should be done. This method is responsible for sending the signals out. */
  virtual void bang (const Value& val) = 0;
  
  /** Set url for class. TODO: Maybe we should pass a pointer to the class in case it moves ? But then if it is removed ? */
  void set_class_url(const std::string& pClass)
  { mClassUrl = pClass; }
  
  /** Used to sort outlet connections. A node with a high trigger position receives the value before
    * another node with a small trigger position, if they are both connected to the same outlet. */ 
  inline real_t trigger_position() { return mTriggerPosition; }
  
  /** Send a Value out of the first outlet. */
  inline void send(const Value& val)
  { send(1, val); }
  
  /** Send a Value out of an outlet. */
  inline void send (size_t pPort, const Value& val)
  { 
//FIX    if (mDebug) *mOutput << "[" << mName << ":" << pPort << "] " << val << std::endl;
    if (pPort < 1 || pPort > mOutlets.size() || val.is_nil()) return;
    mOutlets[pPort - 1]->send(val);
  }
  
  /** Notify observers of a change. */
  void notify(uint key, const Value& pValue);

public:
  static pthread_key_t sThisKey;   /**< Key to retrieve 'this' value from a running thread. */
  
private:
  
  bool  mIsOK;     /**< If something bad arrived to the node during initialization or edit, the node goes into
                     *  broken state and mIsOK becomes false. In 'broken' mode, the node does nothing. */
                     
  real_t mTriggerPosition; /**< When sending signals from a particular slot, a node with a small mTriggerPosition 
                            *  will receive the signal after a node that has a greater mTriggerPosition. */
  String mClassUrl;        /**< Url for the node's class. */
  
  std::vector<Inlet*>  mInlets;    /**< List of inlets. FIXME: is this used ? */
  std::vector<Outlet*> mOutlets;   /**< List of outlets. */
  std::list<Observer*> mObservers; /**< Observers to notify of node changes (observing satellites/commands). */

};

#endif // _NODE_H_