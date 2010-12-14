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
#include "mdns/browser.h"

class LuaBrowser : public mdns::Browser
{
public:
  LuaBrowser(Worker *worker, const char *service_type, int func_idx) : mdns::Browser(service_type)
    worker_(worker), func_idx_(func_idx) {
    start();
  }

  ~LuaBrowser() {
    // release function
    stop();
    luaL_unref(worker_->lua_, LUA_REGISTRYINDEX, func_idx_);
  }

  virtual void add_device(const Location &location) {
    lua_State *L = worker_->lua_;
    ScopedLock lock(worker_);
    lua_rawgeti(L, LUA_REGISTRYINDEX, func_idx_);
    lua_pushstring(L, "add");
    lua_pushstring(L, location.name().c_str());
    int status = lua_pcall(L, 1, 0, 0);

    if (status) {
      printf("Error in add_device: %s\n", lua_tostring(L, -1));
    }
    // clear stack
    lua_settop(worker_->lua_, 0);
  }

  virtual void remove_device(const char *name) {
    lua_State *L = worker_->lua_;
    ScopedLock lock(worker_);
    lua_rawgeti(L, LUA_REGISTRYINDEX, func_idx_);
    lua_pushstring(L, "remove");
    lua_pushstring(L, name);
    int status = lua_pcall(L, 1, 0, 0);

    if (status) {
      printf("Error in remove_device: %s\n", lua_tostring(L, -1));
    }
    // clear stack
    lua_settop(worker_->lua_, 0);
  }
private:
  Worker *worker_;
  int func_idx_;
};

static int LuaBrowser_destructor(lua_State *L) {
  LuaBrowser **userdata = (LuaBrowser**)luaL_checkudata(L, 1, "mdns.Browser");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

static int LuaBrowser_LuaBrowser(lua_State *L) {
  const char *service_type = luaL_checkstring(L, 1);
  luaL_checktype(L, 2, LUA_TFUNCTION);
  lua_pushvalue( L, -1 );
  int func_idx = luaL_ref(L, LUA_REGISTRYINDEX);
  LuaBrowser *retval__ = new LuaBrowser(gWorker, service_type, func_idx);
  lua_pushclass<LuaBrowser>(L, retval__, "mdns.Browser");
  return 1;
}

/* ============================ Lua Registration ====================== */

static const struct luaL_Reg LuaBrowser_member_methods[] = {
  {"__gc"              , LuaBrowser_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg LuaBrowser_namespace_methods[] = {
  {"Browser"           , LuaBrowser_LuaBrowser},
  {NULL, NULL},
};


extern "C" int luaopen_mdns_Browser(lua_State *L) {
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mdns.Browser");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, LuaBrowser_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "mdns", LuaBrowser_namespace_methods);
  return 0;
}
