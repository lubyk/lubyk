/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

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

#include "oscit/midi_message.h"

#include <vector>
#include <iostream>


namespace oscit {

std::ostream &operator<<(std::ostream &out_stream,
  const MidiMessage &midi_message) {
  std::vector<unsigned char>::const_iterator it,begin,end;
  char buffer[10];
  if (midi_message.type_ == NoteOn || midi_message.type_ == NoteOff) {
    // NoteOn or NoteOff
    if (midi_message.type_ == NoteOff)
      out_stream << "-";
    else
      out_stream << "+";
    midi_message.get_note_name(buffer);
    out_stream << midi_message.channel() << ":" << buffer
               << "(" << (int)midi_message.data_[2] << "), ";
    out_stream << midi_message.wait_ << "/" << midi_message.length_;

  } else if (midi_message.type_ == CtrlChange) {
    // CtrlChange
    out_stream << "~" << (int)midi_message.channel() << ":"
               << (int)midi_message.ctrl() << "("
               << (int)midi_message.value() << "), ";
    out_stream << midi_message.wait_;

  } else if (midi_message.type_ == RawMidi) {
    // RawMidi
    out_stream << "[";
    end   = midi_message.data_.end();
    begin = midi_message.data_.begin();
    for( it = begin; it < end; it++) {
      if (it != begin) out_stream << ", ";
      out_stream << (int)*it << std::endl;
    }
    out_stream << "]";

  } else {
    // No data event
    out_stream << midi_message.type_name();
  }
  return out_stream;
}

}  // oscit
