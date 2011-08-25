#include "zmq/Socket.h"

#include "lua_cpp_helper.h"


using namespace zmq;


/* ============================ Constructors     ====================== */

/** zmq::Socket::Socket(int type, lubyk::Worker *worker)
 * include/zmq/Socket.h:70
 */
static int Socket_Socket(lua_State *L) {
  try {
    int type = dubL_checkint(L, 1);
    lubyk::Worker *worker = *((lubyk::Worker **)dubL_checksdata(L, 2, "lubyk.Worker"));
    Socket * retval__ = new Socket(type, worker);
    // The class inherits from 'LuaCallback', use lua_init instead of pushclass.
    return retval__->luaInit(L, retval__, "zmq.Socket");
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.Socket: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.Socket: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Socket_destructor(lua_State *L) {
  Socket **userdata = (Socket**)dubL_checksdata_n(L, 1, "zmq.Socket");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Socket__tostring(lua_State *L) {
  Socket **userdata = (Socket**)dubL_checksdata_n(L, 1, "zmq.Socket");
  
  
  lua_pushfstring(L, "<zmq.Socket: %p %s (%s)>", *userdata, (*userdata)->location(), (*userdata)->type());
  
  return 1;
}

/* ============================ Member Methods   ====================== */



/** void zmq::Socket::bind(const char *location)
 * include/zmq/Socket.h:145
 */
static int Socket_bind1(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    const char *location = dubL_checkstring(L, 2);
    self__->bind(location);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.bind: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.bind: Unknown exception");
  }
  return lua_error(L);
}



/** int zmq::Socket::bind(int min_port=2000, int max_port=20000, int retries=100)
 * include/zmq/Socket.h:155
 */
static int Socket_bind2(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    int top__ = lua_gettop(L);
    int  retval__;
    if (top__ < 2) {
      retval__ = self__->bind();
    } else {
      int min_port = dubL_checkint(L, 2);
      if (top__ < 3) {
        retval__ = self__->bind(min_port);
      } else {
        int max_port = dubL_checkint(L, 3);
        if (top__ < 4) {
          retval__ = self__->bind(min_port, max_port);
        } else {
          int retries = dubL_checkint(L, 4);
          retval__ = self__->bind(min_port, max_port, retries);
        }
      }
    }
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.bind: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.bind: Unknown exception");
  }
  return lua_error(L);
}




/** Overloaded function chooser for bind(...) */
static int Socket_bind(lua_State *L) {
  int type__ = lua_type(L, 2);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TSTRING) {
    return Socket_bind1(L);
  } else if (top__ < 2) {
    return Socket_bind2(L);
  } else if (type__ == LUA_TNUMBER) {
    return Socket_bind2(L);
  } else {
    // use any to raise errors
    return Socket_bind2(L);
  }
}


/** void zmq::Socket::connect(const char *location)
 * include/zmq/Socket.h:178
 */
static int Socket_connect(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    const char *location = dubL_checkstring(L, 2);
    self__->connect(location);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.connect: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.connect: Unknown exception");
  }
  return lua_error(L);
}



/** const char* zmq::Socket::location()
 * include/zmq/Socket.h:302
 */
static int Socket_location(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    const char * retval__ = self__->location();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.location: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.location: Unknown exception");
  }
  return lua_error(L);
}



/** int zmq::Socket::port()
 * include/zmq/Socket.h:306
 */
static int Socket_port(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    int  retval__ = self__->port();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.port: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.port: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize zmq::Socket::recv(lua_State *L)
 * include/zmq/Socket.h:190
 */
static int Socket_recv(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    
    LuaStackSize  retval__ = self__->recv(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.recv: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.recv: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize zmq::Socket::request(lua_State *L)
 * include/zmq/Socket.h:230
 */
static int Socket_request(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    
    LuaStackSize  retval__ = self__->request(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.request: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.request: Unknown exception");
  }
  return lua_error(L);
}



/** void zmq::Socket::send(lua_State *L)
 * include/zmq/Socket.h:213
 */
static int Socket_send(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    
    self__->send(L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.send: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.send: Unknown exception");
  }
  return lua_error(L);
}



/** void zmq::Socket::setsockopt(int type, lua_State *L)
 * include/zmq/Socket.h:108
 */
static int Socket_setsockopt(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    int type = dubL_checkint(L, 2);
    
    self__->setsockopt(type, L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.setsockopt: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.setsockopt: Unknown exception");
  }
  return lua_error(L);
}



/** const char* zmq::Socket::type() const 
 * include/zmq/Socket.h:317
 */
static int Socket_type(lua_State *L) {
  try {
    Socket *self__ = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    const char * retval__ = self__->type();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.type: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.type: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Socket_member_methods[] = {
  {"bind"              , Socket_bind},
  {"connect"           , Socket_connect},
  {"location"          , Socket_location},
  {"port"              , Socket_port},
  {"recv"              , Socket_recv},
  {"request"           , Socket_request},
  {"send"              , Socket_send},
  {"setsockopt"        , Socket_setsockopt},
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
  // save meta-table in zmq.Socket_
  register_mt(L, "zmq", "Socket");

  // register class methods in a global namespace table
  luaL_register(L, "zmq", Socket_namespace_methods);


	return 1;
}
