#ifndef _MIDI_MESSAGE_H_
#define _MIDI_MESSAGE_H_

#include <vector>
#include <iostream>

#define MIDI_NOTE_C0 24

/** Midi messages types. */
enum midi_messages_t {
  NoteOn = 1, /**< Note on message. */
  NoteOff,    /**< Note off message. */
  CtrlChange,
  ClockStart, /**< Start sequencer. */
  ClockStop,  /**< Stop sequencer. */
  ClockTick,  /**< Tick 24 times for every 1/4 note. */ 
  ClockContinue, /**< Continue sequencer after 'pause'. */
  RawMidi,    /**< Other midi message. */
};

/** This class encapsulates midi messages. */
struct MidiMessage
{
  //MidiMessage (const MidiMessage& pMsg)
  //{
  //  mData = pMsg.data;
  //  mWait = pMsg.mWait;
  //  mLength = pMsg.mLength;
  //}
  
  MidiMessage () : mType(RawMidi), mWait(0), mData(3) {}
  MidiMessage (unsigned int pDataSize) : mType(RawMidi), mData(pDataSize) {}
  
  virtual ~MidiMessage () {}
  
  // Constructors //
  
  // Set message from raw midi data. Return false if message could not be set.
  bool set (std::vector<unsigned char>& pMessage, time_t pWait = 0)
  {  
    unsigned char channel;
    if (pMessage.size() == 0) return false; // no message
    switch(pMessage[0])
    {
      case 0xfa:
        mType = ClockStart;
        break;
      case 0xfc:
        mType = ClockStop;
        break;
      case 0xf8:
        mType = ClockTick;
        break;
      case 0xfb:
        mType = ClockContinue;
        break;
      default:
        // FIXME: other messages not implemented yet.
        channel = pMessage[0];
        if (channel >= 0x90) {
          mType = NoteOn;
          mData = pMessage;
          if (velocity() == 0) mType = NoteOff;
        } else if (channel >= 0x80) {
          mType = NoteOff;
          mData = pMessage;
        } else if (channel >= 0xB0) {
          mType = CtrlChange;
          mData = pMessage;
        } else {  
          fprintf(stderr, "unknown message type %i.\n", (int)channel);
          return false;
        }
    }
    mWait = pWait;
    return true;
  }
  
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
  
  void set_as_ctrl (unsigned char pCtrl, unsigned char pCtrlValue, unsigned int pChannel = 1, time_t pTime = 0)
  {
    mType = CtrlChange;
    set_key(pCtrl);
    set_channel(pChannel);
    set_value(pCtrlValue);
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
    else if (mType == NoteOff)
      mData[0] = 0x80 + ((pChannel + 15) % 16);
    else if (mType == CtrlChange)
      mData[0] = 0xB0 + ((pChannel + 15) % 16);
    else
      fprintf(stderr, "set channel for type %i not implemented yet.\n", mType);
  }
  
  inline void set_velocity(unsigned char pVelocity)
  { set_value(pVelocity); }
  
  inline void set_length(time_t pLength)
  { mLength = pLength; }
  
  inline void set_value(unsigned char pValue)
  { mData[2] = pValue % 128; }
  
  inline unsigned int note() const
  { return mData[1]; }
  
  inline unsigned int ctrl() const
  { return mData[1]; }
  
  inline unsigned int value() const
  { return mData[2]; }
  
  inline unsigned int channel() const
  { 
    if (mType == NoteOn)
      return mData[0] - 0x90 + 1;
    else if (mType == NoteOff)
      return mData[0] - 0x80 + 1;
    else if (mType == CtrlChange)
      return mData[0] - 0xB0 + 1;
    else {
      fprintf(stderr, "get channel for type %i not implemented yet.\n", mType);
      return 0;
    }
  }
  
  inline unsigned int velocity() const
  { return mData[2]; }
  
  inline time_t length() const
  { return mLength; }
  
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
  
  /** Return midi message type as a const char. */
  const char * type_name() const
  {
    switch (mType)
    {
      case NoteOn:
        return "NoteOn";
      case NoteOff:
        return "NoteOff";
      case CtrlChange:
        return "Ctrl";
      case ClockStart:
        return "Start";
      case ClockStop:
        return "Stop";
      case ClockTick:
        return ".";
      case ClockContinue:
        return "Continue";
      default:
        return "?";
    }
  }
  /* data */
  midi_messages_t mType;
  time_t          mWait; /**< Wait before sending this note out (ms). */
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
    else
      pStream << "+";
    msg.get_note_name(buffer);
    pStream << msg.channel() << ":" << buffer << "(" << (int)msg.mData[2] << "), ";
    pStream << msg.mWait << "/" << msg.mLength;
  } else if (msg.mType == CtrlChange) {
    pStream << "~" << (int)msg.channel() << ":" << (int)msg.ctrl() << "(" << (int)msg.value() << "), ";
    pStream << msg.mWait;
  } else if (msg.mType == RawMidi) {
    pStream << "[";
    end   = msg.mData.end();
    begin = msg.mData.begin();
    for( it = begin; it < end; it++) {
      if (it != begin) pStream << ", ";
      pStream << (int)*it << std::endl;
    }
    pStream << "]";
  } else {
    pStream << msg.type_name();
  }
  return pStream;
}
#endif