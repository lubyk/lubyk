#ifndef _MIDI_MESSAGE_H_
#define _MIDI_MESSAGE_H_

#include <vector>
#include <iostream>

#define MIDI_MIDDLE_C 60

/** Midi messages types. */
enum midi_messages_t {
  NoteOn = 1, /**< Note on message. */
  NoteOff,  /**< Note off message. */
  RawMidi,    /**< Other midi message. */
};

/** This class encapsulates midi messages. */
struct MidiMessage
{
  MidiMessage () : data(3), type(RawMidi) {}
  MidiMessage (unsigned int pDataSize) : data(pDataSize), type(RawMidi) {}
  
  virtual ~MidiMessage () {}
  
  // Constructors //
  
  static MidiMessage noteOn(unsigned int pNote, unsigned int pVelocity, unsigned int pChannel) 
  {
    MidiMessage * msg = new MidiMessage(3);
    msg->type = NoteOn;
    msg->set_channel(pChannel);
    msg->set_note(pNote);
    msg->set_velocity(pVelocity);
    return *msg;
  }
  
  static MidiMessage noteOff(unsigned int pNote, unsigned int pVelocity, unsigned int pChannel) 
  {
    MidiMessage * msg = new MidiMessage(3);
    msg->type = NoteOff;
    msg->set_channel(pChannel);
    msg->set_note(pNote);
    msg->set_velocity(pVelocity);
    return *msg;
  }
  
  inline void note_on_to_off()
  {
    if (type == NoteOn) {
      data[1] -= 0x10;
      type = NoteOff;
    }
  }
  
  inline void set_note(unsigned int pNote)
  { data[0] = pNote % 128; }
  
  inline void set_channel(unsigned int pChannel)
  {
    if (type == NoteOn)
      data[1] = 0x90 + ((pChannel + 15) % 16);
    else
      data[1] = 0x80 + ((pChannel + 15) % 16);
  }
  
  inline unsigned int channel() const
  { 
    if (type == NoteOn)
      return data[1] - 0x90 + 1;
    else
      return data[1] - 0x80 + 1;
  }
  
  inline void set_velocity(unsigned int pVelocity)
  { data[2] = pVelocity % 128; }
  
  /** Write the note name (as C2#, D-1, E3) into the buffer. The buffer must be min 5 chars large (C-3#\0). */
  inline void get_note_name(char buffer[]) const
  {
    unsigned int i = 0;
    int octave = (data[0] - MIDI_MIDDLE_C) / 12;
    int note   = data[0] % 12;
    
    if (type != NoteOn && type != NoteOff) {
      buffer[0] = '?';
      buffer[1] = '?';
      buffer[2] = '\0';
      return;
    }
    switch(note) {
    case 0:
      buffer[i++] = 'C';
      break;
    case 1:
      buffer[i++] = 'C';
      buffer[i++] = '#';
      break;
    case 2:
      buffer[i++] = 'D';
      break;
    case 3:
      buffer[i++] = 'E';
      buffer[i++] = 'b';
      break;
    case 4:
      buffer[i++] = 'E';
      break;
    case 5:
      buffer[i++] = 'F';
      break;
    case 6:
      buffer[i++] = 'F';
      buffer[i++] = '#';
      break;
    case 7:
      buffer[i++] = 'G';
      break;
    case 8:
      buffer[i++] = 'G';
      buffer[i++] = '#';
      break;
    case 9:
      buffer[i++] = 'A';
      break;
    case 10:
      buffer[i++] = 'B';
      buffer[i++] = 'b';
      break;
    case 11:
      buffer[i++] = 'B';
      break;
    default:
      buffer[i++] = '?';
    }
    
    if (octave < 0) {
      buffer[i++] = '-';
      buffer[i++] = '0' - octave;
    } else {
      buffer[i++] = '0' + octave;
    }
    buffer[i] = '\0';
  }
  
  /* data */
  midi_messages_t type;
  std::vector<unsigned char> data; /**< Raw midi message (defined in RtMidi.h). */ 
};

inline std::ostream& operator<< (std::ostream& pStream, const MidiMessage& msg)
{
  std::vector<unsigned char>::const_iterator it,begin,end;
  char buffer[10];
  if (msg.type == NoteOn || msg.type == NoteOff) {
    if (msg.type == NoteOff) 
      pStream << "-";
    else
      pStream << " ";
    msg.get_note_name(buffer);
    pStream << msg.channel() << ":" << buffer << "(" << (int)msg.data[2] << ")";
  } else {
    pStream << "[";
    end   = msg.data.end();
    begin = msg.data.begin();
    for( it = begin; it < end; it++) {
      if (it != begin) pStream << ", ";
      pStream << *it << std::endl;
    }
    pStream << "]";
  }
  return pStream;
}
#endif