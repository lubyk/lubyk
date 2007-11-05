#ifndef _INLET_H_
#define _INLET_H_

#include "slot.h"
#include "midi_message.h"
#include "rubyk_signal.h"

class Inlet : public Slot {
public:  
  Inlet (void* node, void(*function)(void*node, const Signal& sig)) : Slot(node), mFunction(function) {}
  
  /** Receive a value. */
  void receive (const Signal& sig);
  
private:
  void (*mFunction)(void* node, const Signal& sig);  /**< Function to set a new value. */
};

#endif