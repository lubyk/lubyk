#include "rubyk.h"
#include "midi/RtMidi.h"

class MidiIn : public Node
{
public:
  
  bool init ()
  {
    mPortId = -1;
    
    try {
      mMidiIn = new RtMidiIn();
    } catch (RtError &error) {
      *mOutput << mName << ": " << error.getMessageString() << std::endl;
      return false;
    }

    if (!p.get(&mPortId, "port", true)) {
      // create a virtual port

      // Call function to select port.
      try {
        mMidiIn->openVirtualPort();
      }
      catch (RtError &error) {
        *mOutput << mName << ": " << error.getMessageString() << std::endl;
        // FIXME: close();
        return false;
      }

    } else {
     // Call function to select port.
     try {
       mMidiIn->openPort( mPortId );
       *mOutput << mName << ": opened port " << mPortId << "\n";
     }
     catch (RtError &error) {
       *mOutput << mName << ": " << error.getMessageString() << std::endl;
       // FIXME: close ?
       return false;
     }

    }
    
    // Don't ignore sysex, timing, or active sensing messages.
    mMidiIn->ignoreTypes( false, false, false );
    
    // register as input
    loop_me();

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
    if (val.type == NilValue && mMidiIn)
      get_messages();
  }
  
  void send_and_delete(void * data)
  {
    MidiMessage * msg = (MidiMessage*)data;
    send(msg);
    delete msg;
  }
  
  void clear()
  { remove_my_events(); }
  
  // print a list of possible inputs
  static void list(std::ostream * pOutput, const Value& p) {
    std::vector<std::string> ports;
    if (!input_list(pOutput, ports)) return;
    size_t nPorts = ports.size();
    
    *pOutput << "Midi in ports (" << nPorts << "):" << std::endl;

    for (size_t i=0; i<nPorts; i++ ) {
      *pOutput << "  " << i << ": " << ports[i] << std::endl;
    }
  }
  
  virtual const Value inspect(const Value& val) 
  { 
    std::vector<std::string> portList;
    if (mPortId < 0)
      bprint(mSpy, mSpySize, "-virtual-");
    else if (input_list(mOutput, portList) && (size_t)mPortId < portList.size())
      bprint(mSpy, mSpySize,"%i: %s", mPortId, portList[mPortId].c_str());
    else
      bprint(mSpy, mSpySize,"could not read port name for %i", mPortId);
  }
private:
  
  static bool input_list(std::ostream * pOutput, std::vector<std::string>& pPorts)
  {
    RtMidiIn *midiin = 0;
    unsigned int i,nPorts;
    std::string portName;

    try {
      midiin = new RtMidiIn();
    }
    catch (RtError &error) {
      *pOutput << error.getMessageString() << std::endl;
      return false;
    }

    pPorts.clear();
    
    nPorts = midiin->getPortCount();
    
    for ( i=0; i<nPorts; i++ ) {
      try {
        portName = midiin->getPortName(i);
        pPorts.push_back(portName);
      }
      catch (RtError &error) {
        *pOutput << error.getMessageString() << std::endl;
        return false;
      }
    }
    return true;
  }
  
  
  /** Send midi messages out. */
  void get_messages()
  {
    std::vector<unsigned char> message;
    real_t wait;
    while (true)
    {
      wait = mMidiIn->getMessage( &message );

      if (mMidiMessage.set(message, (time_t)(wait * 1000))) {
        // send message out
        switch(mMidiMessage.mType)
        {
          case NoteOn:
            send(2, mMidiMessage);
            break;
          case NoteOff:
            send(2, mMidiMessage);
            break;
          case CtrlChange:
            send(3, mMidiMessage);
            break;
          case RawMidi:
            break;
          default:
            // clock event
            if (mMidiMessage.mType == ClockTick) {
              send(5, mMidiMessage);
            }
            send(4, mMidiMessage);
        }
        
        // all messages through outlet 1
        send(mMidiMessage);
      } else {
        // no more messages.
        return;
      }
    }
  }
  
  void send_with_delay(MidiMessage& pMessage)
  {
    // TODO
  }
  
  int mPortId;
  RtMidiIn  * mMidiIn;
  MidiMessage mMidiMessage;
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
  CLASS (MidiIn)
  OUTLET(MidiIn,all)    // 1
  OUTLET(MidiIn,notes)  // 2
  OUTLET(MidiIn,ctrl)   // 3
  OUTLET(MidiIn,clock)  // 4
  OUTLET(MidiIn,tick)   // 5
  CLASS_METHOD(MidiIn, list)
  METHOD(MidiIn, clear)
  // rk_cRtMidi = rb_define_class("RtMidi", rb_cObject);
  // rb_define_singleton_method(rk_cRtMidi, "outputs", (VALUE(*)(...))c_outputs, 0);
  // rb_define_method(rk_cRtMidi, "initialize", (VALUE(*)(...))t_initialize, 1);
  // rb_define_method(rk_cRtMidi, "close", (VALUE(*)(...))t_close, 0);
  // rb_define_method(rk_cRtMidi, "sendMessage", (VALUE(*)(...))t_sendMessage, 3);
  // rb_define_method(rk_cRtMidi, "noteOn", (VALUE(*)(...))t_noteOn, 3); // channel, note, velocity
  // rb_define_method(rk_cRtMidi, "noteOff", (VALUE(*)(...))t_noteOff, 3); // channel, note, velocity
}

