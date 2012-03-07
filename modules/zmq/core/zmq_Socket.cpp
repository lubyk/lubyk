/**
 *
 * MACHINE GENERATED FILE. DO NOT EDIT.
 *
 * Bindings for class Socket
 *
 * This file has been generated by dub 2.1.~.
 */
#include "dub/dub.h"
#include "zmq/Socket.h"

using namespace zmq;

/** zmq::Socket::Socket(int type, lubyk::Worker *worker)
 * include/zmq/Socket.h:82
 */
static int Socket_Socket(lua_State *L) {
  try {
    int type = dub_checkint(L, 1);
    lubyk::Worker *worker = *((lubyk::Worker **)dub_checksdata(L, 2, "lubyk::Worker"));
    Socket *retval__ = new Socket(type, worker);
    retval__->pushobject(L, retval__, "zmq.Socket", true);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.Socket: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.Socket: Unknown exception");
  }
  return dub_error(L);
}

/** zmq::Socket::~Socket()
 * include/zmq/Socket.h:109
 */
static int Socket__Socket(lua_State *L) {
  try {
    DubUserdata *userdata = ((DubUserdata*)dub_checksdata_d(L, 1, "zmq.Socket"));
    if (userdata->gc) {
      Socket *self = (Socket *)userdata->ptr;
      delete self;
    }
    userdata->gc = false;
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.~Socket: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.~Socket: Unknown exception");
  }
  return dub_error(L);
}

/** int zmq::Socket::fd()
 * include/zmq/Socket.h:117
 */
static int Socket_fd(lua_State *L) {
  try {
    Socket *self = *((Socket **)dub_checksdata(L, 1, "zmq.Socket"));
    lua_pushnumber(L, self->fd());
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.fd: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.fd: Unknown exception");
  }
  return dub_error(L);
}

/** void zmq::Socket::setNonBlocking(bool non_blocking)
 * include/zmq/Socket.h:126
 */
static int Socket_setNonBlocking(lua_State *L) {
  try {
    Socket *self = *((Socket **)dub_checksdata(L, 1, "zmq.Socket"));
    bool non_blocking = dub_checkboolean(L, 2);
    self->setNonBlocking(non_blocking);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.setNonBlocking: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.setNonBlocking: Unknown exception");
  }
  return dub_error(L);
}

/** void zmq::Socket::setsockopt(int type, lua_State *L)
 * include/zmq/Socket.h:136
 */
static int Socket_setsockopt(lua_State *L) {
  try {
    Socket *self = *((Socket **)dub_checksdata(L, 1, "zmq.Socket"));
    int type = dub_checkint(L, 2);
    self->setsockopt(type, L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.setsockopt: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.setsockopt: Unknown exception");
  }
  return dub_error(L);
}

/** void zmq::Socket::bind(const char *location)
 * include/zmq/Socket.h:189
 */
static int Socket_bind(lua_State *L) {
  try {
    Socket *self = *((Socket **)dub_checksdata(L, 1, "zmq.Socket"));
    int top__ = lua_gettop(L);
    if (top__ >= 4) {
      int min_port = dub_checkint(L, 2);
      int max_port = dub_checkint(L, 3);
      int retries = dub_checkint(L, 4);
      lua_pushnumber(L, self->bind(min_port, max_port, retries));
      return 1;
    } else if (top__ >= 3) {
      int min_port = dub_checkint(L, 2);
      int max_port = dub_checkint(L, 3);
      lua_pushnumber(L, self->bind(min_port, max_port));
      return 1;
    } else if (top__ >= 2) {
      int type__ = lua_type(L, 2);
      if (type__ == LUA_TNUMBER) {
        int min_port = dub_checkint(L, 2);
        lua_pushnumber(L, self->bind(min_port));
        return 1;
      } else {
        const char *location = dub_checkstring(L, 2);
        self->bind(location);
        return 0;
      }
    } else {
      lua_pushnumber(L, self->bind());
      return 1;
    }
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.bind: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.bind: Unknown exception");
  }
  return dub_error(L);
}

/** void zmq::Socket::connect(const char *location)
 * include/zmq/Socket.h:220
 */
static int Socket_connect(lua_State *L) {
  try {
    Socket *self = *((Socket **)dub_checksdata(L, 1, "zmq.Socket"));
    const char *location = dub_checkstring(L, 2);
    self->connect(location);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.connect: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.connect: Unknown exception");
  }
  return dub_error(L);
}

/** bool zmq::Socket::hasEvent(int event)
 * include/zmq/Socket.h:230
 */
static int Socket_hasEvent(lua_State *L) {
  try {
    Socket *self = *((Socket **)dub_checksdata(L, 1, "zmq.Socket"));
    int event = dub_checkint(L, 2);
    lua_pushboolean(L, self->hasEvent(event));
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.hasEvent: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.hasEvent: Unknown exception");
  }
  return dub_error(L);
}

/** LuaStackSize zmq::Socket::recv(lua_State *L)
 * include/zmq/Socket.h:244
 */
static int Socket_recv(lua_State *L) {
  try {
    Socket *self = *((Socket **)dub_checksdata(L, 1, "zmq.Socket"));
    return self->recv(L);
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.recv: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.recv: Unknown exception");
  }
  return dub_error(L);
}

/** bool zmq::Socket::send(lua_State *L)
 * include/zmq/Socket.h:271
 */
static int Socket_send(lua_State *L) {
  try {
    Socket *self = *((Socket **)dub_checksdata(L, 1, "zmq.Socket"));
    lua_pushboolean(L, self->send(L));
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.send: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.send: Unknown exception");
  }
  return dub_error(L);
}

/** bool zmq::Socket::rawSend(lua_State *L)
 * include/zmq/Socket.h:296
 */
static int Socket_rawSend(lua_State *L) {
  try {
    Socket *self = *((Socket **)dub_checksdata(L, 1, "zmq.Socket"));
    lua_pushboolean(L, self->rawSend(L));
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.rawSend: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.rawSend: Unknown exception");
  }
  return dub_error(L);
}

/** LuaStackSize zmq::Socket::request(lua_State *L)
 * include/zmq/Socket.h:314
 */
static int Socket_request(lua_State *L) {
  try {
    Socket *self = *((Socket **)dub_checksdata(L, 1, "zmq.Socket"));
    return self->request(L);
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.request: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.request: Unknown exception");
  }
  return dub_error(L);
}

/** const char* zmq::Socket::location()
 * include/zmq/Socket.h:387
 */
static int Socket_location(lua_State *L) {
  try {
    Socket *self = *((Socket **)dub_checksdata(L, 1, "zmq.Socket"));
    lua_pushstring(L, self->location());
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.location: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.location: Unknown exception");
  }
  return dub_error(L);
}

/** int zmq::Socket::port()
 * include/zmq/Socket.h:391
 */
static int Socket_port(lua_State *L) {
  try {
    Socket *self = *((Socket **)dub_checksdata(L, 1, "zmq.Socket"));
    lua_pushnumber(L, self->port());
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.port: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.port: Unknown exception");
  }
  return dub_error(L);
}

/** const char* zmq::Socket::type() const
 * include/zmq/Socket.h:402
 */
static int Socket_type(lua_State *L) {
  try {
    Socket *self = *((Socket **)dub_checksdata(L, 1, "zmq.Socket"));
    lua_pushstring(L, self->type());
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Socket.type: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Socket.type: Unknown exception");
  }
  return dub_error(L);
}



// --=============================================== __tostring
static int Socket___tostring(lua_State *L) {
  Socket *self = *((Socket **)dub_checksdata_n(L, 1, "zmq.Socket"));
  lua_pushfstring(L, "zmq.Socket: %p (%s (%s))", self, self-> location(), self-> type());
  
  return 1;
}

// --=============================================== METHODS

static const struct luaL_Reg Socket_member_methods[] = {
  { "new"          , Socket_Socket        },
  { "__gc"         , Socket__Socket       },
  { "fd"           , Socket_fd            },
  { "setNonBlocking", Socket_setNonBlocking },
  { "setsockopt"   , Socket_setsockopt    },
  { "bind"         , Socket_bind          },
  { "connect"      , Socket_connect       },
  { "hasEvent"     , Socket_hasEvent      },
  { "recv"         , Socket_recv          },
  { "send"         , Socket_send          },
  { "rawSend"      , Socket_rawSend       },
  { "request"      , Socket_request       },
  { "location"     , Socket_location      },
  { "port"         , Socket_port          },
  { "type"         , Socket_type          },
  { "__tostring"   , Socket___tostring    },
  { NULL, NULL},
};


extern "C" int luaopen_zmq_Socket(lua_State *L)
{
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "zmq.Socket");
  // <mt>

  // register member methods
  luaL_register(L, NULL, Socket_member_methods);
  // save meta-table in zmq
  dub_register(L, "zmq", "Socket_core");
  // <mt>
  lua_pop(L, 1);
  return 0;
}
