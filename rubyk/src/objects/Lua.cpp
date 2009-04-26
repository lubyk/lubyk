#include "lua_script.h"

class Lua : public LuaScript
{
public:
  // inlet 1
  void bang(const Value &val) {
    call_lua("bang", val);
  }
};

extern "C" void init(Planet &planet) {
  CLASS (Lua, "Lua script.", "script: [script content] or file: [path to file]")
  // [1]
  INLET( Lua, bang, "Execute the bang method in lua.", RealIO("any", "Any real value."))
  // {1}, [2]
  c->add_method<Lua, &Script::file>("file", StringIO("path", "Set path to script content."));
  c->add_inlet<Lua, &Script::file>( "file", StringIO("path", "Set path to script content."));
  // {2}, [3]
  c->add_method<Lua, &Script::script>("script", StringIO("lua code", "Script content."));
  c->add_inlet<Lua, &Script::script>("script", StringIO("lua code", "Script content."));
  // {3}, [4]
  c->add_method<Lua, &Script::reload>("reload", RealIO("seconds", "How often shoudl we check file for relaod."));
  c->add_inlet<Lua, &Script::reload>("reload", RealIO("seconds", "How often shoudl we check file for relaod."));
  // {4}
  ADD_METHOD(Lua, bang, "Execute the bang method in lua.", )
  OUTLET(Lua, out, RealIO("any", "Any real value."))
}
