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

using namespace lubyk;

namespace midi {
/** This class lets you receive midi events through virtual ports (where possible)
 * or connect to midi senders and receive midi data from them.
 *
 * @dub lib_name:'In_core'
 *      string_format:'%%s (%%f)'
 *      string_args:'(*userdata)->port_name(), (*userdata)->port()'
 */
class In : LuaCallback {
public:
  In(lubyk::Worker *worker)
   : LuaCallback(worker),
     port_id_(-1),
     port_name_("lubyk"),
     midi_in_(NULL) {

    midi_in_ = new RtMidiIn;

    // set callback
    midi_in_->setCallback(In::s_callback, (void*)this);
  }

  ~In() {
    if (midi_in_) {
      delete midi_in_;
    }
  }

  int port() const {
    return port_id_;
  }

  const char *port_name() const {
    return port_name_.c_str();
  }

  void open_port(int port, lua_State *L) {
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

  void open_port(const char *port_name, lua_State *L) {
    // 1. find port from given name
    int port_count = midi_in_->getPortCount();
    std::string name;

    for (int i = 0; i < port_count; ++i) {
      name = midi_in_->getPortName(i);
      if (name == port_name) {
        return open_port(i, L);
      }
    }
    lua_pushfstring(L, "Port '%s' not found.", port_name);
    lua_error(L);
  }

  void virtual_port(const char *port_name, lua_State *L) {
    port_name_ = port_name;
    return open_port(-1, L);
  }

  /** Set a callback function.
   *
   */
  void __newindex(lua_State *L) {
    // Stack should be ... <self> <key> <value>
    std::string key(luaL_checkstring(L, -2));

    luaL_checktype(L, -1, LUA_TFUNCTION);
    lua_pushvalue(L, -3);
    // ... <self> <key> <value> <self>
    lua_pushvalue(L, -2);
    // ... <self> <key> <value> <self> <value>
    if (key == "receive") {
      set_lua_callback(L);
    } else {
      luaL_error(L, "Invalid callback name '%s' (valid name is 'receive').", key.c_str());
    }

    lua_pop(L, 2);
    // ... <self> <key> <value>
  }
private:

  /** Static callback to trigger when new messages arrive.
   */
  static void s_callback(double timestamp, std::vector<unsigned char> *message, void *data) {
    if (message->size() == 3) {
      ((In*)data)->receive(message);
    }
  }

  inline void receive(std::vector<unsigned char> *message) {
    lua_State *L = lua_;
    if (!lua_) return;
    ScopedLock lock(worker_);

    push_lua_callback(false);
    lua_pushnumber(L, (*message)[0]);
    lua_pushnumber(L, (*message)[1]);
    lua_pushnumber(L, (*message)[2]);

    int status = lua_pcall(lua_, 3, 0, 0);

    if (status) {
      fprintf(stderr, "Error in 'receive' callback: %s\n", lua_tostring(L, -1));
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