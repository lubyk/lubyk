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



class Node : public Object
{
public:
  Node() {}
  
  virtual ~Node() {}
  
  /** This method must be implemented in subclasses. It is used to do a basic setup with default parameters before these
    * are changed during runtime. */
  virtual bool init()
  { return true; }
  
  /** Shortcut to call multiple methods on an object.
    * Using "obj.set(foo:4 bar:5)" is equivalent to calling "obj.foo(4)" and "obj.bar(5)". */
  bool set(const Hash& pParams)
  {
    Hash p(pParams);
    Object * obj;
    Hash_iterator it;
    Hash_iterator end = p.end();
    
    for(it = p.begin(); it != end; it++) {
      if ( (obj = child(*it)) ) {
        obj->trigger(p[*it]);
      }
    }
    return true;
  }
  
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
private:
  
  bool  mIsOK;     /**< If something bad arrived to the node during initialization or edit, the node goes into
                     *  broken state and mIsOK becomes false. In 'broken' mode, the node does nothing. */
                     
  real_t mTriggerPosition; /**< When sending signals from a particular slot, a node with a small mTriggerPosition 
                            *  will receive the signal after a node that has a greater mTriggerPosition. */
  String mClassUrl;        /**< Url for the node's class. */
};



#endif // _NODE_H_