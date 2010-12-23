#include "zmq/ReceiveSocket.h"

#include "lua_cpp_helper.h"


using namespace zmq;


/* ============================ Constructors     ====================== */

/** zmq::ReceiveSocket::ReceiveSocket(rubyk::Worker *worker, int type, int lua_func_idx)
 * include/zmq/ReceiveSocket.h:57
 */
static int ReceiveSocket_ReceiveSocket(lua_State *L) {
  try {
    rubyk::Worker *worker = *((rubyk::Worker **)luaL_checkudata(L, 1, "rubyk.Worker"));
    int type = luaL_checkint(L, 2);
    
    luaL_checktype(L, 3, LUA_TFUNCTION);
    // push on top
    lua_pushvalue(L, 3);
    int lua_func_idx = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_pop(L, 1);
    
    ReceiveSocket * retval__ = new ReceiveSocket(worker, type, lua_func_idx);
    lua_pushclass<ReceiveSocket>(L, retval__, "zmq.ReceiveSocket");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.ReceiveSocket.ReceiveSocket: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.ReceiveSocket.ReceiveSocket: Unknown exception");
    lua_error(L);
    return 0;
  }
}

/* ============================ Destructor       ====================== */

static int ReceiveSocket_destructor(lua_State *L) {
  ReceiveSocket **userdata = (ReceiveSocket**)luaL_checkudata(L, 1, "zmq.ReceiveSocket");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int ReceiveSocket__tostring(lua_State *L) {
  ReceiveSocket **userdata = (ReceiveSocket**)luaL_checkudata(L, 1, "zmq.ReceiveSocket");
  
  lua_pushfstring(L, "<zmq.ReceiveSocket: %p %s>", *userdata, (*userdata)->location());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void zmq::ReceiveSocket::connect(const char *location)
 * include/zmq/ReceiveSocket.h:71
 */
static int ReceiveSocket_connect(lua_State *L) {
  try {
    ReceiveSocket *self__ = *((ReceiveSocket**)luaL_checkudata(L, 1, "zmq.ReceiveSocket"));
    lua_remove(L, 1);
    const char *location = luaL_checkstring(L, 1);
    self__->connect(location);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.ReceiveSocket.connect: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.ReceiveSocket.connect: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void zmq::ReceiveSocket::setsockopt(int type, const char *filter=NULL)
 * include/zmq/ReceiveSocket.h:77
 */
static int ReceiveSocket_setsockopt(lua_State *L) {
  try {
    ReceiveSocket *self__ = *((ReceiveSocket**)luaL_checkudata(L, 1, "zmq.ReceiveSocket"));
    lua_remove(L, 1);
    int top__ = lua_gettop(L);
    int type = luaL_checkint(L, 1);
    if (top__ < 2) {
      self__->setsockopt(type);
    } else {
      const char *filter = luaL_checkstring(L, 2);
      self__->setsockopt(type, filter);
    }
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.ReceiveSocket.setsockopt: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.ReceiveSocket.setsockopt: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg ReceiveSocket_member_methods[] = {
  {"connect"           , ReceiveSocket_connect},
  {"setsockopt"        , ReceiveSocket_setsockopt},
  {"__tostring"        , ReceiveSocket__tostring},
  {"__gc"              , ReceiveSocket_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg ReceiveSocket_namespace_methods[] = {
  {"ReceiveSocket"     , ReceiveSocket_ReceiveSocket},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_zmq_ReceiveSocket(lua_State *L) {
#else
extern "C" int luaopen_zmq_ReceiveSocket(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "zmq.ReceiveSocket");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, ReceiveSocket_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "zmq", ReceiveSocket_namespace_methods);


	return 1;
}
