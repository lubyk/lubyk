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
#ifndef LUBYK_INCLUDE_LK_FINALIZER_H_
#define LUBYK_INCLUDE_LK_FINALIZER_H_

#include "lubyk.h"
using namespace lubyk;

namespace lk {
/** The sole purpose of this class is to ease garbage collection testing
 * in lua. The object calls "callback" when being garbage collected.
 * 
 * @dub lib_name: 'Finalizer_core'
 *      destructor: 'finalizer'
 */
class Finalizer : public LuaObject {
public:
  Finalizer() {}

  virtual ~Finalizer() {}

  // ... <func>
  virtual int luaInit(lua_State *L, void *obj, const char *name) throw() {
    luaL_checktype(L, -1, LUA_TFUNCTION);
    LuaObject::luaInit(L, obj, name);
    // ... <func> <self>
    lua_pushstring(L, "super");
    // ... <func> <self> <'super'>
    lua_gettable(L, -2);
    // ... <func> <self> <udata>
    lua_getfenv(L, -1);
    // ... <func> <self> <udata> <env>
    lua_pushstring(L, "finalizer");
    // ... <func> <self> <udata> <env> <'finalizer'>
    lua_pushvalue(L, -5);
    // ... <func> <self> <udata> <env> <'finalizer'> <func>
    lua_settable(L, -3);
    // ... <func> <self> <udata> <env>
    lua_pop(L, 2);
    return 1;
  }

  // <udata>
  void finalizer(lua_State *L) {
    lua_getfenv(L, -1);
    // <udata> <env>
    lua_pushstring(L, "finalizer");
    // <udata> <env> <'finalizer'>
    lua_gettable(L, -2);
    // <udata> <env> <func>
    int status = lua_pcall(L, 0, 0, 0);
    if (status) {
      fprintf(stderr, "Error in 'finalizer': %s\n", lua_tostring(L, -1));
      lua_pop(L, 1);
    }
    // <udata> <env>
    lua_pop(L, 1);
    // <udata>
    delete this;
  }
};

} // lk

#endif // LUBYK_INCLUDE_LK_FINALIZER_H_
