#ifndef OSCIT_INCLUDE_OSCIT_MIDI_VALUE_H_
#define OSCIT_INCLUDE_OSCIT_MIDI_VALUE_H_
#include "oscit/value.h"
#include "oscit/matrix.h"

namespace oscit {

/** Value is the base type of all data transmitted between objects or used as parameters
and return values for osc messages.*/
class MidiValue : public Value
{ 
 public: 
  explicit MidiValue() {
    set_type(MIDI_VALUE);
  }
  explicit MidiValue(size_t rows, size_t cols) : Value(Matrix(rows,cols)) {}
  explicit MidiValue(const MidiMessage &msg) : Value(msg) {}
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_MIDI_VALUE_H_
