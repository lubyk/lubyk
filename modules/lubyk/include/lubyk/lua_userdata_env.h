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
#ifndef LUBYK_INCLUDE_LUBYK_LUA_USERDATA_PROPS_H_
#define LUBYK_INCLUDE_LUBYK_LUA_USERDATA_PROPS_H_

#include "lubyk.h"

namespace lubyk {
/** Lets Lua set/get properties on the userdata.
 */
class LuaUserdataEnv
{
  bool has_env_;
public:
  LuaUserdataEnv()
   : has_env_(false) {}

  ~LuaUserdataEnv() {}

  // This method inserts the given object to this environment table
  // Stack should be
  // ... <self> <value>
  void add_to_env(lua_State *L) {
    int top = lua_gettop(L);
    if (top < 2) {
      throw Exception("Missing value to add_to_env.");
    }
    get_env(L);
    // ... <self> <value> <env>
    lua_pushvalue(L, -2);
    // ... <self> <value> <env> <value>
    luaL_ref(L, -2);
    // ... <self> <value> <env>
    lua_pop(L, 1);
    // ... <self> <value>
  }

  protected:
    void get_env(lua_State *L, int self_pos = -2) {
      if (!has_env_) {
        lua_newtable(L);
        // ... <self> <value> <env>
        lua_pushvalue(L, -1);
        // ... <self> <value> <env> <env>
        lua_setfenv(L, -4);
        // ... <self> <value> <env>
      } else {
        lua_getfenv(L, -2);
        // ... <self> <value> <env>
      }
    }
};

} // lubyk

#endif // LUBYK_INCLUDE_LUBYK_LUA_USERDATA_PROPS_H_