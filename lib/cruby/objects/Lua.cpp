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
  void input(const Signal& sig)
  { 
    /* For now we only deal with integers. We will need to find a way to
     * pass other types and let the script decide. */
    sig.get(&mInput);
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
  INLET (Lua, input)
  OUTLET(Lua, output)
  METHOD_FOR_LUA(Lua, hello)
  SUPER_METHOD(Lua, Script, set)
  SUPER_METHOD(Lua, Script, load)
  SUPER_METHOD(Lua, Script, script)
}
