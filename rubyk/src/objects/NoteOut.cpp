#include "rubyk.h"
#include "midi_message.h"
#include <sstream>

class NoteOut : public Node
{
public:
  bool init(const Value& p)
  {
    mMessage.mType = NoteOn;
    mMessage.set_key(     MIDI_NOTE_C0);
    mMessage.set_velocity(70);
    mMessage.set_length(  500); // 0.5 sec.
    mMessage.set_channel( 1);
    
    return true;
  }
  
  bool set(const Value& p)
  {  
    mMessage.set_key(      p.val("note",     mMessage.note()    ));
    mMessage.set_velocity( p.val("velocity", mMessage.velocity()));
    mMessage.set_length(   p.val("length",   mMessage.length()  )); // 0.5 sec.
    mMessage.set_channel(  p.val("channel",  mMessage.channel() ));
    return true;
  }
  
  // inlet 1 and 5 (silent set note)
  void bang(const Value& val)
  {
    if (val.type == MidiValue && val.midi_ptr.value->mType == NoteOn) {
      mMessage = *(val.midi_ptr.value);
    } else {
      set_note(sig);
    }
    send(mMessage);
  }

  // inlet 2
  void set_velocity(const Value& val)
  {
    int v = 0;
    val.get(&v);
    if (v) mMessage.set_velocity(v); 
  }
  
  // inlet 3
  void set_length(const Value& val)
  { 
    time_t l;
    if (val.get(&l)) mMessage.set_length(l); 
  }
  
  // inlet 4
  void set_channel(const Value& val)
  {
    int i;
    if (val.get(&i)) mMessage.set_channel(i);
  }
  
  // inlet 5 (set note but do not send)
  void set_note(const Value& val)
  {
    int n;
    if (val.get(&n)) mMessage.set_note(n);
  }
  
  // internal callback
  void noteOff(void * data)
  {
    MidiMessage * msg = (MidiMessage*)data;
    Outlet * out;
    Value sig;
    
    val.set(msg);
    if ( (out = outlet(1)) ) out->send(sig);
    delete msg;
  }
  
  void clear()
  { remove_my_events(); }
  
  virtual const Value inspect(const Value& val) 
  { 
    std::ostringstream oss(std::ostringstream::out);
    oss << mMessage;
    bprint(mSpy, mSpySize,"%s", oss.str().c_str());
  }
  
private:
  /* data */
  MidiMessage  mMessage;
};

extern "C" void init()
{
  CLASS (NoteOut)
  INLET (NoteOut, set_velocity)
  INLET (NoteOut, set_length)
  INLET (NoteOut, set_channel)
  INLET (NoteOut, set_note)
  OUTLET(NoteOut, send)
  METHOD(NoteOut, clear)
}