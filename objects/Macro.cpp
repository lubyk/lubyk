#include "lua_script.h"
#include "command.h"

/*** Lua script with macro capability (can execute rubyk commands). */
class Macro : public LuaScript
{
public:
  
  bool init (const Params& p)
  {
    mCmd.set_output(std::cout);
    mCmd.set_input(std::cin);
    mCmd.set_server(*mServer);
    mCmd.set_silent(true);
    return true;
  }
  
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
  
  /** Execute macro from lua. */
  int parse_command()
  {
    std::string str;
    if (string_from_lua(&str)) {
      // server is currently locked by the execution of the Lua script calling parse. We must unlock.
      mServer->unlock();
      mCmd.parse(str);
      // lock back before going back into lua.
      mServer->lock();
    }
    return 0;
  }
  
private:
  Command mCmd;        /**< Command parser. */
};

extern "C" void init()
{
  CLASS (Macro)
  INLET (Macro, in2)
  INLET (Macro, in3)
  INLET (Macro, in4)
  INLET (Macro, in5)
  INLET (Macro, in6)
  INLET (Macro, in7)
  INLET (Macro, in8)
  INLET (Macro, in9)
  INLET (Macro, in10)
  OUTLET(Macro, out)
  OUTLET(Macro, out2)
  OUTLET(Macro, out3)
  OUTLET(Macro, out4)
  OUTLET(Macro, out5)
  OUTLET(Macro, out6)
  OUTLET(Macro, out7)
  OUTLET(Macro, out8)
  OUTLET(Macro, out9)
  OUTLET(Macro, out10)
  METHOD_FOR_LUA(Macro, parse_command)
  SUPER_METHOD(Macro, Script, load)
  SUPER_METHOD(Macro, Script, script)
}
