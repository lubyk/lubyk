/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/
#ifndef LUBYK_INCLUDE_MIDI_IN_H_
#define LUBYK_INCLUDE_MIDI_IN_H_

#include "lubyk.h"
#include "midi/midi.h"
#include "lubyk/Fifo.h"
#include <iostream>

using namespace lubyk;

namespace midi {
  typedef std::vector<unsigned char> MsgVector;

/** This class lets you receive midi events through virtual ports (where possible)
 * or connect to midi senders and receive midi data from them.
 *
 * @dub lib_name:'In_core'
 *      string_format:'%%s (%%f)'
 *      string_args:'(*userdata)->portName(), (*userdata)->port()'
 *      super: 'lubyk.FifoMethods'
 */
class In : public lubyk::Fifo<MsgVector>, public LuaObject {

public:
  In()
      : port_id_(-1),
        port_name_("lubyk"),
        midi_in_(NULL) {

    midi_in_ = new RtMidiIn;

    // set callback
    midi_in_->setCallback(In::s_callback, (void*)this);
  }

  ~In() {}

  int port() const {
    return port_id_;
  }

  const char *portName() const {
    return port_name_.c_str();
  }

  void openPort(int port, lua_State *L) {
    if (midi_in_ == NULL) {
      lua_pushstring(L, "RtMidiIn not initialized: cannot open port.");
      lua_error(L);
    }
    midi_in_->closePort();

    if (port == -1) {
      // create a virtual port
      midi_in_->openVirtualPort(port_name_);
    } else {
      // try to connect to the given port
      midi_in_->openPort(port);
      port_name_ = midi_in_->getPortName(port);
    }
    port_id_ = port;
  }

  void openPort(const char *port_name, lua_State *L) {
    // 1. find port from given name
    int port_count = midi_in_->getPortCount();
    std::string name;

    for (int i = 0; i < port_count; ++i) {
      name = midi_in_->getPortName(i);
      if (name == port_name) {
        return openPort(i, L);
      }
    }
    lua_pushfstring(L, "Port '%s' not found.", port_name);
    lua_error(L);
  }

  void virtualPort(const char *port_name, lua_State *L) {
    port_name_ = port_name;
    return openPort(-1, L);
  }

protected:

  virtual LuaStackSize unpack(lua_State *L, MsgVector *message) {
    if (message->size() == 0) return 0;  // no message
    unsigned char channel = message->at(0);
    
    lua_newtable(L);
    // <tbl>
    // type = 'xxxx'
    lua_pushstring(L, "type");
    switch(channel) {
      case 0xfa:
        lua_pushstring(L, "Clock");
        lua_settable(L, -3);
        lua_pushstring(L, "op");
        lua_pushstring(L, "Start");
        lua_settable(L, -3);
        break;
      case 0xfc:
        lua_pushstring(L, "Clock");
        lua_settable(L, -3);
        lua_pushstring(L, "op");
        lua_pushstring(L, "Stop");
        lua_settable(L, -3);
        break;
      case 0xf8:
        lua_pushstring(L, "Clock");
        lua_settable(L, -3);
        lua_pushstring(L, "op");
        lua_pushstring(L, "Tick");
        lua_settable(L, -3);
        break;
      case 0xfb:
        lua_pushstring(L, "Clock");
        lua_settable(L, -3);
        lua_pushstring(L, "op");
        lua_pushstring(L, "Continue");
        lua_settable(L, -3);
        break;
      default:
        if (message->size() < 3) return 0;  // error
        // FIXME: other messages not implemented yet.
        if (channel >= 0x90) {
          unsigned int velocity = message->at(2);
          if (velocity > 0) {
            lua_pushstring(L, "NoteOn");
          } else {
            lua_pushstring(L, "NoteOff");
          }
          lua_settable(L, -3);
          lua_pushstring(L, "channel");
          lua_pushnumber(L, channel - 0x90 + 1);
          lua_settable(L, -3);

          lua_pushstring(L, "note");
          lua_pushnumber(L, message->at(1));
          lua_settable(L, -3);

          lua_pushstring(L, "velocity");
          lua_pushnumber(L, velocity);
          lua_settable(L, -3);
        } else if (channel >= 0x80) {
          lua_pushstring(L, "NoteOff");
          lua_settable(L, -3);
          lua_pushstring(L, "channel");
          lua_pushnumber(L, channel - 0x80 + 1);
          lua_settable(L, -3);

          lua_pushstring(L, "note");
          lua_pushnumber(L, message->at(1));
          lua_settable(L, -3);

          lua_pushstring(L, "velocity");
          lua_pushnumber(L, message->at(2));
          lua_settable(L, -3);
        } else if (channel >= 0xB0) {
          lua_pushstring(L, "Ctrl");
          lua_settable(L, -3);
          lua_pushstring(L, "channel");
          lua_pushnumber(L, channel - 0x80 + 1);
          lua_settable(L, -3);
          
          lua_pushstring(L, "ctrl");
          lua_pushnumber(L, message->at(1));
          lua_settable(L, -3);

          lua_pushstring(L, "value");
          lua_pushnumber(L, message->at(2));
          lua_settable(L, -3);
        } else {  
          fprintf(stderr, "unknown message type %i.\n", (int)channel);
          return 0;
        }
    }
    return 1;
  }

private:

  /** Static callback to trigger when new messages arrive.
   */
  static void s_callback(double timestamp, MsgVector *message, void *data) {
    ((In*)data)->receive(timestamp, message);
  }

  inline void receive(double timestamp, MsgVector *message) {
    // push a copy
    if (!push(new MsgVector(*message))) {
      fprintf(stderr, "Midi message buffer full !\n");
    }
  }


  // static bool input_list(std::ostream * pOutput, std::vector<std::string>& ports)
  // {
  //   RtMidiIn *midiout = 0;
  //   unsigned int i,nPorts;
  //   std::string portName;
  //
  //   try {
  //     midiout = new RtMidiIn();
  //   }
  //   catch (RtError &error) {
  //     *pOutput << error.getMessageString() << std::endl;
  //     return false;
  //   }
  //
  //   ports.clear();
  //
  //   nPorts = midiout->getPortCount();
  //
  //   for ( i=0; i<nPorts; i++ ) {
  //     try {
  //       portName = midiout->getPortName(i);
  //       ports.push_back(portName);
  //     } catch (RtError &error) {
  //       *pOutput << error.getMessageString() << std::endl;
  //       return false;
  //     }
  //   }
  //   return true;
  // }

  /** Midi port id to which the element is connected.
   *  If the value is -1 this means it has opened its own virtual port.
   */
  int port_id_;

  /** The connected port name.
   */
  std::string port_name_;

  /** Pointer to our RtMidiIn instance (midi::In is just a wrapper around
   *  RtMidiIn).
   */
  RtMidiIn *midi_in_;
};

} // midi
#endif // LUBYK_INCLUDE_MIDI_IN_H_
