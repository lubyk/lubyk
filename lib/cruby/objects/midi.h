/** This file describes the main types used for midi. */
#ifndef _MIDI_H_
#define _MIDI_H_
#include <vector>

typedef 

class MidiMessage
{
public:
  MidiMessage () : mMessage(3);
  virtual ~MidiMessage ();
  
  // Constructors //
  
  static MidiMessage& noteOn(MidiMessage& message, unsigned char pChannel, unsigned char pNote, unsigned char pVelocity) 
  {
    message.mMessage[0] = 0x90 + ((pChannel + 15) % 16);
    message.mMessage[1] = pNote % 128;
    message.mMessage[2] = pVelocity % 128;
  }
  
  static noteOff(MidiMessage& message, unsigned char pChannel, unsigned char pNote, unsigned char pVelocity) 
  {
    message.mMessage[0] = 0x80 + ((pChannel + 15) % 16);
    message.mMessage[1] = pNote % 128;
    message.mMessage[2] = pVelocity % 128;
  }
  
  note_on_to_off()
  {
    if (mMessage[0] > 0x90) mMessage[0] -= 0x10;
  }
  
private:
  /* data */
  std::vector<unsigned char> mMessage; /**< Raw midi message (defined in RtMidi.h). */ 
};
#endif