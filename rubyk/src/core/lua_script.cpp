#include "lua_script.h"

extern "C" {
// we compiled Lua as C code
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#define RUBYK_THIS_IN_LUA "__this"

LuaScript::LuaScript() {
  // Our own lua context.
  lua_ = lua_open();
  
  // Load Lua main libraries
  luaL_openlibs(lua_);
  
  // push 'this' into the global field '__this'
  lua_pushlightuserdata(lua_, (void*)this);
  lua_setglobal(lua_, "__this");
}

LuaScript::~LuaScript() {
  lua_close(lua_);
}

const Value LuaScript::eval_script() {
  int status;
  
  /* set 'current_time' */
  lua_pushnumber(lua_, worker_->current_time_);
  lua_setglobal(lua_, "current_time");
  
  // compile script
  status = luaL_loadbuffer(lua_, script_.c_str(), script_.size(), name_.c_str());
  if (status) {
    return Value(BAD_REQUEST_ERROR, std::string(lua_tostring(lua_, -1)).append("."));
  }
  
  // Run the script to create the functions.
  status = lua_pcall(lua_, 0, 0, 0); // 0 arg, 1 result, no error function
  if (status) {
    // TODO: proper error reporting
      return Value(BAD_REQUEST_ERROR, std::string(lua_tostring(lua_, -1)).append("."));
  }
  // ok, we can receive and process values (again).
  return Value(script_);
}
