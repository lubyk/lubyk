/*************************************************
 LuaGL - an OpenGL binding for Lua
 2003-2004(c) Fabio Guerra, Cleyde Marlyse
 www.luagl.sourceforge.net

 See Copyright Notice in LuaGL.h
*************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
  #ifdef BUILDING_LUAGLUT_DLL
    #define LUAGLUT_API __declspec(dllexport)
  #else
    #define LUAGLUT_API __declspec(dllimport)
  #endif
  #include <windows.h>
#else
  #ifdef BUILDING_LUAGLUT_DLL
    #define LUAGLUT_API
  #else
    #define LUAGLUT_API extern
  #endif
#endif
#include <stdlib.h>

#ifdef __macosx__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#endif

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


LUAGLUT_API int luaopen_glut (lua_State *L);

#ifdef __cplusplus
}
#endif