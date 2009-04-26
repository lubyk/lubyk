#include "rubyk.h"

class NoteOut : public Node {
 public:
  NoteOut() {
    note_.set_as_note(MIDI_NOTE_C0 + 36); // C3
  }
  
  // [1] set note, send current note
  void note(const Value &val) {
    if (val.is_real()) {
      note_.midi_message_->set_note(val.r);
    }
    send(note_);
  }
  
  // [2] velocity
  const Value velocity(const Value &val) {
    if (val.is_real()) {
      note_.midi_message_->set_velocity(val.r);
    }
    return Value((Real)note_.midi_message_->velocity());
  }
  
  // [3] length
  const Value length(const Value &val) {
    if (val.is_real()) {
      note_.midi_message_->set_length(val.r);
    }
    return Value((Real)note_.midi_message_->length());
  }
  
  // [4] channel
  const Value channel(const Value &val) {
    if (val.is_real()) {
      note_.midi_message_->set_channel(val.r);
    }
    return Value((Real)note_.midi_message_->channel());
  }
  
  // [5] set note, no send
  const Value set_note(const Value &val) {
    if (val.is_real()) {
      note_.midi_message_->set_note(val.r);
    }
    return Value((Real)note_.midi_message_->note());
  }
  
  //void clear()
  //{ remove_my_events(); }
  
  virtual void inspect(Value *hash) const {
    hash->set("note", note_);
  }
  
private:
  /* data */
  Value  note_;
};

extern "C" void init(Planet &planet) {
  CLASS (NoteOut, "Helper to create a note.", "note, velocity, length, channel");
  METHOD(NoteOut, note, RangeIO(0, 127, "midi note", "Set value / send note out."));
  OUTLET(NoteOut, note, MidiIO("Midi note."));
  METHOD(NoteOut, velocity, RangeIO(0, 127, "note velocity", "Set/get note velocity."));
  METHOD(NoteOut, length, RealIO("note length", "Set/get note length."));
  METHOD(NoteOut, channel,  RangeIO(1, 16, "midi channel", "Set/get midi channel."));
  METHOD(NoteOut, set_note, RangeIO(0, 127, "midi note", "Set/get note value (does not send note out)."));
}