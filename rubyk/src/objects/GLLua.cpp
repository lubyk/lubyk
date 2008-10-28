#include "lua_script.h"

extern "C" {
#include <LuaGL.h>
#include <LuaGLUT.h>
}

class GLLua : public LuaScript
{
public:
  
  bool set (const Params& p)
  {
    return set_lua(p);
  }
  
  // inlet 1
  void bang(const Signal& sig)
  {
    mMutex.lock();
      // protected resource
      call_lua("bang", sig);
    mMutex.unlock();
  }
  
  // inlet 2
  void draw(const Signal& sig)
  { 
    if (!is_opengl_thread()) {
      *mOutput << mName << ": " << "not an openGL thread." << std::endl;
      return;
    }
    glPushMatrix();
    mMutex.lock();
      // protected resource
      call_lua("draw", sig);
    mMutex.unlock();
    glPopMatrix();
  }
  
  void in3(const Signal& sig)
  { set_lua_global("in3", sig); }
  
  void in4(const Signal& sig)
  { set_lua_global("in4", sig); }
  
  void in5(const Signal& sig)
  { set_lua_global("in5", sig);}
  
  void in6(const Signal& sig)
  { set_lua_global("in6", sig);}
  
  void in7(const Signal& sig)
  { set_lua_global("in7", sig);}
  
  void in8(const Signal& sig)
  { set_lua_global("in8", sig);}
  
  void in9(const Signal& sig)
  { set_lua_global("in9", sig);}
  
  void in10(const Signal& sig)
  { set_lua_global("in10", sig);}
  
private:
  
  /* open all standard libraries and openGL libraries (called by LuaScript on init) */
  void open_lua_libs()
  {
    open_base_lua_libs();
    open_opengl_lua_libs();
  }
  
  Mutex mMutex;
protected:
  
  /* open base lua libraries */
  void open_opengl_lua_libs()
  {
    open_lua_lib("opengl", luaopen_opengl);
    open_lua_lib("glut", luaopen_glut); 
  }
};

extern "C" void init()
{
  CLASS (GLLua)
  INLET (GLLua, draw)
  INLET (GLLua, in3)
  INLET (GLLua, in4)
  INLET (GLLua, in5)
  INLET (GLLua, in6)
  INLET (GLLua, in7)
  INLET (GLLua, in8)
  INLET (GLLua, in9)
  INLET (GLLua, in10)
  OUTLET(GLLua, out)
  OUTLET(GLLua, draw)
  OUTLET(GLLua, out3)
  OUTLET(GLLua, out4)
  OUTLET(GLLua, out5)
  OUTLET(GLLua, out6)
  OUTLET(GLLua, out7)
  OUTLET(GLLua, out8)
  OUTLET(GLLua, out9)
  OUTLET(GLLua, out10)
  SUPER_METHOD(GLLua, Script, load)
  SUPER_METHOD(GLLua, Script, script)
}
