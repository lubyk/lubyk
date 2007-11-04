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
  {}

  // inlet 2
  void set_velocity(const Signal& sig)
  {}

  // inlet 3
  void set_duration(const Signal& sig)
  {}
  
  // inlet 4
  void set_channel(const Signal& sig)
  {}
  
  // internal callback
  void noteOff(void * data)
  {
    mNoteOff = (MidiMessage*)data;
    bang();
  }
  
  // outlet 1
  void send(Signal& sig)
  {
    if (mNoteOff) {
      // we have a note off (this is the noteOff's 'bang')
      SEND_VOID_POINTER(mMessage, true); // free after sending
    } else {
      // send note on and register note off
      MidiMessage * message = new MidiMessage(mMessage);
      
      message->note_on_to_off();
      
      // register note off
      register_event<NoteOut, &NoteOut::noteOff>(mDuration, (void *)message);
      
      // send note on
      SEND_VOID_POINTER(mMessage, false);
    }
  }
  
private:
  /* data */
  MidiMessage   mNoteOn;
  MidiMessage * mNoteOff;
  unsigned long mDuration;
};