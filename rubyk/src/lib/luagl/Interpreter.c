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

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <LuaGL.h>
#include <LuaGLUT.h>

static lua_State *L;

static void  
open_lua_lib(lua_State* L, const char* name, lua_CFunction fn)  
{  
    lua_pushcfunction(L, fn) ;  
    lua_pushstring(L, name) ;  
    lua_call(L, 1, 0) ;  
}

static void
draw_frame()
{
  
  lua_getglobal(L, "DrawFrame"); /* function to be called */
  
  /* Run the function. */
  int status = lua_pcall(L, 0, 1, 0); // 1 arg, 1 result, no error function
  if (status) {
    printf("could not call DrawFrame\n");
    printf("%s\n", lua_tostring(L, -1));
    return;
  }
}

static void
reshape()
{
  
  lua_getglobal(L, "Reshape"); /* function to be called */
  
  /* Run the function. */
  int status = lua_pcall(L, 0, 1, 0); // 1 arg, 1 result, no error function
  if (status) {
    printf("could not call DrawFrame\n");
    printf("%s\n", lua_tostring(L, -1));
    return;
  }
}

// main function
int main(int argc, char *argv[])
{
   L = lua_open();

   open_lua_lib(L, "", luaopen_base);
   open_lua_lib(L, LUA_TABLIBNAME, luaopen_table);
   open_lua_lib(L, LUA_IOLIBNAME, luaopen_io);
   open_lua_lib(L, LUA_STRLIBNAME, luaopen_string);
   open_lua_lib(L, LUA_MATHLIBNAME, luaopen_math);
   open_lua_lib(L, "opengl", luaopen_opengl);
   open_lua_lib(L, "glut", luaopen_glut);

   if (argc < 2)
   {
#ifdef _WIN32
      printf("\n usage: luagl.exe filename.lua \n\n");
#else
      printf("\n usage: luagl filename.lua \n\n");
#endif
      return -1;
   }
   if(luaL_loadfile(L, argv[1]) || lua_pcall(L, 0, 0, 0))
   {
      printf("Error running lua script:\n\n  %s \n\n", lua_tostring(L, -1));
      lua_pop(L, 1);
      return -2;
   }
   
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB);
   glutInitWindowSize(640, 480);
   int win = glutCreateWindow("luagl test");
   
   glEnable(GL_POINT_SMOOTH);
   glEnable(GL_BLEND); //Enable alpha blending
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blend function
   
   // current window is win
   glutDisplayFunc(&draw_frame);

   glutMainLoop();

   return 0;
}
