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

static int LuaTimer_destructor(lua_State *L) {
  LuaTimer **userdata = (LuaTimer**)luaL_checkudata(L, 1, "rk.Timer");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

static int LuaTimer_LuaTimer(lua_State *L) {
  float interval = luaL_checknumber(L, 1);
  if (interval < 0) {
    luaL_error(L, "Timer interval must be a positive value (got %f)\n", interval);
  }
  luaL_checktype(L, 2, LUA_TFUNCTION);
  // remove interval from stack
  lua_pushvalue( L, -1 );
  int func_idx = luaL_ref(L, LUA_REGISTRYINDEX);
  LuaTimer *retval__ = new LuaTimer(gWorker, interval, func_idx);
  lua_pushclass<LuaTimer>(L, retval__, "rk.Timer");
  return 1;
}

static int LuaTimer_start(lua_State *L) {
  LuaTimer *self__ = *((LuaTimer**)luaL_checkudata(L, 1, "rk.Timer"));
  self__->start();
  return 0;
}

static int LuaTimer_stop(lua_State *L) {
  LuaTimer *self__ = *((LuaTimer**)luaL_checkudata(L, 1, "rk.Timer"));
  self__->stop();
  return 0;
}

static int LuaTimer_join(lua_State *L) {
  LuaTimer *self__ = *((LuaTimer**)luaL_checkudata(L, 1, "rk.Timer"));
  self__->join();
  return 0;
}
/* ============================ Lua Registration ====================== */

static const struct luaL_Reg LuaTimer_member_methods[] = {
  {"start"             , LuaTimer_start},
  {"stop"              , LuaTimer_stop},
  {"join"              , LuaTimer_join},
  {"__gc"              , LuaTimer_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg LuaTimer_namespace_methods[] = {
  {"Timer"             , LuaTimer_LuaTimer},
  {NULL, NULL},
};


extern "C" int luaopen_rk_Timer(lua_State *L) {
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "rk.Timer");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, LuaTimer_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "rk", LuaTimer_namespace_methods);
  return 0;
}
