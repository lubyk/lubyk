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
#ifndef LUBYK_INCLUDE_LK_CONSTANTS_H_
#define LUBYK_INCLUDE_LK_CONSTANTS_H_

#include "lubyk/lua.h"

#include <string.h>

namespace lk {

// Taken from loadlib.c (Lua 5.1)
static int readable (const char *filename) {
  FILE *f = fopen(filename, "r");  /* try to open file */
  if (f == NULL) return 0;  /* open failed */
  fclose(f);
  return 1;
}

// Taken from loadlib.c (Lua 5.1)
static const char *pushnexttemplate (lua_State *L, const char *path) {
  const char *l;
  while (*path == *LUA_PATHSEP) path++;  /* skip separators */
  if (*path == '\0') return NULL;  /* no more templates */
  l = strchr(path, *LUA_PATHSEP);  /* find next separator */
  if (l == NULL) l = path + strlen(path);
  lua_pushlstring(L, path, l - path);  /* template */
  return l;
}


// <class_name>
// Taken from loadlib.c (Lua 5.1)
LuaStackSize findfile(lua_State *L) {
  const char *name  = luaL_checkstring(L, 1);
  const char *pname = "path";
  const char *path;
  name = luaL_gsub(L, name, ".", LUA_DIRSEP);
  lua_getfield(L, LUA_ENVIRONINDEX, pname);
  path = lua_tostring(L, -1);
  if (path == NULL)
    luaL_error(L, LUA_QL("package.%s") " must be a string", pname);
  lua_pushliteral(L, "");  /* error accumulator */
  while ((path = pushnexttemplate(L, path)) != NULL) {
    const char *filename;
    filename = luaL_gsub(L, lua_tostring(L, -1), LUA_PATH_MARK, name);
    lua_remove(L, -2);  /* remove path template */
    if (readable(filename)) { /* does file exist and is readable? */
      lua_pushstring(L, filename);
      return 1;
    }
    lua_pushfstring(L, "\n\tno file " LUA_QS, filename);
    lua_remove(L, -2);  /* remove file name */
    lua_concat(L, 2);  /* add entry to possible error message */
  }
  return 0;  /* not found */
}

} // lk

#endif // LUBYK_INCLUDE_LK_CONSTANTS_H_