#include "class.h"
#include "rtmidi/RtMidi.h"

class Midi : public Node
{
public:
  
  bool init (const Params& p)
  {
    int port;
    
    try {
      mMidiout = new RtMidiOut();
    } catch (RtError &error) {
      *mOutput << mName << ": " << error.getMessageString() << std::endl;
      return false;
    }
    
    if (!p.get(&port, "port", true)) {
      // create a virtual port

      // Call function to select port.
      try {
        mMidiout->openVirtualPort();
      }
      catch (RtError &error) {
        *mOutput << mName << ": " << error.getMessageString() << std::endl;
        // FIXME: close();
        return false;
      }

    } else {
     // Call function to select port.
     try {
       mMidiout->openPort( port );
     }
     catch (RtError &error) {
       *mOutput << mName << ": " << error.getMessageString() << std::endl;
       // FIXME: close ?
       return false;
     }

    }
    return true;
  }
  
  bool set (const Params& p)
  {
    *mOutput << mName << ": cannot change a Midi object during runtime, yet.\n";
    return true;
  }
  
  // inlet 1
  void bang(const Signal& sig)
  {
    if (mDebug) *mOutput << mName << ": " << sig << std::endl;
    
    if (!mMidiout || sig.type != MidiSignal) return;
    
    if (sig.midi_ptr.value->mWait) {
      MidiMessage * msg;
      if (sig.midi_ptr.free_me) {
        sig.clear_free_me(); // we take hold of it
        register_event<Midi, &Midi::send_and_delete>(msg->mWait, (void*)msg);
      } else {
        *mOutput << mName << ": we decided not to implement midimessages that are not released (free_me not true). Please change your code...\n";
      }
    } else if (sig.midi_ptr.free_me) {
      sig.clear_free_me(); // we take hold
      send_and_delete((void*)(sig.midi_ptr.value));
    } else {
      *mOutput << mName << ": we decided not to implement midimessages that are not released (free_me not true). Please change your code...\n";
    }
  }
  
  void send_and_delete(void * data)
  {
    MidiMessage * msg = (MidiMessage*)data;
    mMidiout->sendMessage( &(msg->mData) );
    if (msg->mType == NoteOn && msg->mLength) {
      msg->note_on_to_off();
      register_forced_event<Midi, &Midi::send_and_delete>(msg->mLength, (void*)msg);
    } else {
      delete msg;
    }
  }
  
  
  void clear()
  { remove_my_events(); }
  
  // print a list of possible inputs
  static void inputs(std::ostream * pOutput, const Params& p) {
    RtMidiIn *midiin = 0;
    unsigned int i,nPorts;
    std::string portName;

    // RtMidiOut constructor
    try {
      midiin = new RtMidiIn();
    }
    catch (RtError &error) {
      *pOutput << error.getMessageString() << std::endl;
      return;
    }

    // Check outputs.
    
    nPorts = midiin->getPortCount();
    *pOutput << "Midi in ports (" << nPorts << "):" << std::endl;
    
    for ( i=0; i<nPorts; i++ ) {
      *pOutput << "  " << i << " : " ;
      try {
        portName = midiin->getPortName(i);
        *pOutput << portName << std::endl;
      }
      catch (RtError &error) {
        *pOutput << error.getMessageString() << std::endl;
      }
    }
  }
  
  // print a list of possible outputs
  static void outputs(std::ostream * pOutput, const Params& p) {
    RtMidiOut *midiout = 0;
    unsigned int i,nPorts;
    std::string portName;

    // RtMidiOut constructor
    try {
      midiout = new RtMidiOut();
    }
    catch (RtError &error) {
      *pOutput << error.getMessageString() << std::endl;
      return;
    }

    // Check outputs.
    
    nPorts = midiout->getPortCount();
    *pOutput << "Midi out ports (" << nPorts << "):" << std::endl;
    
    for ( i=0; i<nPorts; i++ ) {
      *pOutput << "  " << i << " : " ;
      try {
        portName = midiout->getPortName(i);
        *pOutput << portName << std::endl;
      }
      catch (RtError &error) {
        *pOutput << error.getMessageString() << std::endl;
      }
    }
  }
private:
  /* data */
  RtMidiOut * mMidiout;
};
/*
// params: portNumber
static VALUE t_initialize(VALUE self, VALUE rPort) {
  
  // RtMidiOut constructor
  try {
    midiout = new RtMidiOut();
  }
  catch (RtError &error) {
    error.printMessage();
    return Qfalse;
  }

  if (rPort == Qnil) {
    // Call function to select port.
    try {
      midiout->openVirtualPort();
    }
    catch (RtError &error) {
      error.printMessage();
      t_close(self);
      return Qnil;
    }
    
  } else {
    // Call function to select port.
    try {
      midiout->openPort( NUM2INT(rPort) );
    }
    catch (RtError &error) {
      error.printMessage();
      t_close(self);
      return Qnil;
    }
    
  }
  return self;
}


static VALUE t_sendMessage(VALUE self, VALUE rByte1, VALUE rByte2, VALUE rByte3) {
  std::vector<unsigned char> message(3);
  
  message[0] = NUM2INT(rByte1);
  message[1] = NUM2INT(rByte2);
  message[2] = NUM2INT(rByte3);
  midiout->sendMessage( &message );

  return Qtrue;
}

// Send note on message
static VALUE t_noteOn(VALUE self, VALUE rChannel, VALUE rNote, VALUE rVelocity) {
  std::vector<unsigned char> message(3);
  message[0] = 0x90 + ((NUM2INT(rChannel) + 15) % 16);
  message[1] = NUM2INT(rNote) % 128;
  message[2] = NUM2INT(rVelocity) % 128;
  midiout->sendMessage( &message );

  return Qtrue;
}

// Send note off message
static VALUE t_noteOff(VALUE self, VALUE rChannel, VALUE rNote, VALUE rVelocity) {
  std::vector<unsigned char> message(3);
  // Send out a series of MIDI messages.
  message[0] = 0x80 + ((NUM2INT(rChannel) + 15) % 16);
  message[1] = NUM2INT(rNote) % 128;
  message[2] = NUM2INT(rVelocity) % 128;
  midiout->sendMessage( &message );

  return Qtrue;
}
*/
extern "C" void init() 
{
  CLASS (Midi)
  CLASS_METHOD(Midi, inputs)
  CLASS_METHOD(Midi, outputs)
  METHOD(Midi, clear)
  // rk_cRtMidi = rb_define_class("RtMidi", rb_cObject);
  // rb_define_singleton_method(rk_cRtMidi, "outputs", (VALUE(*)(...))c_outputs, 0);
  // rb_define_method(rk_cRtMidi, "initialize", (VALUE(*)(...))t_initialize, 1);
  // rb_define_method(rk_cRtMidi, "close", (VALUE(*)(...))t_close, 0);
  // rb_define_method(rk_cRtMidi, "sendMessage", (VALUE(*)(...))t_sendMessage, 3);
  // rb_define_method(rk_cRtMidi, "noteOn", (VALUE(*)(...))t_noteOn, 3); // channel, note, velocity
  // rb_define_method(rk_cRtMidi, "noteOff", (VALUE(*)(...))t_noteOff, 3); // channel, note, velocity
}

