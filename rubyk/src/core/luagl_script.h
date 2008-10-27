#ifndef _LUAGL_SCRIPT_
#define _LUAGL_SCRIPT_
#include "lua_script.h"

extern "C" {
// we compiled as C code
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <LuaGL.h>
#include <LuaGLUT.h>
}

#define LUA_RETURN_BUFFER_SIZE 32
#define LUA_MATRIX_NAME "Matrix"

class LuaGLScript : public LuaScript
{
public:
  
private:
  /* open all standard libraries and openGL libraries */
  virtual void open_lua_libs ();
  
protected:
  
  /* open base lua libraries */
  void open_opengl_lua_libs ();
};

#endif // _LUAGL_SCRIPT_