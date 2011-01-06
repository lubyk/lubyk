#include "zmq/Socket.h"

#include "lua_cpp_helper.h"


using namespace zmq;


/* ============================ Constructors     ====================== */

/** zmq::Socket::Socket(rubyk::Worker *worker, int type)
 * include/zmq/Socket.h:64
 */
static int Socket_Socket(lua_State *L) {
  try {
    rubyk::Worker *worker = *((rubyk::Worker **)luaL_checkudata(L, 1, "rubyk.Worker"));
    int type = luaL_checkint(L, 2);
    Socket * retval__ = new Socket(worker, type);
    lua_pushclass<Socket>(L, retval__, "zmq.Socket");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "zmq.Socket.Socket: %s", e.what());
  } catch (...) {
    return luaL_error(L, "zmq.Socket.Socket: Unknown exception");
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
  
  
  lua_pushfstring(L, "<zmq.Socket: %p %s (%s)>", *userdata, (*userdata)->location(), (*userdata)->type());
  
  return 1;
}

/* ============================ Member Methods   ====================== */



/** void zmq::Socket::bind(const char *location)
 * include/zmq/Socket.h:104
 */
static int Socket_bind1(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    const char *location = luaL_checkstring(L, 2);
    self__->bind(location);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "zmq.Socket.bind: %s", e.what());
  } catch (...) {
    return luaL_error(L, "zmq.Socket.bind: Unknown exception");
  }
}


/** int zmq::Socket::bind(int min_port=2000, int max_port=20000, int retries=100)
 * include/zmq/Socket.h:114
 */
static int Socket_bind2(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    int top__ = lua_gettop(L);
    int  retval__;
    if (top__ < 2) {
      retval__ = self__->bind();
    } else {
      int min_port = luaL_checkint(L, 2);
      if (top__ < 3) {
        retval__ = self__->bind(min_port);
      } else {
        int max_port = luaL_checkint(L, 3);
        if (top__ < 4) {
          retval__ = self__->bind(min_port, max_port);
        } else {
          int retries = luaL_checkint(L, 4);
          retval__ = self__->bind(min_port, max_port, retries);
        }
      }
    }
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "zmq.Socket.bind: %s", e.what());
  } catch (...) {
    return luaL_error(L, "zmq.Socket.bind: Unknown exception");
  }
}



/** Overloaded function chooser for bind(...) */
static int Socket_bind(lua_State *L) {
  int type__ = lua_type(L, 2);
  if (type__ == LUA_TSTRING) {
    return Socket_bind1(L);
  } else if (type__ == LUA_TNUMBER) {
    return Socket_bind2(L);
  } else {
    // use any to raise errors
    return Socket_bind2(L);
  }
}


/** void zmq::Socket::connect(const char *location)
 * include/zmq/Socket.h:134
 */
static int Socket_connect(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    const char *location = luaL_checkstring(L, 2);
    self__->connect(location);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "zmq.Socket.connect: %s", e.what());
  } catch (...) {
    return luaL_error(L, "zmq.Socket.connect: Unknown exception");
  }
}


/** void zmq::Socket::join()
 * include/zmq/Socket.h:272
 */
static int Socket_join(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    self__->join();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "zmq.Socket.join: %s", e.what());
  } catch (...) {
    return luaL_error(L, "zmq.Socket.join: Unknown exception");
  }
}


/** void zmq::Socket::kill()
 * include/zmq/Socket.h:267
 */
static int Socket_kill(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    self__->kill();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "zmq.Socket.kill: %s", e.what());
  } catch (...) {
    return luaL_error(L, "zmq.Socket.kill: Unknown exception");
  }
}


/** const char* zmq::Socket::location()
 * include/zmq/Socket.h:220
 */
static int Socket_location(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    const char * retval__ = self__->location();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "zmq.Socket.location: %s", e.what());
  } catch (...) {
    return luaL_error(L, "zmq.Socket.location: Unknown exception");
  }
}


/** int zmq::Socket::port()
 * include/zmq/Socket.h:224
 */
static int Socket_port(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    int  retval__ = self__->port();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "zmq.Socket.port: %s", e.what());
  } catch (...) {
    return luaL_error(L, "zmq.Socket.port: Unknown exception");
  }
}


/** void zmq::Socket::quit()
 * include/zmq/Socket.h:263
 */
static int Socket_quit(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    self__->quit();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "zmq.Socket.quit: %s", e.what());
  } catch (...) {
    return luaL_error(L, "zmq.Socket.quit: Unknown exception");
  }
}


/** LuaStackSize zmq::Socket::recv(lua_State *L)
 * include/zmq/Socket.h:146
 */
static int Socket_recv(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    
    LuaStackSize  retval__ = self__->recv(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "zmq.Socket.recv: %s", e.what());
  } catch (...) {
    return luaL_error(L, "zmq.Socket.recv: Unknown exception");
  }
}


/** LuaStackSize zmq::Socket::request(lua_State *L)
 * include/zmq/Socket.h:186
 */
static int Socket_request(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    
    LuaStackSize  retval__ = self__->request(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "zmq.Socket.request: %s", e.what());
  } catch (...) {
    return luaL_error(L, "zmq.Socket.request: Unknown exception");
  }
}


/** void zmq::Socket::send(lua_State *L)
 * include/zmq/Socket.h:169
 */
static int Socket_send(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    
    self__->send(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "zmq.Socket.send: %s", e.what());
  } catch (...) {
    return luaL_error(L, "zmq.Socket.send: Unknown exception");
  }
}


/** void zmq::Socket::set_callback(lua_State *L)
 * include/zmq/Socket.h:257
 */
static int Socket_set_callback(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    
    self__->set_callback(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "zmq.Socket.set_callback: %s", e.what());
  } catch (...) {
    return luaL_error(L, "zmq.Socket.set_callback: Unknown exception");
  }
}


/** void zmq::Socket::setsockopt(int type, const char *filter=NULL)
 * include/zmq/Socket.h:91
 */
static int Socket_setsockopt(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    int top__ = lua_gettop(L);
    int type = luaL_checkint(L, 2);
    if (top__ < 3) {
      self__->setsockopt(type);
    } else {
      const char *filter = luaL_checkstring(L, 3);
      self__->setsockopt(type, filter);
    }
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "zmq.Socket.setsockopt: %s", e.what());
  } catch (...) {
    return luaL_error(L, "zmq.Socket.setsockopt: Unknown exception");
  }
}


/** bool zmq::Socket::should_run()
 * include/zmq/Socket.h:277
 */
static int Socket_should_run(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    bool  retval__ = self__->should_run();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "zmq.Socket.should_run: %s", e.what());
  } catch (...) {
    return luaL_error(L, "zmq.Socket.should_run: Unknown exception");
  }
}


/** const char* zmq::Socket::type() const 
 * include/zmq/Socket.h:235
 */
static int Socket_type(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    const char * retval__ = self__->type();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "zmq.Socket.type: %s", e.what());
  } catch (...) {
    return luaL_error(L, "zmq.Socket.type: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Socket_member_methods[] = {
  {"bind"              , Socket_bind},
  {"connect"           , Socket_connect},
  {"join"              , Socket_join},
  {"kill"              , Socket_kill},
  {"location"          , Socket_location},
  {"port"              , Socket_port},
  {"quit"              , Socket_quit},
  {"recv"              , Socket_recv},
  {"request"           , Socket_request},
  {"send"              , Socket_send},
  {"set_callback"      , Socket_set_callback},
  {"setsockopt"        , Socket_setsockopt},
  {"should_run"        , Socket_should_run},
  {"type"              , Socket_type},
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
