#include "zmq/Socket.h"

#include "lua_cpp_helper.h"


using namespace zmq;


/* ============================ Constructors     ====================== */

/** zmq::Socket::Socket(rubyk::Worker *worker, int type, bool create_sock=true)
 * include/zmq/Socket.h:63
 */
static int Socket_Socket(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Socket * retval__;
    rubyk::Worker *worker = *((rubyk::Worker **)luaL_checkudata(L, 1, "rubyk.Worker"));
    int type = luaL_checkint(L, 2);
    if (top__ < 3) {
      retval__ = new Socket(worker, type);
    } else {
      bool create_sock = lua_toboolean(L, 3);
      retval__ = new Socket(worker, type, create_sock);
    }
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
  
  lua_pushfstring(L, "<zmq.Socket: %p %s (%s)>", *userdata, (*userdata)->location(), (*userdata)->type());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void zmq::Socket::bind(const char *location)
 * include/zmq/Socket.h:103
 */
static int Socket_bind(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    const char *location = luaL_checkstring(L, 2);
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


/** int zmq::Socket::bind_to_random_port(int min_port=2000, int max_port=20000, int retries=100)
 * include/zmq/Socket.h:113
 */
static int Socket_bind_to_random_port(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    int top__ = lua_gettop(L);
    int  retval__;
    if (top__ < 2) {
      retval__ = self__->bind_to_random_port();
    } else {
      int min_port = luaL_checkint(L, 2);
      if (top__ < 3) {
        retval__ = self__->bind_to_random_port(min_port);
      } else {
        int max_port = luaL_checkint(L, 3);
        if (top__ < 4) {
          retval__ = self__->bind_to_random_port(min_port, max_port);
        } else {
          int retries = luaL_checkint(L, 4);
          retval__ = self__->bind_to_random_port(min_port, max_port, retries);
        }
      }
    }
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.Socket.bind_to_random_port: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Socket.bind_to_random_port: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void zmq::Socket::connect(const char *location)
 * include/zmq/Socket.h:131
 */
static int Socket_connect(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    const char *location = luaL_checkstring(L, 2);
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


/** void zmq::Socket::join()
 * include/zmq/Socket.h:236
 */
static int Socket_join(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    self__->join();
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.Socket.join: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Socket.join: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void zmq::Socket::kill()
 * include/zmq/Socket.h:232
 */
static int Socket_kill(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
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


/** const char* zmq::Socket::location()
 * include/zmq/Socket.h:183
 */
static int Socket_location(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    const char * retval__ = self__->location();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.Socket.location: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Socket.location: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** int zmq::Socket::port()
 * include/zmq/Socket.h:187
 */
static int Socket_port(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    int  retval__ = self__->port();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.Socket.port: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Socket.port: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void zmq::Socket::quit()
 * include/zmq/Socket.h:228
 */
static int Socket_quit(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
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
 * include/zmq/Socket.h:141
 */
static int Socket_recv(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    
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
 * include/zmq/Socket.h:178
 */
static int Socket_request(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    
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
 * include/zmq/Socket.h:161
 */
static int Socket_send(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    
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


/** void zmq::Socket::set_callback(lua_State *L)
 * include/zmq/Socket.h:220
 */
static int Socket_set_callback(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    
    self__->set_callback(L);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.Socket.set_callback: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Socket.set_callback: Unknown exception");
    lua_error(L);
    return 0;
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


/** bool zmq::Socket::should_run()
 * include/zmq/Socket.h:241
 */
static int Socket_should_run(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    bool  retval__ = self__->should_run();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.Socket.should_run: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Socket.should_run: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** const char* zmq::Socket::type() const 
 * include/zmq/Socket.h:198
 */
static int Socket_type(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)luaL_checkudata(L, 1, "zmq.Socket"));
    const char * retval__ = self__->type();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.Socket.type: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Socket.type: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Socket_member_methods[] = {
  {"bind"              , Socket_bind},
  {"bind_to_random_port", Socket_bind_to_random_port},
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