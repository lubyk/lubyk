#include "zmq/SendSocket.h"

#include "lua_cpp_helper.h"


using namespace zmq;


/* ============================ Constructors     ====================== */

/** zmq::SendSocket::SendSocket(int type)
 * include/zmq/SendSocket.h:58
 */
static int SendSocket_SendSocket(lua_State *L) {
  try {
    int type = luaL_checkint(L, 1);
    SendSocket * retval__ = new SendSocket(type);
    lua_pushclass<SendSocket>(L, retval__, "zmq.SendSocket");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.SendSocket.SendSocket: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.SendSocket.SendSocket: Unknown exception");
    lua_error(L);
    return 0;
  }
}

/* ============================ Destructor       ====================== */

static int SendSocket_destructor(lua_State *L) {
  SendSocket **userdata = (SendSocket**)luaL_checkudata(L, 1, "zmq.SendSocket");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int SendSocket__tostring(lua_State *L) {
  SendSocket **userdata = (SendSocket**)luaL_checkudata(L, 1, "zmq.SendSocket");
  
  lua_pushfstring(L, "<zmq.SendSocket: %p %s>", *userdata, (*userdata)->location());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void zmq::SendSocket::bind(const char *location)
 * include/zmq/SendSocket.h:69
 */
static int SendSocket_bind(lua_State *L) {
  try {
    SendSocket *self__ = *((SendSocket**)luaL_checkudata(L, 1, "zmq.SendSocket"));
    lua_remove(L, 1);
    const char *location = luaL_checkstring(L, 1);
    self__->bind(location);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.SendSocket.bind: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.SendSocket.bind: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void zmq::SendSocket::send(lua_State *L)
 * include/zmq/SendSocket.h:74
 */
static int SendSocket_send(lua_State *L) {
  try {
    SendSocket *self__ = *((SendSocket**)luaL_checkudata(L, 1, "zmq.SendSocket"));
    lua_remove(L, 1);
    
    self__->send(L);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.SendSocket.send: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.SendSocket.send: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg SendSocket_member_methods[] = {
  {"bind"              , SendSocket_bind},
  {"send"              , SendSocket_send},
  {"__tostring"        , SendSocket__tostring},
  {"__gc"              , SendSocket_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg SendSocket_namespace_methods[] = {
  {"SendSocket"        , SendSocket_SendSocket},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_zmq_SendSocket(lua_State *L) {
#else
extern "C" int luaopen_zmq_SendSocket(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "zmq.SendSocket");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, SendSocket_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "zmq", SendSocket_namespace_methods);


	return 1;
}
