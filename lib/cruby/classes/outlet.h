#ifndef _OUTLET_H_
#define _OUTLET_H_

#include "slot.h"
#include "signal.h"

class Outlet : public Slot {
public:  
  Outlet (void* node, void(*function)(void*node, Signal& sig)) : Slot(node), mFunction(function) {}
  
  /** Compute the value using the provided function and send the result to all connections. Do nothing if
    * there are no connections. */
  void compute_and_send ();
  
  void send(const Signal& s);
  
private:
  void(*mFunction)(void* node, Signal& sig);  /**< Function to get the new value. */
};

#endif
