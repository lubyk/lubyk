#include "lua_script.h"

class Lua : public LuaScript
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
  
  // inlet 2
  void input2(const Signal& sig)
  { 
    set_lua_global("in2", sig);
  }
  
  // inlet 3
  void input3(const Signal& sig)
  { 
    set_lua_global("in3", sig);
  }
  
  // inlet 4
  void input4(const Signal& sig)
  { 
    set_lua_global("in4", sig);
  }
  
  // inlet 5
  void input5(const Signal& sig)
  { 
    set_lua_global("in5", sig);
  }
};

extern "C" void init()
{
  CLASS (Lua)
  INLET (Lua, input2)
  INLET (Lua, input3)
  INLET (Lua, input4)
  INLET (Lua, input5)
  OUTLET(Lua, output)
  OUTLET(Lua, output2)
  OUTLET(Lua, output3)
  OUTLET(Lua, output4)
  OUTLET(Lua, output5)
  SUPER_METHOD(Lua, Script, load)
  SUPER_METHOD(Lua, Script, script)
}
