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
  
  /** This method must be implemented in subclasses. It's the place where most
    * of the work should be done. This method is responsible for sending the signals out. */
  virtual void bang (const Value& val) = 0;
  
  /** Used to sort outlet connections. A node with a high trigger position receives the value before
    * another node with a small trigger position, if they are both connected to the same outlet. */ 
  inline real_t trigger_position() { return mTriggerPosition; }
private:
  
  real_t mTriggerPosition; /**< When sending signals from a particular slot, a node with a small mTriggerPosition 
                            *  will receive the signal after a node that has a greater mTriggerPosition. */
};



#endif // _NODE_H_