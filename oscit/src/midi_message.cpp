#include "oscit/midi_message.h"

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