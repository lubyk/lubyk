#ifndef _OUTLET_H_
#define _OUTLET_H_

#include "slot.h"
#include "rubyk_signal.h"

class Outlet : public Slot {
public:  
  Outlet (void* node) : Slot(node) {}
  
  /** Send the signal to all connections. Do nothing if there are no connections. */
  void send (const Signal& s);
};

#endif
