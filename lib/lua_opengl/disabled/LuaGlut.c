/*************************************************
*  LuaGL - an OpenGL binding for Lua
*  2003-2004(c) Fabio Guerra, Cleyde Marlyse
*  www.luagl.sourceforge.net
*-------------------------------------------------
*  Description: This file implements the GLUT
*               binding for Lua 5.0.
*-------------------------------------------------
*  Last Update: 14/07/2004
*  Version: v0.101
*-------------------------------------------------
*  See Copyright Notice in LuaGL.h
*************************************************/

#include "LuaGlut.h"
#define BUILDING_LUAGLUT_DLL

#ifdef _WIN32
#pragma warning(disable:4100)
#endif

typedef struct callbackFunc_typ {
  const char *name;
  lua_State *luastate;
} callbackFunc;


static callbackFunc displayFunc;
static callbackFunc keyboardFunc;
static callbackFunc reshapeFunc;
static callbackFunc idleFunc;

static void display_func(void)
{
   lua_getglobal(displayFunc.luastate, displayFunc.name);

   if(!lua_isfunction(displayFunc.luastate, -1))
      printf("Script error: cannot find %s function.\n\n", displayFunc.name);

   else if(lua_pcall(displayFunc.luastate, 0, 0, 0))
   {
      printf("Error running lua script:\n\n  %s \n\n", lua_tostring(displayFunc.luastate, -1));
      lua_pop(displayFunc.luastate, 1);
   }
}
static void keyboard_func(unsigned char key, int x, int y)
{
   lua_getglobal(keyboardFunc.luastate, keyboardFunc.name);

   if(!lua_isfunction(keyboardFunc.luastate, -1))
      printf("Script error: cannot find %s function.\n\n", keyboardFunc.name);
   else
   {
      lua_pushnumber(keyboardFunc.luastate, key);
      lua_pushnumber(keyboardFunc.luastate, x);
      lua_pushnumber(keyboardFunc.luastate, y);

      if(lua_pcall(keyboardFunc.luastate, 3, 0, 0))
      {
         printf("Error running lua script:\n\n  %s \n\n", lua_tostring(keyboardFunc.luastate, -1));
         lua_pop(keyboardFunc.luastate, 1);
      }
   }
}
static void reshape_func(int width, int height)
{
   lua_getglobal(reshapeFunc.luastate, reshapeFunc.name);

   if(!lua_isfunction(reshapeFunc.luastate, -1))
      printf("Script error: cannot find %s function.\n\n", reshapeFunc.name);

   else
   {
      lua_pushnumber(reshapeFunc.luastate, width);
      lua_pushnumber(reshapeFunc.luastate, height);

      if(lua_pcall(reshapeFunc.luastate, 2, 0, 0))
      {
         printf("Error running lua script:\n\n  %s \n\n", lua_tostring(reshapeFunc.luastate, -1));
         lua_pop(reshapeFunc.luastate, 1);
      }
   }
}

static void idle_func(void)
{
   lua_getglobal(idleFunc.luastate, idleFunc.name);

   if(!lua_isfunction(idleFunc.luastate, -1))
      printf("Script error: cannot find %s function.\n\n", idleFunc.name);

   else if(lua_pcall(idleFunc.luastate, 0, 0, 0))
   {
      printf("Error running lua script:\n\n  %s \n\n", lua_tostring(idleFunc.luastate, -1));
      lua_pop(idleFunc.luastate, 1);
   }
}
/*Init () -> none*/
static int glut_init(lua_State *L)
{
   int argc = 1;
   char *argv = (char *)malloc(sizeof(char));
   *argv = 0;

   glutInit(&argc, &argv);

   free(argv);
   return 0;
}

/*InitDisplayMode () -> none*/
static int glut_init_display_mode(lua_State *L)
{
   glutInitDisplayMode(GLUT_RGBA | GL_DOUBLE);
   return 0;
}

/*CreateWindow (title) -> none*/
static int glut_create_window(lua_State *L)
{
   /* test arguments type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'glut.CreateWindow'");

   /* call opengl function */
   glutCreateWindow((const char *)lua_tostring(L, 1));

   return 0;
}

/*InitWindowSize (width, height) -> none*/
static int glut_init_window_size(lua_State *L)
{
   /* test arguments type */
   if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
      luaL_error(L, "incorrect argument to function 'glut.InitWindowSize'");

   /* call opengl function */
   glutInitWindowSize((int)lua_tonumber(L, 1), (int)lua_tonumber(L, 2));

   return 0;
}

static int glut_display_func(lua_State *L)
{
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'glut.DisplayFunc'");
   displayFunc.name = lua_tostring(L, 1);
   displayFunc.luastate = L;

   glutDisplayFunc(display_func);
   return 0;
}
static int glut_idle_func(lua_State *L)
{
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'glut.IdleFunc'");
   idleFunc.name = lua_tostring(L, 1);
   idleFunc.luastate = L;

   glutIdleFunc(idle_func);
   return 0;
}
static int glut_keyboard_func(lua_State *L)
{
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'glut.KeyboardFunc'");
   keyboardFunc.name = lua_tostring(L, 1);
   keyboardFunc.luastate = L;

   glutKeyboardFunc(keyboard_func);
   return 0;
}
static int glut_reshape_func(lua_State *L)
{
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'glut.ReshapeFunc'");
   reshapeFunc.name = lua_tostring(L, 1);
   reshapeFunc.luastate = L;

   glutReshapeFunc(reshape_func);
   return 0;
}

static int glut_main_loop(lua_State *L)
{
   glutMainLoop();
   return 0;
}

static int glut_post_redisplay(lua_State *L)
{
   glutPostRedisplay();
   return 0;
}

static int glut_swap_buffers(lua_State *L)
{
   glutSwapBuffers();
   return 0;
}

static const luaL_reg glutlib[] = {
  {"CreateWindow", glut_create_window},
  {"DisplayFunc", glut_display_func},
  {"IdleFunc", glut_idle_func},
  {"Init", glut_init},
  {"InitDisplayMode", glut_init_display_mode},
  {"InitWindowSize", glut_init_window_size},
  {"KeyboardFunc", glut_keyboard_func},
  {"MainLoop", glut_main_loop},
  {"PostRedisplay", glut_post_redisplay},
  {"ReshapeFunc", glut_reshape_func},
  {"SwapBuffers", glut_swap_buffers},
  {NULL, NULL}
};

LUAGLUT_API int luaopen_glut (lua_State *L){
  luaL_openlib(L, "glut", glutlib, 0);
  return 1;
}