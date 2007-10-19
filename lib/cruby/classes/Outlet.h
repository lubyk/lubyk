#ifndef _OUTLET_H_
#define _OUTLET_H_

#include "Slot.h"

class Outlet : public Slot {
public:  
  Outlet (void* node, float(*function)(void*node)) : Slot(node), mFunction(function) {}
  
  /** Compute the value using the provided function and send the result to all connections. Do nothing if
    * there are no connections. */
  void compute_and_send ();
  
private:
  float (*mFunction)(void* node);  /**< Function to compute the new value. */
};

#endif
