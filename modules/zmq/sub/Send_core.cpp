#include "zmq/Send.h"

#include "lua_cpp_helper.h"


using namespace zmq;


/* ============================ Constructors     ====================== */

/** zmq::Send::Send(rubyk::Worker *worker, const char *location)
 * include/zmq/Send.h:58
 */
static int Send_Send(lua_State *L) {
  try {
    rubyk::Worker *worker = *((rubyk::Worker **)luaL_checkudata(L, 1, "rubyk.Worker"));
    const char *location = luaL_checkstring(L, 2);
    Send * retval__ = new Send(worker, location);
    lua_pushclass<Send>(L, retval__, "zmq.Send");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.Send.Send: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Send.Send: Unknown exception");
    lua_error(L);
    return 0;
  }
}

/* ============================ Destructor       ====================== */

static int Send_destructor(lua_State *L) {
  Send **userdata = (Send**)luaL_checkudata(L, 1, "zmq.Send");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Send__tostring(lua_State *L) {
  Send **userdata = (Send**)luaL_checkudata(L, 1, "zmq.Send");
  
  lua_pushfstring(L, "<zmq.Send: %p %s>", *userdata, (*userdata)->location());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void zmq::Send::send(const char *message)
 * include/zmq/Send.h:72
 */
static int Send_send(lua_State *L) {
  try {
    Send *self__ = *((Send**)luaL_checkudata(L, 1, "zmq.Send"));
    lua_remove(L, 1);
    const char *message = luaL_checkstring(L, 1);
    self__->send(message);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.Send.send: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Send.send: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Send_member_methods[] = {
  {"send"              , Send_send},
  {"__tostring"        , Send__tostring},
  {"__gc"              , Send_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Send_namespace_methods[] = {
  {"Send"              , Send_Send},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_zmq_Send_core(lua_State *L) {
#else
extern "C" int luaopen_zmq_Send_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "zmq.Send");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Send_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "zmq", Send_namespace_methods);


	return 1;
}
