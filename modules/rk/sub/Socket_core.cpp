#include "rk/Socket.h"

#include "lua_cpp_helper.h"


using namespace rk;


/* ============================ Constructors     ====================== */

/** rk::Socket::Socket(rubyk::Worker *worker, int socket_type)
 * include/rk/Socket.h:85
 */
static int Socket_Socket(lua_State *L) {
  try {
    rubyk::Worker *worker = *((rubyk::Worker **)luaL_checkudata(L, 1, "rubyk.Worker"));
    int socket_type = luaL_checkint(L, 2);
    Socket * retval__ = new Socket(worker, socket_type);
    lua_pushclass<Socket>(L, retval__, "rk.Socket");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Socket.Socket: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Socket.Socket: Unknown exception");
  }
}

/* ============================ Destructor       ====================== */

static int Socket_destructor(lua_State *L) {
  Socket **userdata = (Socket**)luaL_checkudata(L, 1, "rk.Socket");
  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Socket__tostring(lua_State *L) {
  Socket **userdata = (Socket**)luaL_checkudata(L, 1, "rk.Socket");
  
  
  lua_pushfstring(L, "<rk.Socket: %p %s:%d --> %s:%d>", *userdata, (*userdata)->local_host(), (*userdata)->local_port(), (*userdata)->remote_host(), (*userdata)->remote_port());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** LuaStackSize rk::Socket::accept(lua_State *L)
 * include/rk/Socket.h:216
 */
static int Socket_accept(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    
    LuaStackSize  retval__ = self__->accept(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Socket.accept: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Socket.accept: Unknown exception");
  }
}


/** int rk::Socket::bind(const char *localhost=NULL, int port=0)
 * include/rk/Socket.h:99
 */
static int Socket_bind(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
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
    return luaL_error(L, "rk.Socket.bind: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Socket.bind: Unknown exception");
  }
}


/** void rk::Socket::connect(const char *host, int port)
 * include/rk/Socket.h:154
 */
static int Socket_connect(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    const char *host = luaL_checkstring(L, 2);
    int port = luaL_checkint(L, 3);
    self__->connect(host, port);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Socket.connect: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Socket.connect: Unknown exception");
  }
}


/** void rk::Socket::kill()
 * include/rk/Socket.h:316
 */
static int Socket_kill(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    self__->kill();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Socket.kill: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Socket.kill: Unknown exception");
  }
}


/** void rk::Socket::listen()
 * include/rk/Socket.h:204
 */
static int Socket_listen(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    self__->listen();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Socket.listen: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Socket.listen: Unknown exception");
  }
}


/** const char* rk::Socket::local_host() const 
 * include/rk/Socket.h:321
 */
static int Socket_local_host(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    const char * retval__ = self__->local_host();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Socket.local_host: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Socket.local_host: Unknown exception");
  }
}


/** int rk::Socket::local_port() const 
 * include/rk/Socket.h:325
 */
static int Socket_local_port(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    int  retval__ = self__->local_port();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Socket.local_port: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Socket.local_port: Unknown exception");
  }
}


/** void rk::Socket::loop(lua_State *L)
 * include/rk/Socket.h:302
 */
static int Socket_loop(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    
    self__->loop(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Socket.loop: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Socket.loop: Unknown exception");
  }
}


/** int rk::Socket::port() const 
 * include/rk/Socket.h:337
 */
static int Socket_port(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    int  retval__ = self__->port();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Socket.port: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Socket.port: Unknown exception");
  }
}


/** void rk::Socket::quit()
 * include/rk/Socket.h:309
 */
static int Socket_quit(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    self__->quit();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Socket.quit: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Socket.quit: Unknown exception");
  }
}


/** LuaStackSize rk::Socket::recv(lua_State *L)
 * include/rk/Socket.h:262
 */
static int Socket_recv(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    
    LuaStackSize  retval__ = self__->recv(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Socket.recv: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Socket.recv: Unknown exception");
  }
}


/** const char* rk::Socket::remote_host() const 
 * include/rk/Socket.h:329
 */
static int Socket_remote_host(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    const char * retval__ = self__->remote_host();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Socket.remote_host: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Socket.remote_host: Unknown exception");
  }
}


/** int rk::Socket::remote_port() const 
 * include/rk/Socket.h:333
 */
static int Socket_remote_port(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    int  retval__ = self__->remote_port();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Socket.remote_port: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Socket.remote_port: Unknown exception");
  }
}


/** LuaStackSize rk::Socket::request(lua_State *L)
 * include/rk/Socket.h:295
 */
static int Socket_request(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    
    LuaStackSize  retval__ = self__->request(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Socket.request: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Socket.request: Unknown exception");
  }
}


/** void rk::Socket::send(lua_State *L)
 * include/rk/Socket.h:283
 */
static int Socket_send(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    
    self__->send(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Socket.send: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Socket.send: Unknown exception");
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
  {"TCP"               , rk::Socket::TCP},
  {"UDP"               , rk::Socket::UDP},
  {NULL, NULL},
};


#ifdef DUB_LUA_NO_OPEN
int luaload_rk_Socket_core(lua_State *L) {
#else
extern "C" int luaopen_rk_Socket_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "rk.Socket");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Socket_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "rk", Socket_namespace_methods);


  // register class enums
  register_constants(L, "rk.Socket_const", Socket_namespace_constants);

	return 1;
}
