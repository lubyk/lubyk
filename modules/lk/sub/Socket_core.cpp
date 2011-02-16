#include "lk/Socket.h"

#include "lua_cpp_helper.h"


using namespace lk;


/* ============================ Constructors     ====================== */

/** lk::Socket::Socket(lubyk::Worker *worker, int socket_type)
 * include/lk/Socket.h:85
 */
static int Socket_Socket(lua_State *L) {
  try {
    lubyk::Worker *worker = *((lubyk::Worker **)luaL_checkudata(L, 1, "lubyk.Worker"));
    int socket_type = luaL_checkint(L, 2);
    Socket * retval__ = new Socket(worker, socket_type);
    lua_pushclass<Socket>(L, retval__, "lk.Socket");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.Socket: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.Socket: Unknown exception");
  }
}


/* ============================ Destructor       ====================== */

static int Socket_destructor(lua_State *L) {
  Socket **userdata = (Socket**)luaL_checkudata(L, 1, "lk.Socket");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Socket__tostring(lua_State *L) {
  Socket **userdata = (Socket**)luaL_checkudata(L, 1, "lk.Socket");
  
  
  lua_pushfstring(L, "<lk.Socket: %p %s:%d --> %s:%d>", *userdata, (*userdata)->local_host(), (*userdata)->local_port(), (*userdata)->remote_host(), (*userdata)->remote_port());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** LuaStackSize lk::Socket::accept(lua_State *L)
 * include/lk/Socket.h:216
 */
static int Socket_accept(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    
    LuaStackSize  retval__ = self__->accept(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.accept: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.accept: Unknown exception");
  }
}


/** int lk::Socket::bind(const char *localhost=NULL, int port=0)
 * include/lk/Socket.h:99
 */
static int Socket_bind(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    int top__ = lua_gettop(L);
    int  retval__;
    if (top__ < 2) {
      retval__ = self__->bind();
    } else {
      const char *localhost = luaL_checkstring(L, 2);
      if (top__ < 3) {
        retval__ = self__->bind(localhost);
      } else {
        int port = luaL_checkint(L, 3);
        retval__ = self__->bind(localhost, port);
      }
    }
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.bind: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.bind: Unknown exception");
  }
}


/** void lk::Socket::connect(const char *host, int port)
 * include/lk/Socket.h:154
 */
static int Socket_connect(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    const char *host = luaL_checkstring(L, 2);
    int port = luaL_checkint(L, 3);
    self__->connect(host, port);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.connect: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.connect: Unknown exception");
  }
}


/** void lk::Socket::kill()
 * include/lk/Socket.h:314
 */
static int Socket_kill(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    self__->kill();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.kill: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.kill: Unknown exception");
  }
}


/** void lk::Socket::listen()
 * include/lk/Socket.h:204
 */
static int Socket_listen(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    self__->listen();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.listen: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.listen: Unknown exception");
  }
}


/** const char* lk::Socket::local_host() const 
 * include/lk/Socket.h:319
 */
static int Socket_local_host(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    const char * retval__ = self__->local_host();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.local_host: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.local_host: Unknown exception");
  }
}


/** int lk::Socket::local_port() const 
 * include/lk/Socket.h:323
 */
static int Socket_local_port(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    int  retval__ = self__->local_port();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.local_port: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.local_port: Unknown exception");
  }
}


/** void lk::Socket::loop(lua_State *L)
 * include/lk/Socket.h:300
 */
static int Socket_loop(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    
    self__->loop(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.loop: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.loop: Unknown exception");
  }
}


/** int lk::Socket::port() const 
 * include/lk/Socket.h:335
 */
static int Socket_port(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    int  retval__ = self__->port();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.port: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.port: Unknown exception");
  }
}


/** void lk::Socket::quit()
 * include/lk/Socket.h:307
 */
static int Socket_quit(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    self__->quit();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.quit: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.quit: Unknown exception");
  }
}


/** LuaStackSize lk::Socket::recv(lua_State *L)
 * include/lk/Socket.h:262
 */
static int Socket_recv(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    
    LuaStackSize  retval__ = self__->recv(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.recv: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.recv: Unknown exception");
  }
}


/** const char* lk::Socket::remote_host() const 
 * include/lk/Socket.h:327
 */
static int Socket_remote_host(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    const char * retval__ = self__->remote_host();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.remote_host: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.remote_host: Unknown exception");
  }
}


/** int lk::Socket::remote_port() const 
 * include/lk/Socket.h:331
 */
static int Socket_remote_port(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    int  retval__ = self__->remote_port();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.remote_port: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.remote_port: Unknown exception");
  }
}


/** LuaStackSize lk::Socket::request(lua_State *L)
 * include/lk/Socket.h:293
 */
static int Socket_request(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    
    LuaStackSize  retval__ = self__->request(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.request: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.request: Unknown exception");
  }
}


/** void lk::Socket::send(lua_State *L)
 * include/lk/Socket.h:282
 */
static int Socket_send(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    
    self__->send(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.send: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.send: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Socket_member_methods[] = {
  {"accept"            , Socket_accept},
  {"bind"              , Socket_bind},
  {"connect"           , Socket_connect},
  {"kill"              , Socket_kill},
  {"listen"            , Socket_listen},
  {"local_host"        , Socket_local_host},
  {"local_port"        , Socket_local_port},
  {"loop"              , Socket_loop},
  {"port"              , Socket_port},
  {"quit"              , Socket_quit},
  {"recv"              , Socket_recv},
  {"remote_host"       , Socket_remote_host},
  {"remote_port"       , Socket_remote_port},
  {"request"           , Socket_request},
  {"send"              , Socket_send},
  {"__tostring"        , Socket__tostring},
  {"__gc"              , Socket_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Socket_namespace_methods[] = {
  {"Socket"            , Socket_Socket},
  {NULL, NULL},
};


static const struct lua_constants_Reg Socket_namespace_constants[] = {
  {"TCP"               , lk::Socket::TCP},
  {"UDP"               , lk::Socket::UDP},
  {NULL, NULL},
};


#ifdef DUB_LUA_NO_OPEN
int luaload_lk_Socket_core(lua_State *L) {
#else
extern "C" int luaopen_lk_Socket_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "lk.Socket");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Socket_member_methods);
  // save meta-table in lk.Socket_
  register_mt(L, "lk", "Socket");

  // register class methods in a global namespace table
  luaL_register(L, "lk", Socket_namespace_methods);


  // register class enums
  register_constants(L, "lk.Socket_const", Socket_namespace_constants);

	return 1;
}
