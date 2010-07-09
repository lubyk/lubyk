/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#include "rubyk.h"
using namespace rk;

class NoteOut : public Node {
 public:
  NoteOut() {
    midi_note_.set_as_note(MIDI_NOTE_C0 + 36); // C3
  }

  // [1] set note, send current note
  const Value note(const Value &val) {
    if (val.is_real()) {
      midi_note_.set_note(val.r);
      send(Value(midi_note_));
    } else if (val.is_bang()) {
      send(Value(midi_note_));
    }
    return Value((Real)midi_note_.note());
  }

  // [2] velocity
  const Value velocity(const Value &val) {
    if (val.is_real()) {
      midi_note_.set_velocity(val.r);
    }
    return Value((Real)midi_note_.velocity());
  }

  // [3] length
  const Value length(const Value &val) {
    if (val.is_real()) {
      midi_note_.set_length(val.r);
    }
    return Value((Real)midi_note_.length());
  }

  // [4] channel
  const Value channel(const Value &val) {
    if (val.is_real()) {
      midi_note_.set_channel(val.r);
    }
    return Value((Real)midi_note_.channel());
  }

  // [5] set note, no send
  const Value set_note(const Value &val) {
    if (val.is_real()) {
      midi_note_.set_note(val.r);
    }
    return Value((Real)midi_note_.note());
  }

  //void clear()
  //{ remove_my_events(); }

  virtual void inspect(Value *hash) const {
    hash->set("note", midi_note_.to_s());
  }

private:
  /* data */
  MidiMessage midi_note_;
};

extern "C" void init(Planet &planet) {
  CLASS (NoteOut, "Helper to create a note.", "note, velocity, length, channel");
  METHOD(NoteOut, note,     Attribute::range_io("Midi note value (sends note out).", 0, 127));
  METHOD(NoteOut, velocity, Attribute::range_io("Note velocity.", 0, 127));
  METHOD(NoteOut, length,   Attribute::range_io("Note length.", 0, 5000));
  METHOD(NoteOut, channel,  Attribute::range_io("Midi channel.", 1, 16));
  METHOD(NoteOut, set_note, Attribute::range_io("Note value (does not send note out).", 0, 127));

  OUTLET(NoteOut, note,     Attribute::midi_io("Sends midi notes out."));
}