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
#ifndef LUBYK_INCLUDE_LK_DEBUG_H_
#define LUBYK_INCLUDE_LK_DEBUG_H_

#include "lubyk.h"
using namespace lubyk;

namespace lk {
/** The sole purpose of this class is to ease garbage collection testing
 * in lua. The object calls "callback" when being garbage collected.
 * 
 * @dub lib_name: 'Debug_core'
 */
class Debug : public ThreadedLuaObject {
  std::string name_;
public:
  Debug(lua_State *L) {
    int top = lua_gettop(L);
    for(int i=1; i<=top; ++i) {
      if (i>1) name_.append("\t");
      if (lua_isstring(L, i)) {
        name_.append(lua_tostring(L, i));
      } else {
        char buf[1024];
        // <self> <...>
        // convert arguments to inspect name
        snprintf(buf, 1024, "%s:%p", luaL_typename(L,-1), lua_topointer(L,-1));
        name_.append(buf);
      }
    }
    printf("++++++++++++++ %s\n", name_.c_str());
    lua_pop(L, top); // clear so that a table is not used as Debug's self
  }

  virtual ~Debug() {
    printf("-------------- ~%s\n", name_.c_str());
    if (!pushLuaCallback("callback")) return;
    // <func> <self>
    int status = lua_pcall(lua_, 1, 0, 0);

    if (status) {
      printf("Error in callback function: %s\n", lua_tostring(lua_, -1));
    }
  }

  /** Return userdata type and pointer value.
   */
  static LuaStackSize userdata(lua_State *L) {
    void *p = lua_touserdata(L, 1);
    if (p != NULL) {
      void *ptr = *((void**)(p));
      // <ud>
      if (lua_getmetatable(L, -1)) {
        // <ud> <mt>
        lua_pushstring(L, "type");
        // <ud> <mt> "type"
        lua_gettable(L, -2);
        // <ud> <mt> "type"
        // <ud> <mt> "libname.classname"
        lua_pushfstring(L, "%p", ptr);
        // <ud> <mt> "libname.classname" <ptr value>
        return 2;
      }
    }
    return 0;
  }
};

} // lk

#endif // LUBYK_INCLUDE_LK_DEBUG_H_
