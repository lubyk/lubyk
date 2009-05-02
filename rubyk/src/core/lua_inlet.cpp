#include "lua_inlet.h"
#include "lua_script.h"


LuaInlet::LuaInlet(LuaScript *node, const char *name, const Value &type) :
    Inlet(static_cast<Node*>(node), name, &LuaInlet::receive_method, type) {}

void LuaInlet::receive_method(Inlet *inlet, const Value &val) {
  ((LuaScript*)inlet->node())->call_lua(inlet->name().c_str(), val);
}