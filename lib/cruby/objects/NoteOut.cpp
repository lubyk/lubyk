#include "class.h"
#include "midi.h"

class NoteOut : public Node
{
public:
  bool init(const Params& p)
  {
    
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
  void set_duration(const Signal& sig)
  { sig.set(mDuration); }
  
  // inlet 4
  void set_channel(const Signal& sig)
  {
    int i;
    sig.update(i);
    if (sig.get(&i)) mMessage.set_channel(i);
  }
  
  // internal callback
  void noteOff(void * data)
  {
    MidiMessage * msg = (MidiMessage*)data;
    Outlet * out;
    Signal sig;
    
    sig.set((void*)msg);
    
    if (out = outlet(1)) out->send(s);
    delete msg;
  }
  
  // outlet 1
  void send(Signal& sig)
  {
    // send note on and register note off
    MidiMessage * message = new MidiMessage(mMessage);
    
    message->note_on_to_off();
    
    // register note off
    register_event<NoteOut, &NoteOut::noteOff>(mDuration, (void *)message);
    
    // send note on
    sig.set((void*)mMessage);
  }
  
private:
  /* data */
  MidiMessage   mMessage;
  unsigned long mDuration;
};