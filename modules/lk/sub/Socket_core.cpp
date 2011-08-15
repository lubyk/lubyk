#include "lk/Socket.h"

#include "lua_cpp_helper.h"


using namespace lk;


/* ============================ Constructors     ====================== */

/** lk::Socket::Socket(lubyk::Worker *worker, int socket_type)
 * include/lk/Socket.h:105
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
  
  
  lua_pushfstring(L, "<lk.Socket: %p %s:%d --> %s:%d>", *userdata, (*userdata)->localHost(), (*userdata)->localPort(), (*userdata)->remoteHost(), (*userdata)->remotePort());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** LuaStackSize lk::Socket::accept(lua_State *L)
 * include/lk/Socket.h:242
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
 * include/lk/Socket.h:124
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


/** void lk::Socket::close()
 * include/lk/Socket.h:113
 */
static int Socket_close(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    self__->close();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.close: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.close: Unknown exception");
  }
}


/** void lk::Socket::connect(const char *host, int port)
 * include/lk/Socket.h:180
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
 * include/lk/Socket.h:397
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
 * include/lk/Socket.h:230
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


/** const char* lk::Socket::localHost() const 
 * include/lk/Socket.h:402
 */
static int Socket_localHost(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    const char * retval__ = self__->localHost();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.localHost: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.localHost: Unknown exception");
  }
}


/** int lk::Socket::localPort() const 
 * include/lk/Socket.h:406
 */
static int Socket_localPort(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    int  retval__ = self__->localPort();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.localPort: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.localPort: Unknown exception");
  }
}


/** void lk::Socket::loop(lua_State *L)
 * include/lk/Socket.h:383
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
 * include/lk/Socket.h:418
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
 * include/lk/Socket.h:390
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
 * include/lk/Socket.h:321
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


/** const char* lk::Socket::remoteHost() const 
 * include/lk/Socket.h:410
 */
static int Socket_remoteHost(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    const char * retval__ = self__->remoteHost();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.remoteHost: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.remoteHost: Unknown exception");
  }
}


/** int lk::Socket::remotePort() const 
 * include/lk/Socket.h:414
 */
static int Socket_remotePort(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    int  retval__ = self__->remotePort();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.remotePort: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.remotePort: Unknown exception");
  }
}


/** void lk::Socket::send(lua_State *L)
 * include/lk/Socket.h:349
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


/** void lk::Socket::setRecvTimeout(int timeout)
 * include/lk/Socket.h:284
 */
static int Socket_setRecvTimeout(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    int timeout = luaL_checkint(L, 2);
    self__->setRecvTimeout(timeout);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.setRecvTimeout: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.setRecvTimeout: Unknown exception");
  }
}


/** void lk::Socket::setSendTimeout(int timeout)
 * include/lk/Socket.h:288
 */
static int Socket_setSendTimeout(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "lk.Socket"));
    int timeout = luaL_checkint(L, 2);
    self__->setSendTimeout(timeout);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Socket.setSendTimeout: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Socket.setSendTimeout: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Socket_member_methods[] = {
  {"accept"            , Socket_accept},
  {"bind"              , Socket_bind},
  {"close"             , Socket_close},
  {"connect"           , Socket_connect},
  {"kill"              , Socket_kill},
  {"listen"            , Socket_listen},
  {"localHost"         , Socket_localHost},
  {"localPort"         , Socket_localPort},
  {"loop"              , Socket_loop},
  {"port"              , Socket_port},
  {"quit"              , Socket_quit},
  {"recv"              , Socket_recv},
  {"remoteHost"        , Socket_remoteHost},
  {"remotePort"        , Socket_remotePort},
  {"send"              , Socket_send},
  {"setRecvTimeout"    , Socket_setRecvTimeout},
  {"setSendTimeout"    , Socket_setSendTimeout},
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
