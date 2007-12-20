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
    call_lua("bang");
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
    double d;
    const Matrix * live;
    if (sig.get(&live)) {
      lua_pushmatrix(*live);
      lua_setglobal(mLua, key);
    } else if (sig.get(&d)) {
      lua_pushnumber(mLua, d);
      lua_setglobal(mLua, key);
    } else if (sig.type && sig.type != BangSignal) {
      *mOutput << mName << ": cannot set '" << key << "' to " << sig << " (type not yet suported in Lua).\n";
    }
  }
};

extern "C" void init()
{
  CLASS (Lua)
  INLET (Lua, input2)
  INLET (Lua, input3)
  INLET (Lua, input4)
  OUTLET(Lua, output)
  OUTLET(Lua, output2)
  OUTLET(Lua, output3)
  OUTLET(Lua, output4)
  SUPER_METHOD(Lua, Script, load)
  SUPER_METHOD(Lua, Script, script)
}
