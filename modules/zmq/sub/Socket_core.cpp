#include "zmq/Socket.h"

#include "lua_cpp_helper.h"





using namespace zmq;


/* ============================ Constructors     ====================== */

/** zmq::Socket::Socket(int type, lubyk::Worker *worker)
 * include/zmq/Socket.h:73
 */
static int Socket_Socket(lua_State *L) {
  try {
    int type = dubL_checkint(L, 1);
    lubyk::Worker *worker = *((lubyk::Worker **)dubL_checksdata(L, 2, "lubyk.Worker"));
    Socket * retval__ = new Socket(type, worker);
    // The class inherits from 'LuaObject', use luaInit instead of lua_pushclass.
    return retval__->luaInit(L, retval__, "zmq.Socket");
  } catch (std::exception &e) {
    lua_pushfstring(L, "Socket: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Socket: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Socket_destructor(lua_State *L) {
  Socket **userdata = (Socket**)dubL_checksdata_n(L, 1, "zmq.Socket");

  
  if (*userdata) (*userdata)->luaDestroy();
  
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
 * include/zmq/Socket.h:166
 */
static int Socket_bind1(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    const char *location = dubL_checkstring(L, 2);
    self->bind(location);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "bind: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "bind: Unknown exception");
  }
  return lua_error(L);
}



/** int zmq::Socket::bind(int min_port=2000, int max_port=20000, int retries=100)
 * include/zmq/Socket.h:176
 */
static int Socket_bind2(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    int top__ = lua_gettop(L);
    int  retval__;
    if (top__ < 2) {
      retval__ = self->bind();
    } else {
      int min_port = dubL_checkint(L, 2);
      if (top__ < 3) {
        retval__ = self->bind(min_port);
      } else {
        int max_port = dubL_checkint(L, 3);
        if (top__ < 4) {
          retval__ = self->bind(min_port, max_port);
        } else {
          int retries = dubL_checkint(L, 4);
          retval__ = self->bind(min_port, max_port, retries);
        }
      }
    }
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "bind: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "bind: Unknown exception");
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
 * include/zmq/Socket.h:199
 */
static int Socket_connect(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    const char *location = dubL_checkstring(L, 2);
    self->connect(location);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "connect: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "connect: Unknown exception");
  }
  return lua_error(L);
}



/** const char* zmq::Socket::location()
 * include/zmq/Socket.h:336
 */
static int Socket_location(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    const char * retval__ = self->location();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "location: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "location: Unknown exception");
  }
  return lua_error(L);
}



/** int zmq::Socket::port()
 * include/zmq/Socket.h:340
 */
static int Socket_port(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    int  retval__ = self->port();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "port: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "port: Unknown exception");
  }
  return lua_error(L);
}



/** void zmq::Socket::rawSend(lua_State *L)
 * include/zmq/Socket.h:249
 */
static int Socket_rawSend(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    
    self->rawSend(L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "rawSend: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "rawSend: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize zmq::Socket::rawSock(lua_State *L)
 * include/zmq/Socket.h:118
 */
static int Socket_rawSock(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    
    LuaStackSize  retval__ = self->rawSock(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "rawSock: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "rawSock: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize zmq::Socket::recv(lua_State *L)
 * include/zmq/Socket.h:211
 */
static int Socket_recv(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    
    LuaStackSize  retval__ = self->recv(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "recv: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "recv: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize zmq::Socket::request(lua_State *L)
 * include/zmq/Socket.h:264
 */
static int Socket_request(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    
    LuaStackSize  retval__ = self->request(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "request: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "request: Unknown exception");
  }
  return lua_error(L);
}



/** void zmq::Socket::send(lua_State *L)
 * include/zmq/Socket.h:232
 */
static int Socket_send(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    
    self->send(L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "send: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "send: Unknown exception");
  }
  return lua_error(L);
}



/** void zmq::Socket::setsockopt(int type, lua_State *L)
 * include/zmq/Socket.h:129
 */
static int Socket_setsockopt(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    int type = dubL_checkint(L, 2);
    
    self->setsockopt(type, L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setsockopt: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setsockopt: Unknown exception");
  }
  return lua_error(L);
}



/** const char* zmq::Socket::type() const 
 * include/zmq/Socket.h:351
 */
static int Socket_type(lua_State *L) {
  try {
    Socket *self = *((Socket**)dubL_checksdata(L, 1, "zmq.Socket"));
    const char * retval__ = self->type();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "type: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "type: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Socket_member_methods[] = {
  {"bind"              , Socket_bind},
  {"connect"           , Socket_connect},
  {"location"          , Socket_location},
  {"port"              , Socket_port},
  {"rawSend"           , Socket_rawSend},
  {"rawSock"           , Socket_rawSock},
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
