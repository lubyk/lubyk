#include "rubyk.h"

class Lua : public LuaScript {
  
};

extern "C" void init(Planet &planet) {
  CLASS (Lua, "Lua script.", "script: [script content] or file: [path to file]")
  // {1}
  c->add_method<Script, &Script::file>("file", StringIO("path", "Set path to script content."));
  // {2}
  c->add_method<Script, &Script::script>("script", StringIO("lua code", "Script content."));
  // {3}
  c->add_method<Script, &Script::reload>("reload", RealIO("seconds", "How often shoudl we check file for relaod."));
}