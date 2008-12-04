#include "lua_script.h"
#include <stdlib.h>

class System : public LuaScript
{
public:
  
  bool set (const Params& p)
  {
    return set_lua(p);
  }
  
  
  bool eval_script(const std::string& pScript) 
  {
    if (pScript.find("function bang()") != std::string::npos) {
      mScript = pScript;
    } else {
      mScript = std::string("function bang()\nsystem([[").append(pScript).append("\n]])\nend\n");
    }
    
    return eval_lua_script(mScript);
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
  
  
  /** Execute an applescript from lua. */
  int sys_system()
  { 
    if (string_from_lua(&mSystCall)) {
      NEW_THREAD(System, do_system)
    }
    return 0;
  }
  
  void do_system()
  {  
    system(mSystCall.c_str());
  }
private:
  std::string mSystCall; // FIXME: this is not thread safe !
};

extern "C" void init()
{
  CLASS (System)
  INLET (System, in2)
  INLET (System, in3)
  INLET (System, in4)
  INLET (System, in5)
  INLET (System, in6)
  INLET (System, in7)
  INLET (System, in8)
  INLET (System, in9)
  INLET (System, in10)
  OUTLET(System, out)
  OUTLET(System, out2)
  OUTLET(System, out3)
  OUTLET(System, out4)
  OUTLET(System, out5)
  OUTLET(System, out6)
  OUTLET(System, out7)
  OUTLET(System, out8)
  OUTLET(System, out9)
  OUTLET(System, out10)
  Class::find("System")->add_method_for_lua("system", &LuaScript::cast_method_for_lua<System, &System::sys_system>);
  SUPER_METHOD(System, Script, load)
  SUPER_METHOD(System, Script, script)
}
