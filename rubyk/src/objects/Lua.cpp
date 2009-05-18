#include "rubyk.h"

class Lua : public LuaScript {
};

extern "C" void init(Planet &planet) {
  CLASS(Lua, "Lua script.", "script: [script content] or file: [path to file]")
  // {1}
  ADD_SUPER_METHOD(Lua, Script, file, StringIO("path", "Set path to script content."))
  // {2}
  ADD_SUPER_METHOD(Lua, Script, script, StringIO("lua code", "Script content."))
  // {3}
  ADD_SUPER_METHOD(Lua, Script, reload, RealIO("seconds", "How often shoudl we check file for relaod."))
}