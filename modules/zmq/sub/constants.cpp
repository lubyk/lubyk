#include "zmq/constants.h"

#include "lua_cpp_helper.h"

using namespace zmq;



// Register namespace

static const struct luaL_Reg zmq_functions[] = {

  {NULL, NULL},
};


static const struct lua_constants_Reg zmq_namespace_constants[] = {
  {"PAIR"                          , zmq::PAIR},
  {"PUB"                           , zmq::PUB},
  {"SUB"                           , zmq::SUB},
  {"REQ"                           , zmq::REQ},
  {"REP"                           , zmq::REP},
  {"XREQ"                          , zmq::XREQ},
  {"XREP"                          , zmq::XREP},
  {"PULL"                          , zmq::PULL},
  {"PUSH"                          , zmq::PUSH},
  {"HWM"                           , zmq::HWM},
  {"SWAP"                          , zmq::SWAP},
  {"AFFINITY"                      , zmq::AFFINITY},
  {"IDENTITY"                      , zmq::IDENTITY},
  {"SUBSCRIBE"                     , zmq::SUBSCRIBE},
  {"UNSUBSCRIBE"                   , zmq::UNSUBSCRIBE},
  {"RATE"                          , zmq::RATE},
  {"RECOVERY_IVL"                  , zmq::RECOVERY_IVL},
  {"MCAST_LOOP"                    , zmq::MCAST_LOOP},
  {"SNDBUF"                        , zmq::SNDBUF},
  {"RCVBUF"                        , zmq::RCVBUF},
  {"RCVMORE"                       , zmq::RCVMORE},
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
