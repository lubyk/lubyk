#include "rubyk.h"

/////////////  This is a dummy Lua module just to load all mdns core cpp code //

// Register namespace
static const struct luaL_Reg lib_functions[] = {
  {"core", NULL},
  {NULL, NULL},
};

extern "C" int luaopen_mdns_core(lua_State *L) {
  // register functions
  luaL_register(L, "mdns", lib_functions);
  return 0;
}
