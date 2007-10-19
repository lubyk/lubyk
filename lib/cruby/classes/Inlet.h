#ifndef _INLET_H_
#define _INLET_H_

#include "Slot.h"

class Inlet : public Slot {
public:  
  Inlet (void* node, void(*function)(void*node, float value)) : Slot(node), mFunction(function) {}
  
  /** Receive a value. */
  void receive (float value);
  
private:
  void (*mFunction)(void* node, float value);  /**< Function to be called when a new value is received. */
};

#endif