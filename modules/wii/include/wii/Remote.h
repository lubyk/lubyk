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
 *      ignore:'set_remote,acceleration,button,disconnected'
 */
class Remote
{
  std::string name_;
  LuaCallback acceleration_;
  LuaCallback button_;

  class Implementation;
  Implementation *impl_;
  friend class Implementation;
public:
  Remote(lubyk::Worker *worker, const char *remote_name);

  ~Remote();

  const char *name() const {
    return name_.c_str();
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
    if (key == "acceleration") {
      acceleration_.set_lua_callback(L);
    } else if (key == "button") {
      button_.set_lua_callback(L);
    } else {
      luaL_error(L, "Invalid function name '%s' (valid names are 'acceleration' and 'button').", key.c_str());
    }
  }

  /** Set remote leds.
   */
  void set_leds(bool led1, bool led2, bool led3, bool led4);

  //*********** CALLBACKS

  /** This callback is called on new accelerator data.
   */
  void acceleration(const char *sensor, float x, float y, float z) {
    lua_State *L = acceleration_.lua_;
    if (!L) return;
    ScopedLock lock(acceleration_.worker_);

    acceleration_.push_lua_callback(false);
    lua_pushstring(L, sensor);
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    lua_pushnumber(L, z);
    // <func> <sensor name> <x> <y> <z>
    int status = lua_pcall(L, 4, 0, 0);

    if (status) {
      printf("Error in acceleration callback: %s\n", lua_tostring(L, -1));
    }
  }

  /** This callback is called on button changes.
   */
  void button(const char *type, bool pressed) {
    lua_State *L = button_.lua_;
    if (!L) return;
    ScopedLock lock(button_.worker_);

    button_.push_lua_callback(false);
    lua_pushstring(L, type);
    lua_pushboolean(L, pressed);
    // <func> <btn name> <pressed>
    int status = lua_pcall(L, 2, 0, 0);

    if (status) {
      printf("Error in button callback: %s\n", lua_tostring(L, -1));
    }
  }

  /** This callback is called when the wii Remote is disconnected.
   */
  void disconnected() {
    // noop for the moment
  }

  /** @internal
   */
  void set_remote(void *remote);
};
} // wii

#endif // LUBYK_INCLUDE_WII_REMOTE_H_