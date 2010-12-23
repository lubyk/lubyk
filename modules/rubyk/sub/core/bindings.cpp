/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher - Buma (http://teti.ch).

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
#include "rubyk.h"

void rubyk::dump_lua_stack(lua_State *L, const char *msg) {
  int i;
  int top = lua_gettop(L);

  printf("%s (top: %d)\n", msg, top);

  for (i = 1; i <= top; ++i) {
    switch (lua_type(L, i)) {
      case LUA_TSTRING:
        printf("%3i: '%s'\n", i, lua_tostring(L, i));
        break;
      case LUA_TBOOLEAN:
        printf("%3i: %s\n", i, lua_toboolean(L, i) ? "true" : "false");
        break;
      case LUA_TNUMBER:
        printf("%3i: %g\n", i, lua_tonumber(L, i));
        break;
      case LUA_TTABLE:
        printf("%3i: {...}\n", i);
        break;
      case LUA_TNONE:
        printf("%3i: --\n", i);
      case LUA_TNIL:
        printf("%3i: nil\n", i);
        break;
      case LUA_TFUNCTION:
        printf("%3i: function\n", i);
        break;
      default:
        printf("%3i: %s\n", i, lua_typename(L, i));
    }
  }
  printf("\n");  /* end the listing */
}

/////////////  This is a dummy Lua module just to load all rubyk core cpp code //

// Register namespace
static const struct luaL_Reg lib_functions[] = {
  {NULL, NULL},
};

extern "C" int luaopen_rubyk_core(lua_State *L) {
  // register functions
  luaL_register(L, "rubyk", lib_functions);
  return 0;
}
