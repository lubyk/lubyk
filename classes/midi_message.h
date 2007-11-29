#ifndef _MIDI_MESSAGE_H_
#define _MIDI_MESSAGE_H_

#include <vector>
#include <iostream>

#define MIDI_NOTE_C0 24

/** Midi messages types. */
enum midi_messages_t {
  NoteOn = 1, /**< Note on message. */
  NoteOff,  /**< Note off message. */
  Ctrl,
  RawMidi,    /**< Other midi message. */
};

/** This class encapsulates midi messages. */
struct MidiMessage
{
  MidiMessage () : mData(3), mType(RawMidi) {}
  MidiMessage (unsigned int pDataSize) : mData(pDataSize), mType(RawMidi) {}
  
  virtual ~MidiMessage () {}
  
  // Constructors //
  
  void set_as_note (unsigned char pNote, unsigned char pVelocity = 80, unsigned int pLength = 500, unsigned int pChannel = 1, time_t pTime = 0)
  {
    if (pVelocity)
      mType = NoteOn;
    else
      mType = NoteOff;
    set_key(pNote);
    set_channel(pChannel);
    set_value(pVelocity);
    mLength = pLength;
    mWait   = pTime;
  }
  
  void set_as_ctrl (unsigned char pCtrl, unsigned char pValue, unsigned int pLength = 0, unsigned int pChannel = 1, time_t pTime = 0)
  {
    mType = Ctrl;
    set_key(pCtrl);
    set_channel(pChannel);
    set_value(pValue);
    mLength = pLength;
    mWait   = pTime;
  }
  
  inline void note_on_to_off()
  {
    if (mType == NoteOn) {
      mData[0] -= 0x10;
      mType   = NoteOff;
    }
  }
  
  inline void set_note(unsigned char pNote)
  { set_key(pNote); }

  inline void set_ctrl(unsigned char pCtrl)
  { set_key(pCtrl); }
  
  inline void set_key(unsigned char pNote)
  { mData[1] = pNote % 128; }
  
  inline void set_channel(unsigned char pChannel)
  {
    if (mType == NoteOn)
      mData[0] = 0x90 + ((pChannel + 15) % 16);
    else if (mType == Ctrl)
      mData[0] = 0xB0 + ((pChannel + 15) % 16);
    else
      mData[0] = 0x80 + ((pChannel + 15) % 16);
  }
  
  inline unsigned int channel() const
  { 
    if (mType == NoteOn)
      return mData[0] - 0x90 + 1;
    else
      return mData[0] - 0x80 + 1;
  }
  
  inline void set_velocity(unsigned char pVelocity)
  { set_value(pVelocity); }
  
  inline void set_value(unsigned char pValue)
  { mData[2] = pValue % 128; }
  
  /** Write the note name (as C2#, D-1, E3) into the buffer. The buffer must be min 5 chars large (C-3#\0). */
  inline void get_note_name(char buffer[]) const
  {
    unsigned int i = 0;
    int octave = (mData[1] - MIDI_NOTE_C0) / 12;
    int note   = mData[1] % 12;
    
    if (mType != NoteOn && mType != NoteOff) {
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
  midi_messages_t mType;
  time_t          mWait; /**< Wait before sending this note out. */
  time_t        mLength; /**< Note duration in milliseconds. */
  std::vector<unsigned char> mData; /**< Raw midi message (defined in RtMidi.h). */ 
};

inline std::ostream& operator<< (std::ostream& pStream, const MidiMessage& msg)
{
  std::vector<unsigned char>::const_iterator it,begin,end;
  char buffer[10];
  if (msg.mType == NoteOn || msg.mType == NoteOff) {
    if (msg.mType == NoteOff) 
      pStream << "-";
    else if (msg.mType == NoteOn)
      pStream << "+";
    else if (msg.mType == Ctrl)
      pStream << "~";
    msg.get_note_name(buffer);
    pStream << msg.channel() << ":" << buffer << "(" << (int)msg.mData[2] << "), " << msg.mWait << "/" << msg.mLength;
  } else {
    pStream << "[";
    end   = msg.mData.end();
    begin = msg.mData.begin();
    for( it = begin; it < end; it++) {
      if (it != begin) pStream << ", ";
      pStream << *it << std::endl;
    }
    pStream << "]";
  }
  return pStream;
}
#endif