#ifndef RUBYK_SRC_CORE_LUA_INLET_H_
#define RUBYK_SRC_CORE_LUA_INLET_H_
#include "inlet.h"

class LuaScript;

class LuaInlet : public Inlet {
public:
  LuaInlet(LuaScript *node, const char *name, const Value &type);

  static void receive_method(Inlet *inlet, const Value &val);
};

#endif // RUBYK_SRC_CORE_LUA_INLET_H_