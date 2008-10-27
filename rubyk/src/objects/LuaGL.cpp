#include "lua_script.h"

extern "C" {
// we compiled as C code
#include <LuaGL.h>
#include <LuaGLUT.h>
}

class LuaGL : public LuaScript
{
public:
  
  bool set (const Params& p)
  {
    return set_lua(p);
  }
  
  // inlet 1
  void bang(const Signal& sig)
  {
    set_lua_global("in1", sig);
    call_lua(&mS, "bang");
    if (mS.type) send(mS);
  }
  
  void in2(const Signal& sig)
  { set_lua_global("in2", sig); }
  
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
  CLASS (LuaGL)
  INLET (LuaGL, in2)
  INLET (LuaGL, in3)
  INLET (LuaGL, in4)
  INLET (LuaGL, in5)
  INLET (LuaGL, in6)
  INLET (LuaGL, in7)
  INLET (LuaGL, in8)
  INLET (LuaGL, in9)
  INLET (LuaGL, in10)
  OUTLET(LuaGL, out)
  OUTLET(LuaGL, out2)
  OUTLET(LuaGL, out3)
  OUTLET(LuaGL, out4)
  OUTLET(LuaGL, out5)
  OUTLET(LuaGL, out6)
  OUTLET(LuaGL, out7)
  OUTLET(LuaGL, out8)
  OUTLET(LuaGL, out9)
  OUTLET(LuaGL, out10)
  SUPER_METHOD(LuaGL, Script, load)
  SUPER_METHOD(LuaGL, Script, script)
}
