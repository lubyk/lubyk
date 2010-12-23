#include "zmq/constants.h"

#include "lua_cpp_helper.h"

using namespace zmq;



// Register namespace

static const struct luaL_Reg zmq_functions[] = {

  {NULL, NULL},
};


static const struct lua_constants_Reg zmq_namespace_constants[] = {
  {"SUB"                           , zmq::SUB},
  {"PULL"                          , zmq::PULL},
  {"PUB"                           , zmq::PUB},
  {"PUSH"                          , zmq::PUSH},
  {"SUBSCRIBE"                     , zmq::SUBSCRIBE},
  {"UNSUBSCRIBE"                   , zmq::UNSUBSCRIBE},
  {NULL, NULL},
};


#ifdef DUB_LUA_NO_OPEN
int luaload_zmq_constants(lua_State *L) {
#else
extern "C" int luaopen_zmq_constants(lua_State *L) {
#endif
  // register functions
  luaL_register(L, "zmq", zmq_functions);


  // register namespace enums
  register_constants(L, "zmq", zmq_namespace_constants);

  return 0;
}
