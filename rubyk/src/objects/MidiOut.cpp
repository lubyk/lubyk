#include "rubyk.h"
#include "midi/RtMidi.h"

class MidiOut : public Node
{
public:
  
  bool init (const Value& p)
  {
    mPortId = -1;
    
    try {
      mMidiOut = new RtMidiOut();
    } catch (RtError &error) {
      *mOutput << mName << ": " << error.getMessageString() << std::endl;
      return false;
    }
    
    if (!p.get(&mPortId, "port", true)) {
      // create a virtual port

      // Call function to select port.
      try {
        mMidiOut->openVirtualPort();
      }
      catch (RtError &error) {
        *mOutput << mName << ": " << error.getMessageString() << std::endl;
        // FIXME: close();
        return false;
      }

    } else {
     // Call function to select port.
     try {
       mMidiOut->openPort( mPortId );
     }
     catch (RtError &error) {
       *mOutput << mName << ": " << error.getMessageString() << std::endl;
       // FIXME: close ?
       return false;
     }

    }
    return true;
  }
  
  bool set (const Value& p)
  {
    *mOutput << mName << ": cannot change a Midi object during runtime, yet.\n";
    return true;
  }
  
  // inlet 1
  void bang(const Value& val)
  {
    MidiMessage * msg;
    if (mDebug) *mOutput << mName << ": " << sig << std::endl;
    
    if (!mMidiOut || sig.type != MidiValue) return;
    
    if (sig.midi_ptr.value->mWait) {
      if (sig.midi_ptr.free_me) {
        sig.clear_free_me(); // we take hold of it
        register_event<MidiOut, &MidiOut::send_and_delete>(sig.midi_ptr.value->mWait, (void*)(sig.midi_ptr.value));
      } else {
        // copy
        msg = new MidiMessage(*(sig.midi_ptr.value));
        register_event<MidiOut, &MidiOut::send_and_delete>(msg->mWait, (void*)msg);
      }
    } else if (sig.midi_ptr.free_me) {
      sig.clear_free_me(); // we take hold
      send_and_delete((void*)(sig.midi_ptr.value));
    } else {  
      msg = new MidiMessage(*(sig.midi_ptr.value));
      send_and_delete((void*)(msg));
    }
  }
  
  void send_and_delete(void * data)
  {
    MidiMessage * msg = (MidiMessage*)data;
    mMidiOut->sendMessage( &(msg->mData) );
    if (msg->mType == NoteOn && msg->mLength) {
      msg->note_on_to_off();
      register_forced_event<MidiOut, &MidiOut::send_and_delete>(msg->mLength, (void*)msg);
    } else {
      delete msg;
    }
  }
  
  
  void clear()
  { remove_my_events(); }
  
  // print a list of possible outputs
  static void list(std::ostream * pOutput, const Value& p)
  {
    std::vector<std::string> ports;
    if (!output_list(pOutput, ports)) return;
    size_t nPorts = ports.size();
    
    *pOutput << "Midi out ports (" << nPorts << "):" << std::endl;

    for (size_t i=0; i<nPorts; i++ ) {
      *pOutput << "  " << i << ": " << ports[i] << std::endl;
    }
  }
  
  virtual const Value inspect(const Value& val) 
  { 
    std::vector<std::string> portList;
    if (mPortId < 0)
      bprint(mSpy, mSpySize, "-virtual-");
    else if (output_list(mOutput, portList) && (size_t)mPortId < portList.size())
      bprint(mSpy, mSpySize,"%i: %s", mPortId, portList[mPortId].c_str());
    else
      bprint(mSpy, mSpySize,"could not read port name for %i", mPortId);
  }
private:
  
  static bool output_list(std::ostream * pOutput, std::vector<std::string>& pPorts)
  {
    RtMidiOut *midiout = 0;
    unsigned int i,nPorts;
    std::string portName;

    try {
      midiout = new RtMidiOut();
    }
    catch (RtError &error) {
      *pOutput << error.getMessageString() << std::endl;
      return false;
    }

    pPorts.clear();
    
    nPorts = midiout->getPortCount();
    
    for ( i=0; i<nPorts; i++ ) {
      try {
        portName = midiout->getPortName(i);
        pPorts.push_back(portName);
      }
      catch (RtError &error) {
        *pOutput << error.getMessageString() << std::endl;
        return false;
      }
    }
    return true;
  }
  
  int mPortId;
  RtMidiOut * mMidiOut;
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
  CLASS (MidiOut)
  CLASS_METHOD(MidiOut, list)
  METHOD(MidiOut, clear)
  // rk_cRtMidi = rb_define_class("RtMidi", rb_cObject);
  // rb_define_singleton_method(rk_cRtMidi, "outputs", (VALUE(*)(...))c_outputs, 0);
  // rb_define_method(rk_cRtMidi, "initialize", (VALUE(*)(...))t_initialize, 1);
  // rb_define_method(rk_cRtMidi, "close", (VALUE(*)(...))t_close, 0);
  // rb_define_method(rk_cRtMidi, "sendMessage", (VALUE(*)(...))t_sendMessage, 3);
  // rb_define_method(rk_cRtMidi, "noteOn", (VALUE(*)(...))t_noteOn, 3); // channel, note, velocity
  // rb_define_method(rk_cRtMidi, "noteOff", (VALUE(*)(...))t_noteOff, 3); // channel, note, velocity
}

