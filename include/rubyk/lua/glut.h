#ifndef RUBYK_INCLUDE_RUBYK_LUA_GLUT_H_
#define RUBYK_INCLUDE_RUBYK_LUA_GLUT_H_

#include "rubyk/lua/lua.h"

extern "C" {
int luaopen_glut(lua_State *L);
int luaopen_rubyk(lua_State *L);
}

#endif // RUBYK_INCLUDE_RUBYK_LUA_GLUT_H_
