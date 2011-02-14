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
#ifndef LUBYK_INCLUDE_WII_BROWSER_H_
#define LUBYK_INCLUDE_WII_BROWSER_H_

#include "lubyk.h"
using namespace lubyk;

#include "wii/Remote.h"

namespace wii {

/** Browse for Wii Remotes.
 *
 * @dub lib_name:'Browser_core'
 *      ignore: 'found,need_more,find_more'
 */
class Browser : public LuaCallback
{
  class Implementation;
  Implementation *impl_;
  friend class Implementation;
  size_t need_count_;
  bool need_more_;
public:
  Browser(lubyk::Worker *worker);

  ~Browser();

  /** Should be called each time we need to connect to a new remote.
   */
  void find();

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
    if (key == "found") {
      set_lua_callback(L);
    } else {
      luaL_error(L, "Invalid function name '%s' (valid name is 'found').", key.c_str());
    }

    lua_pop(L, 2);
    // ... <self> <key> <value>
  }

  /** A wiimote has been found, ask for a wii.Remote to create
   * connection.
   */
  wii::Remote *found(const char *name) {
    lua_State *L = lua_;
    if (!L) {
      printf("Remote found without browser callback.\n");
      return NULL;
    }

    ScopedLock lock(worker_);
    need_count_--;
    need_more_ = (need_count_ > 0);

    push_lua_callback(false);

    lua_pushstring(L, name);

    // <func> <name>
    int status = lua_pcall(L, 1, 1, 0);

    if (status) {
      printf("Error in 'found' callback: %s\n", lua_tostring(L, -1));
    }

    if (lua_type(lua_, -1) == LUA_TNIL) {
      // no wii.Remote.. cannot link
      return NULL;
    } else {
      return *((wii::Remote **)luaL_checkudata(L, -1, "wii.Remote"));
    }
  }

  /** @internal.
   */
  bool need_more() {
    return need_more_;
  }

  /** @internal
   */
  void find_more();
};
} // wii

#endif // LUBYK_INCLUDE_WII_BROWSER_H_