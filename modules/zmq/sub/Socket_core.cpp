#include "zmq/Socket.h"

#include "lua_cpp_helper.h"


using namespace zmq;


/* ============================ Constructors     ====================== */

/** zmq::Socket::Socket(rubyk::Worker *worker, int type)
 * include/zmq/Socket.h:60
 */
static int Socket_Socket(lua_State *L) {
  try {
    rubyk::Worker *worker = *((rubyk::Worker **)luaL_checkudata(L, 1, "rubyk.Worker"));
    int type = luaL_checkint(L, 2);
    Socket * retval__ = new Socket(worker, type);
    lua_pushclass<Socket>(L, retval__, "zmq.Socket");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.Socket.Socket: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Socket.Socket: Unknown exception");
    lua_error(L);
    return 0;
  }
}

/* ============================ Destructor       ====================== */

static int Socket_destructor(lua_State *L) {
  Socket **userdata = (Socket**)luaL_checkudata(L, 1, "zmq.Socket");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Socket__tostring(lua_State *L) {
  Socket **userdata = (Socket**)luaL_checkudata(L, 1, "zmq.Socket");
  
  lua_pushfstring(L, "<zmq.Socket: %p %s>", *userdata, (*userdata)->location());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void zmq::Socket::bind(const char *location)
 * include/zmq/Socket.h:82
 */
static int Socket_bind(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    lua_remove(L, 1);
    const char *location = luaL_checkstring(L, 1);
    self__->bind(location);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.Socket.bind: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Socket.bind: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void zmq::Socket::connect(const char *location)
 * include/zmq/Socket.h:89
 */
static int Socket_connect(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    lua_remove(L, 1);
    const char *location = luaL_checkstring(L, 1);
    self__->connect(location);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.Socket.connect: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Socket.connect: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void zmq::Socket::kill()
 * include/zmq/Socket.h:150
 */
static int Socket_kill(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    lua_remove(L, 1);
    self__->kill();
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.Socket.kill: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Socket.kill: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void zmq::Socket::loop(int lua_func_idx)
 * include/zmq/Socket.h:136
 */
static int Socket_loop(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    lua_remove(L, 1);
    
    luaL_checktype(L, 1, LUA_TFUNCTION);
    // push on top
    lua_pushvalue(L, 1);
    int lua_func_idx = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_pop(L, 1);
    
    self__->loop(lua_func_idx);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.Socket.loop: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Socket.loop: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void zmq::Socket::quit()
 * include/zmq/Socket.h:143
 */
static int Socket_quit(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    lua_remove(L, 1);
    self__->quit();
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.Socket.quit: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Socket.quit: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** LuaStackSize zmq::Socket::recv(lua_State *L)
 * include/zmq/Socket.h:98
 */
static int Socket_recv(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    lua_remove(L, 1);
    
    LuaStackSize  retval__ = self__->recv(L);
    return retval__;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.Socket.recv: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Socket.recv: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** LuaStackSize zmq::Socket::request(lua_State *L)
 * include/zmq/Socket.h:129
 */
static int Socket_request(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    lua_remove(L, 1);
    
    LuaStackSize  retval__ = self__->request(L);
    return retval__;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.Socket.request: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Socket.request: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void zmq::Socket::send(lua_State *L)
 * include/zmq/Socket.h:113
 */
static int Socket_send(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    lua_remove(L, 1);
    
    self__->send(L);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.Socket.send: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Socket.send: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void zmq::Socket::setsockopt(int type, const char *filter=NULL)
 * include/zmq/Socket.h:72
 */
static int Socket_setsockopt(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
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
    std::string *s = new std::string("zmq.Socket.setsockopt: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Socket.setsockopt: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Socket_member_methods[] = {
  {"bind"              , Socket_bind},
  {"connect"           , Socket_connect},
  {"kill"              , Socket_kill},
  {"loop"              , Socket_loop},
  {"quit"              , Socket_quit},
  {"recv"              , Socket_recv},
  {"request"           , Socket_request},
  {"send"              , Socket_send},
  {"setsockopt"        , Socket_setsockopt},
  {"__tostring"        , Socket__tostring},
  {"__gc"              , Socket_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Socket_namespace_methods[] = {
  {"Socket"            , Socket_Socket},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_zmq_Socket_core(lua_State *L) {
#else
extern "C" int luaopen_zmq_Socket_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "zmq.Socket");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Socket_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "zmq", Socket_namespace_methods);


	return 1;
}
