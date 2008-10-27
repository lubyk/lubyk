/*************************************************
*  LuaGL - an OpenGL binding for Lua
*  2003-2004(c) Fabio Guerra, Cleyde Marlyse
*  www.luagl.sourceforge.net
*-------------------------------------------------
*  Description: This file is a simple example of
*               a Lua interpreter that uses LuaGL.
*-------------------------------------------------
*  Last Update: 14/07/2003
*  Version: v1.0
*-------------------------------------------------
*  See Copyright Notice in LuaGL.h
*************************************************/

#ifdef _WIN32
  #include <windows.h>
#endif

#include <GL/gl.h>
#include <glut.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <LuaGL.h>
#include <LuaGLUT.h>

// main function
int main(int argc, char *argv[])
{
   lua_State *L = lua_open();

   luaopen_base(L);
   luaopen_table(L);
   luaopen_io(L);
   luaopen_string(L);
   luaopen_math(L);
   luaopen_opengl(L);
   luaopen_glut(L);

   if (argc < 2)
   {
      printf("\n usage: luagl.exe filename.lua \n\n");
      return -1;
   }
   if(luaL_loadfile(L, argv[1]) || lua_pcall(L, 0, 0, 0))
   {
      printf("Error running lua script:\n\n  %s \n\n", lua_tostring(L, -1));
      lua_pop(L, 1);
      return -2;
   }

   return 0;
}
