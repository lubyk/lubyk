#ifndef _INLET_H_
#define _INLET_H_

#include "slot.h"
#include "midi_message.h"
#include "values.h"

class Inlet : public Slot {
public:  
  Inlet (void* node, void(*function)(void*node, const Value& sig)) : Slot(node), mFunction(function) {}
  
  /** Receive a value. */
  void receive (const Value& sig);
  
private:
  void (*mFunction)(void* node, const Value& sig);  /**< Function to set a new value. */
};

#endif