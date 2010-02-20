#ifndef RUBYK_SRC_LIB_LUA_H_
#define RUBYK_SRC_LIB_LUA_H_
extern "C" {
// we compiled Lua as C code
// not sure of that...
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
}

#endif