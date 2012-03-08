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
#include "lua/dump.h"

static void print_lua_value(lua_State *L, int i, bool inspect_tables, int flags = 3) {
    // print initial number
  if (flags & 1) {
    printf("%3i: ", i);
  } else if (flags == 0) {
    // print key
    printf("     ");
  }

  switch (lua_type(L, i)) {
    case LUA_TSTRING:
      if (flags & 2) {
        printf("'%s'", lua_tostring(L, i));
      } else {
        // printing key
        printf("  %-12s", lua_tostring(L, i));
      }
      break;
    case LUA_TBOOLEAN:
      printf("%s", lua_toboolean(L, i) ? "true" : "false");
      break;
    case LUA_TNUMBER:
      printf("%g", lua_tonumber(L, i));
      break;
    case LUA_TTABLE:
      if (inspect_tables) {
        printf("{\n");
        lua_pushvalue(L, i);
        for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1)) {
          // inspect table content
          print_lua_value(L, -2, false, 0);
          print_lua_value(L, -1, false, 2);
        }
        lua_pop(L, 1);
        printf("     }");
      } else {
        printf("{}");
      }

      break;
    case LUA_TNONE:
      printf("--");
      break;
    case LUA_TNIL:
      printf("nil");
      break;
    default:
      printf("%s:%p", luaL_typename(L,i), lua_topointer(L,i));
  }
  if (flags & 2) {
    // printing value
    printf("\n");
  } else {
    // printing key
    printf(" = ");
  }
}

void luaDump(lua_State *L, const char *msg, bool inspect_tables) {
  int i;
  int top = lua_gettop(L);

  printf("%s (top: %d)\n", msg, top);

  for (i = 1; i <= top; ++i) {
    print_lua_value(L, i, inspect_tables);
  }
  printf("\n");  /* end the listing */
}

