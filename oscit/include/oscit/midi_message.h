#ifndef OSCIT_INCLUDE_OSCIT_MIDI_MESSAGE_H_
#define OSCIT_INCLUDE_OSCIT_MIDI_MESSAGE_H_
#include <vector>
#include <iostream>

#include "oscit/reference_counted.h"

namespace oscit {

#define MIDI_NOTE_C0 24

/** Midi messages types. */
enum MidiMessageType {
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
class MidiMessage : public ReferenceCounted {
 public:
  MidiMessage() : type_(RawMidi), wait_(0), data_(3) {}
  explicit MidiMessage(unsigned int data_size) : type_(RawMidi), data_(data_size) {}
  
  virtual ~MidiMessage () {}
  
  /** Set message from raw midi data. Return false if message could not be set.
   */
  bool set(std::vector<unsigned char> &message, time_t wait = 0) {  
    unsigned char channel;
    if (message.size() == 0) return false;  // no message
    
    switch(message[0]) {
      case 0xfa:
        type_ = ClockStart;
        break;
      case 0xfc:
        type_ = ClockStop;
        break;
      case 0xf8:
        type_ = ClockTick;
        break;
      case 0xfb:
        type_ = ClockContinue;
        break;
      default:
        // FIXME: other messages not implemented yet.
        channel = message[0];
        if (channel >= 0x90) {
          type_ = NoteOn;
          data_ = message;
          if (velocity() == 0) type_ = NoteOff;
        } else if (channel >= 0x80) {
          type_ = NoteOff;
          data_ = message;
        } else if (channel >= 0xB0) {
          type_ = CtrlChange;
          data_ = message;
        } else {  
          fprintf(stderr, "unknown message type %i.\n", (int)channel);
          return false;
        }
    }
    wait_ = wait;
    return true;
  }
  
  void set_as_note(unsigned char note, unsigned char velocity = 80,
    unsigned int length = 500, unsigned int channel = 1, time_t wait = 0) {
    if (velocity != 0) {
      type_ = NoteOn;
    } else {
      type_ = NoteOff;
    }
    set_key(note);
    set_channel(channel);
    set_value(velocity);
    length_ = length;
    wait_   = wait;
  }
  
  void set_as_ctrl (unsigned char ctrl, unsigned char ctrl_value,
    unsigned int channel = 1, time_t wait = 0) {
    type_ = CtrlChange;
    set_key(ctrl);
    set_channel(channel);
    set_value(ctrl_value);
    wait_   = wait;
  }
  
  inline void note_on_to_off() {
    if (type_ == NoteOn) {
      data_[0] -= 0x10;
      type_   = NoteOff;
    }
  }
  
  void set_type(MidiMessageType type) {
    type_ = type;
  }
  
  inline void set_note(unsigned char note) {
    set_key(note);
  }

  inline void set_ctrl(unsigned char ctrl) {
    set_key(ctrl);
  }
  
  inline void set_key(unsigned char note)  {
    data_[1] = note % 128;
  }
  
  inline void set_channel(unsigned char channel) {
    if (type_ == NoteOn) {
      data_[0] = 0x90 + ((channel + 15) % 16);
    } else if (type_ == NoteOff) {
      data_[0] = 0x80 + ((channel + 15) % 16);
    } else if (type_ == CtrlChange) {
      data_[0] = 0xB0 + ((channel + 15) % 16);
    } else {
      fprintf(stderr, "set channel for type %i not implemented yet.\n", type_);
    }
  }
  
  inline void set_velocity(unsigned char velocity) {
    set_value(velocity);
  }
  
  inline void set_length(time_t length) {
    length_ = length;
  }
  
  inline void set_value(unsigned char pValue) {
    data_[2] = pValue % 128;
  }
  
  inline MidiMessageType type() const { return type_; }
  
  inline unsigned int note() const { return data_[1]; }
  
  inline unsigned int ctrl() const { return data_[1]; }
  
  inline unsigned int value() const { return data_[2]; }
  
  inline unsigned int channel() const { 
    if (type_ == NoteOn) {
      return data_[0] - 0x90 + 1;
    } else if (type_ == NoteOff) {
      return data_[0] - 0x80 + 1;
    } else if (type_ == CtrlChange) {
      return data_[0] - 0xB0 + 1;
    } else {
      fprintf(stderr, "get channel for type %i not implemented yet.\n", type_);
      return 0;
    }
  }
  
  inline unsigned int velocity() const { return data_[2]; }
  
  inline time_t length() const { return length_; }
  
  /** Write the note name (as C2#, D-1, E3) into the buffer. The buffer must be min 5 chars large (C-3#\0). */
  inline void get_note_name(char buffer[]) const {
    unsigned int i = 0;
    int octave = (data_[1] - MIDI_NOTE_C0) / 12;
    int note   = data_[1] % 12;
    
    if (type_ != NoteOn && type_ != NoteOff) {
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
  const char * type_name() const {
    switch (type_)
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
  
 private:
  friend std::ostream &operator<<(std::ostream &out_stream,
    const MidiMessage &midi_message);
    
  /** Type of midi messate (NoteOn, NoteOff, etc).
   */
  MidiMessageType type_;
  
  /** Number of milliseconds (?) to wait before sending the midi event out.
   */
  time_t          wait_;
  
  /** Duration of note in milliseconds.
   */
  time_t        length_;
  
  /** Raw midi message as a vector of unsigned char (compatible with RtMidi).
   */
  std::vector<unsigned char> data_;
};

std::ostream &operator<<(std::ostream &out_stream,
  const MidiMessage &midi_message);

}  // oscit
#endif  // OSCIT_INCLUDE_OSCIT_MIDI_MESSAGE_H_