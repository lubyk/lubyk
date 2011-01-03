#include "rk/Socket.h"

#include "lua_cpp_helper.h"


using namespace rk;


/* ============================ Constructors     ====================== */

/** rk::Socket::Socket(rubyk::Worker *worker)
 * include/rk/Socket.h:76
 */
static int Socket_Socket(lua_State *L) {
  try {
    rubyk::Worker *worker = *((rubyk::Worker **)luaL_checkudata(L, 1, "rubyk.Worker"));
    Socket * retval__ = new Socket(worker);
    lua_pushclass<Socket>(L, retval__, "rk.Socket");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Socket.Socket: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Socket.Socket: Unknown exception");
    lua_error(L);
    return 0;
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
 * include/rk/Socket.h:194
 */
static int Socket_accept(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    
    LuaStackSize  retval__ = self__->accept(L);
    return retval__;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Socket.accept: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Socket.accept: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** int rk::Socket::bind(const char *localhost=NULL, int port=0)
 * include/rk/Socket.h:89
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
    std::string *s = new std::string("rk.Socket.bind: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Socket.bind: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void rk::Socket::connect(const char *host, int port)
 * include/rk/Socket.h:143
 */
static int Socket_connect(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    const char *host = luaL_checkstring(L, 2);
    int port = luaL_checkint(L, 3);
    self__->connect(host, port);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Socket.connect: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Socket.connect: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void rk::Socket::kill()
 * include/rk/Socket.h:294
 */
static int Socket_kill(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    self__->kill();
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Socket.kill: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Socket.kill: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void rk::Socket::listen()
 * include/rk/Socket.h:182
 */
static int Socket_listen(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    self__->listen();
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Socket.listen: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Socket.listen: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** const char* rk::Socket::local_host() const 
 * include/rk/Socket.h:299
 */
static int Socket_local_host(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    const char * retval__ = self__->local_host();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Socket.local_host: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Socket.local_host: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** int rk::Socket::local_port() const 
 * include/rk/Socket.h:303
 */
static int Socket_local_port(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    int  retval__ = self__->local_port();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Socket.local_port: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Socket.local_port: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void rk::Socket::loop(lua_State *L)
 * include/rk/Socket.h:280
 */
static int Socket_loop(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    
    self__->loop(L);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Socket.loop: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Socket.loop: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** int rk::Socket::port() const 
 * include/rk/Socket.h:315
 */
static int Socket_port(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    int  retval__ = self__->port();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Socket.port: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Socket.port: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void rk::Socket::quit()
 * include/rk/Socket.h:287
 */
static int Socket_quit(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    self__->quit();
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Socket.quit: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Socket.quit: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** LuaStackSize rk::Socket::recv(lua_State *L)
 * include/rk/Socket.h:238
 */
static int Socket_recv(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    
    LuaStackSize  retval__ = self__->recv(L);
    return retval__;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Socket.recv: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Socket.recv: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** const char* rk::Socket::remote_host() const 
 * include/rk/Socket.h:307
 */
static int Socket_remote_host(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    const char * retval__ = self__->remote_host();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Socket.remote_host: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Socket.remote_host: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** int rk::Socket::remote_port() const 
 * include/rk/Socket.h:311
 */
static int Socket_remote_port(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    int  retval__ = self__->remote_port();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Socket.remote_port: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Socket.remote_port: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** LuaStackSize rk::Socket::request(lua_State *L)
 * include/rk/Socket.h:273
 */
static int Socket_request(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    
    LuaStackSize  retval__ = self__->request(L);
    return retval__;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Socket.request: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Socket.request: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void rk::Socket::send(lua_State *L)
 * include/rk/Socket.h:261
 */
static int Socket_send(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "rk.Socket"));
    
    self__->send(L);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Socket.send: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Socket.send: Unknown exception");
    lua_error(L);
    return 0;
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


	return 1;
}
