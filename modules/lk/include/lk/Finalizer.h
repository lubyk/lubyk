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
 *      destructor: 'finalize'
 */
class Finalizer : public LuaObject {
public:
  Finalizer() {}

  virtual ~Finalizer() {}

  virtual int luaInit(lua_State *L, void *obj, const char *name) throw() {
    LuaObject::luaInit(L, obj, name);
    // ... <self>
    lua_pushstring(L, "super");
    // ... <self> <'super'>
    lua_gettable(L, -2);
    // ... <self> <udata>
    // create our own environment
    lua_newtable(L);
    // ... <self> <udata> <env>
    lua_pushvalue(L, -1);
    // ... <self> <udata> <env> <env>
    if (!lua_setfenv(L, -3)) {
      luaL_error(L, "Could not set userdata env on '%s'.", lua_typename(L, lua_type(L, -3)));
    }
    // ... <self> <udata> <env>
    lua_pushstring(L, "self");
    // ... <self> <udata> <env> <'self'>
    lua_pushvalue(L, -4);
    // ... <self> <udata> <env> <'self'> <self>
    lua_settable(L, -3); // env.self = <self>
    // ... <self> <udata> <env>
    lua_pop(L, 2);
    // ... <self>
    return 1;
  }

  // We do <udata>.<env>.self.finalize(self)
  // <udata>
  void finalize(lua_State *L) {
    lua_getfenv(L, -1);
    // <udata> <env>
    lua_pushstring(L, "self");
    // <udata> <env> <'self'>
    lua_gettable(L, -2);
    // <udata> <env> <self>
    lua_pushstring(L, "finalize");
    // <udata> <env> <self> <'finalize'>
    lua_gettable(L, -2);
    // <udata> <env> <self> <func>
    lua_pushvalue(L, -2);
    // <udata> <env> <self> <func> <self>
    int status = lua_pcall(L, 1, 0, 0);
    if (status) {
      fprintf(stderr, "Error in 'finalize': %s\n", lua_tostring(L, -1));
      lua_pop(L, 1);
    }
    // <udata> <env> <self>
    lua_pop(L, 2);
    // <udata>
    delete this;
  }
};

} // lk

#endif // LUBYK_INCLUDE_LK_FINALIZER_H_
