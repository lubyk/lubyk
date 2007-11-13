#include "lua_script.h"

class Lua : public LuaScript
{
public:
  
  bool init (const Params& p)
  {
    mInput = 0;
    return init_lua(p);
  }
  
  // inlet 1
  void input1(const Signal& sig)
  { 
    set_lua_global("in1", sig);
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
  
  void set_lua_global(const char * key, const Signal& sig)
  {
    float f;
    if (sig.get(&f)) {
      lua_pushnumber(mLua, f);
      lua_setglobal(mLua, key);
    }
  }
  
  // outlet 1
  void output(Signal& sig)
  {
    call_lua("update", sig, mInput);
  }
  
  int hello()
  {
    printf("hello lua !\n");
    return 0;
  }
  
private:
  float mInput;
};

extern "C" void init()
{
  CLASS (Lua)
  INLET (Lua, input1)
  INLET (Lua, input2)
  INLET (Lua, input3)
  INLET (Lua, input4)
  OUTLET(Lua, output)
  METHOD_FOR_LUA(Lua, hello)
  SUPER_METHOD(Lua, Script, set)
  SUPER_METHOD(Lua, Script, load)
  SUPER_METHOD(Lua, Script, script)
}
