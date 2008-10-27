#include "luagl_script.h"

void LuaGLScript::open_lua_libs()
{
  open_base_lua_libs();
  open_opengl_lua_libs();
}

void LuaGLScript::open_opengl_lua_libs()
{
  open_lua_lib("opengl", luaopen_opengl);
  open_lua_lib("glut", luaopen_glut); 
}