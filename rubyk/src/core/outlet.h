#ifndef _OUTLET_H_
#define _OUTLET_H_

#include "slot.h"
#include "values.h"

class Outlet : public Slot {
public:  
  Outlet (void* node) : Slot(node) {}
  Outlet (void* node, uint pType) : Slot(node, pType) {}
  
  /** Send the signal to all connections. Do nothing if there are no connections. */
  void send (const Value& s);
};

#endif
