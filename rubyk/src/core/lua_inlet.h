#ifndef RUBYK_SRC_CORE_LUA_INLET_H_
#define RUBYK_SRC_CORE_LUA_INLET_H_
#include "inlet.h"

class LuaInlet : public Inlet {
public:
  LuaInlet(LuaScript *node, const char *name, const Value &type) :
      Inlet(node, name, &LuaInlet::receive_method, type) {}
  
  static void receive_method(Node *receiver, const Value &val) {
    (((LuaScript*)receiver)->call_lua)(receiver->name().c_str(), val);
  }
};

#endif // RUBYK_SRC_CORE_LUA_INLET_H_