#include "class.h"
#include "midi_message.h"
#include <sstream>

class NoteOut : public Node
{
public:
  bool init(const Params& p)
  {
    mMessage.type = NoteOn;
    mMessage.set_note(     p.val("note",     MIDI_NOTE_C0   ));
    mMessage.set_velocity( p.val("velocity", 70             ));
    mLength =              p.val("length",   500             ); // 0.5 sec.
    mMessage.set_channel(  p.val("channel",  1              ));
    
    return true;
  }
  
  // inlet 1 and 5 (silent set note)
  void set_note(const Signal& sig)
  {
    int n = 0;
    if (sig.type == MidiSignal) {
      mMessage = *(sig.midi_ptr.value);
    } else {
      sig.get(&n);
      if (n) mMessage.set_note(n); 
    }
  }

  // inlet 2
  void set_velocity(const Signal& sig)
  {
    int v = 0;
    sig.get(&v);
    if (v) mMessage.set_velocity(v); 
  }
  
  // inlet 3
  void set_length(const Signal& sig)
  { sig.get(&mLength); }
  
  // inlet 4
  void set_channel(const Signal& sig)
  {
    int i;
    if (sig.get(&i)) mMessage.set_channel(i);
  }
  
  // internal callback
  void noteOff(void * data)
  {
    MidiMessage * msg = (MidiMessage*)data;
    Outlet * out;
    Signal sig;
    
    sig.set(msg);
    if (out = outlet(1)) out->send(sig);
    delete msg;
  }
  
  // outlet 1
  void send(Signal& sig)
  {
    // send note on and register note off
    MidiMessage * message = new MidiMessage(mMessage);
    
    message->note_on_to_off();
    
    // register note off (forced = must be run on quit)
    register_forced_event<NoteOut, &NoteOut::noteOff>(mLength, (void *)message);
    // send note on
    sig.set(&mMessage);
  }
  
  void clear()
  { remove_my_events(); }
  
  virtual void spy()
  { 
    std::ostringstream oss(std::ostringstream::out);
    oss << mMessage;
    bprint(mSpy, mSpySize,"%s, %i", oss.str().c_str(), mLength);
  }
  
private:
  /* data */
  MidiMessage  mMessage;
  time_t mLength;
};

extern "C" void init()
{
  CLASS (NoteOut)
  INLET (NoteOut, set_note)
  INLET (NoteOut, set_velocity)
  INLET (NoteOut, set_length)
  INLET (NoteOut, set_channel)
  INLET (NoteOut, set_note)
  OUTLET(NoteOut, send)
  METHOD(NoteOut, clear)
}