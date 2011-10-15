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
#ifndef LUBYK_INCLUDE_WII_REMOTE_H_
#define LUBYK_INCLUDE_WII_REMOTE_H_

#include "lubyk.h"
using namespace lubyk;

namespace wii {

class Browser;

/** Receive data from a Wiimote.
 *
 * @dub lib_name:'Remote_core'
 *      string_format:'%%s'
 *      string_args:'(*userdata)->name()'
 *      ignore:'setRemote,acceleration,button,disconnected'
 */
class Remote : public LuaObject {
  std::string name_;

  class Implementation;
  Implementation *impl_;
  friend class Implementation;
public:
  Remote(const char *remote_name = NULL);

  ~Remote();

  const char *name() const {
    return name_.c_str();
  }

  void disconnect();

  /** Set remote leds.
   */
  void setLeds(bool led1, bool led2, bool led3, bool led4);

  //*********** CALLBACKS

  /** This callback is called on new accelerator data.
   */
  void acceleration(const char *sensor, float x, float y, float z) {
    lua_State *L = lua_;

    if (!pushLuaCallback("acceleration")) return;
    lua_pushstring(L, sensor);
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    lua_pushnumber(L, z);
    // <func> <self> <sensor name> <x> <y> <z>
    int status = lua_pcall(L, 5, 0, 0);

    if (status) {
      printf("Error in 'acceleration' callback: %s\n", lua_tostring(L, -1));
    }
  }

  /** This callback is called on button changes.
   */
  void button(const char *type, bool pressed) {
    lua_State *L = lua_;

    if (!pushLuaCallback("button")) return;
    lua_pushstring(L, type);
    lua_pushboolean(L, pressed);
    // <func> <self> <btn name> <pressed>
    int status = lua_pcall(L, 3, 0, 0);

    if (status) {
      printf("Error in 'button' callback: %s\n", lua_tostring(L, -1));
    }
  }

  /** This callback is called when the wii Remote is disconnected.
   */
  void disconnected() {
    lua_State *L = lua_;

    if (!pushLuaCallback("disconnected")) return;
    // <func> <self>
    int status = lua_pcall(L, 1, 0, 0);

    if (status) {
      printf("Error in 'disconnected' callback: %s\n", lua_tostring(L, -1));
    }
  }

  /** This callback is called when the wii Remote is disconnected.
   */
  void connected() {
    lua_State *L = lua_;

    if (!pushLuaCallback("connected")) {
      // default action
      setLeds(true, false, false, false);
      return;
    }

    lua_pushstring(L, name_.c_str());
    // <func> <self> <name>
    int status = lua_pcall(L, 2, 0, 0);

    if (status) {
      printf("Error in 'connected' callback: %s\n", lua_tostring(L, -1));
    }
  }

  /** @internal
   */
  void setRemote(void *remote);
};
} // wii

#endif // LUBYK_INCLUDE_WII_REMOTE_H_
