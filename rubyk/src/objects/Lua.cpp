#include "lua_script.h"

class Lua : public LuaScript
{
public:
  
  bool set (const Value& p)
  {
    return set_lua(p);
  }
  
  // inlet 1
  void bang(const Value& sig)
  {
    call_lua("bang", sig);
  }
  
  void in2(const Value& sig)
  { set_lua_global("in2", sig); }
  
  void in3(const Value& sig)
  { set_lua_global("in3", sig); }
  
  void in4(const Value& sig)
  { set_lua_global("in4", sig); }
  
  void in5(const Value& sig)
  { set_lua_global("in5", sig);}
  
  void in6(const Value& sig)
  { set_lua_global("in6", sig);}
  
  void in7(const Value& sig)
  { set_lua_global("in7", sig);}
  
  void in8(const Value& sig)
  { set_lua_global("in8", sig);}
  
  void in9(const Value& sig)
  { set_lua_global("in9", sig);}
  
  void in10(const Value& sig)
  { set_lua_global("in10", sig);}
};

extern "C" void init()
{
  CLASS (Lua)
  INLET (Lua, in2)
  INLET (Lua, in3)
  INLET (Lua, in4)
  INLET (Lua, in5)
  INLET (Lua, in6)
  INLET (Lua, in7)
  INLET (Lua, in8)
  INLET (Lua, in9)
  INLET (Lua, in10)
  OUTLET(Lua, out)
  OUTLET(Lua, out2)
  OUTLET(Lua, out3)
  OUTLET(Lua, out4)
  OUTLET(Lua, out5)
  OUTLET(Lua, out6)
  OUTLET(Lua, out7)
  OUTLET(Lua, out8)
  OUTLET(Lua, out9)
  OUTLET(Lua, out10)
  SUPER_METHOD(Lua, Script, load)
  SUPER_METHOD(Lua, Script, script)
}
