#include "class.h"
#include "midi_message.h"
#include <sstream>

class NoteOut : public Node
{
public:
  bool init(const Params& p)
  {
    mMessage.type = NoteOn;
    mMessage.set_note( p.get("note", 0) );
    mMessage.set_velocity( p.get("velocity", 80) );
    mLength = p.get("length", 500); // 0.5 sec.
    mMessage.set_channel( p.get("channel", 1) );
  }
  
  // inlet 1
  void set_note(const Signal& sig)
  {
    int n = 0;
    sig.get(&n);
    if (n) mMessage.set_velocity(n);
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
    
    // register note off
    register_event<NoteOut, &NoteOut::noteOff>(mLength, (void *)message);
    
    // send note on
    sig.set(&mMessage);
  }
  
  virtual void spy()
  { 
    std::ostringstream oss(std::ostringstream::out);
    oss << mMessage;
    spy_print("%s, %i", oss.str().c_str(), mLength);
  }
  
private:
  /* data */
  MidiMessage  mMessage;
  time_t mLength;
};

extern "C" void init()
{
  Class::declare<NoteOut>("NoteOut");
}