/**
 *
 * MACHINE GENERATED FILE. DO NOT EDIT.
 *
 * Bindings for class Poller
 *
 * This file has been generated by dub 2.1.~.
 */
#include "dub/dub.h"
#include "zmq/Poller.h"

using namespace zmq;

/** Set attributes (key, value)
 * 
 */
static int Poller__set_(lua_State *L) {

  Poller *self = *((Poller **)dub_checksdata_n(L, 1, "zmq.Poller"));
  const char *key = luaL_checkstring(L, 2);
  int key_h = dub_hash(key, 2);
  switch(key_h) {
    case 0: {
      if (DUB_ASSERT_KEY(key, "sThisKey")) break;
      Poller::sThisKey = **((pthread_key_t *)dub_checksdata_n(L, 3, "pthread_key_t"));
      return 0;
    }
  }
  if (lua_istable(L, 1)) {
    lua_rawset(L, 1);
  } else {
    luaL_error(L, KEY_EXCEPTION_MSG, key);
  }
  return 0;
}

/** Get attributes (key)
 * 
 */
static int Poller__get_(lua_State *L) {

  Poller *self = *((Poller **)dub_checksdata_n(L, 1, "zmq.Poller", true));
  const char *key = luaL_checkstring(L, 2);
  // <self> "key" <mt>
  // rawget(mt, key)
  lua_pushvalue(L, 2);
  // <self> "key" <mt> "key"
  lua_rawget(L, -2);
  if (!lua_isnil(L, -1)) {
    // Found method.
    return 1;
  } else {
    // Not in mt = attribute access.
    lua_pop(L, 2);
  }
  int key_h = dub_hash(key, 2);
  switch(key_h) {
    case 0: {
      if (DUB_ASSERT_KEY(key, "sThisKey")) break;
      dub_pushudata(L, &Poller::sThisKey, "pthread_key_t", false);
      return 1;
    }
  }
  return 0;
}

/** zmq::Poller::Poller(int reserve=8)
 * include/zmq/Poller.h:100
 */
static int Poller_Poller(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    if (top__ >= 1) {
      int reserve = dub_checkint(L, 1);
      Poller *retval__ = new Poller(reserve);
      retval__->pushobject(L, retval__, "zmq.Poller", true);
      return 1;
    } else {
      Poller *retval__ = new Poller();
      retval__->pushobject(L, retval__, "zmq.Poller", true);
      return 1;
    }
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Poller.Poller: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Poller.Poller: Unknown exception");
  }
  return dub_error(L);
}

/** zmq::Poller::~Poller()
 * include/zmq/Poller.h:130
 */
static int Poller__Poller(lua_State *L) {
  try {
    DubUserdata *userdata = ((DubUserdata*)dub_checksdata_d(L, 1, "zmq.Poller"));
    if (userdata->gc) {
      Poller *self = (Poller *)userdata->ptr;
      delete self;
    }
    userdata->gc = false;
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Poller.~Poller: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Poller.~Poller: Unknown exception");
  }
  return dub_error(L);
}

/** bool zmq::Poller::poll(float timeout)
 * include/zmq/Poller.h:141
 */
static int Poller_poll(lua_State *L) {
  try {
    Poller *self = *((Poller **)dub_checksdata(L, 1, "zmq.Poller"));
    float timeout = dub_checknumber(L, 2);
    lua_pushboolean(L, self->poll(timeout));
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Poller.poll: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Poller.poll: Unknown exception");
  }
  return dub_error(L);
}

/** LuaStackSize zmq::Poller::events(lua_State *L)
 * include/zmq/Poller.h:200
 */
static int Poller_events(lua_State *L) {
  try {
    Poller *self = *((Poller **)dub_checksdata(L, 1, "zmq.Poller"));
    return self->events(L);
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Poller.events: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Poller.events: Unknown exception");
  }
  return dub_error(L);
}

/** int zmq::Poller::add(int fd, int events)
 * include/zmq/Poller.h:218
 */
static int Poller_add(lua_State *L) {
  try {
    Poller *self = *((Poller **)dub_checksdata(L, 1, "zmq.Poller"));
    int type__ = lua_type(L, 2);
    if (type__ == LUA_TNUMBER) {
      int fd = dub_checkint(L, 2);
      int events = dub_checkint(L, 3);
      lua_pushnumber(L, self->add(fd, events));
      return 1;
    } else {
      Socket *sock = *((Socket **)dub_checksdata(L, 2, "zmq.Socket"));
      int events = dub_checkint(L, 3);
      lua_pushnumber(L, self->add(sock, events));
      return 1;
    }
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Poller.add: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Poller.add: Unknown exception");
  }
  return dub_error(L);
}

/** void zmq::Poller::modify(int idx, int events, lua_State *L)
 * include/zmq/Poller.h:228
 */
static int Poller_modify(lua_State *L) {
  try {
    Poller *self = *((Poller **)dub_checksdata(L, 1, "zmq.Poller"));
    int idx = dub_checkint(L, 2);
    int events = dub_checkint(L, 3);
    self->modify(idx, events, L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Poller.modify: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Poller.modify: Unknown exception");
  }
  return dub_error(L);
}

/** void zmq::Poller::remove(int idx)
 * include/zmq/Poller.h:250
 */
static int Poller_remove(lua_State *L) {
  try {
    Poller *self = *((Poller **)dub_checksdata(L, 1, "zmq.Poller"));
    int idx = dub_checkint(L, 2);
    self->remove(idx);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Poller.remove: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Poller.remove: Unknown exception");
  }
  return dub_error(L);
}

/** int zmq::Poller::count()
 * include/zmq/Poller.h:276
 */
static int Poller_count(lua_State *L) {
  try {
    Poller *self = *((Poller **)dub_checksdata(L, 1, "zmq.Poller"));
    lua_pushnumber(L, self->count());
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Poller.count: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Poller.count: Unknown exception");
  }
  return dub_error(L);
}

/** LuaStackSize zmq::Poller::idxToPos(int idx, lua_State *L)
 * include/zmq/Poller.h:283
 */
static int Poller_idxToPos(lua_State *L) {
  try {
    Poller *self = *((Poller **)dub_checksdata(L, 1, "zmq.Poller"));
    int idx = dub_checkint(L, 2);
    return self->idxToPos(idx, L);
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Poller.idxToPos: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Poller.idxToPos: Unknown exception");
  }
  return dub_error(L);
}

/** LuaStackSize zmq::Poller::posToIdx(int pos, lua_State *L)
 * include/zmq/Poller.h:292
 */
static int Poller_posToIdx(lua_State *L) {
  try {
    Poller *self = *((Poller **)dub_checksdata(L, 1, "zmq.Poller"));
    int pos = dub_checkint(L, 2);
    return self->posToIdx(pos, L);
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Poller.posToIdx: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Poller.posToIdx: Unknown exception");
  }
  return dub_error(L);
}

/** LuaStackSize zmq::Poller::posToFd(int pos, lua_State *L)
 * include/zmq/Poller.h:301
 */
static int Poller_posToFd(lua_State *L) {
  try {
    Poller *self = *((Poller **)dub_checksdata(L, 1, "zmq.Poller"));
    int pos = dub_checkint(L, 2);
    return self->posToFd(pos, L);
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Poller.posToFd: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Poller.posToFd: Unknown exception");
  }
  return dub_error(L);
}

/** LuaStackSize zmq::Poller::posToEvent(int pos, lua_State *L)
 * include/zmq/Poller.h:310
 */
static int Poller_posToEvent(lua_State *L) {
  try {
    Poller *self = *((Poller **)dub_checksdata(L, 1, "zmq.Poller"));
    int pos = dub_checkint(L, 2);
    return self->posToEvent(pos, L);
  } catch (std::exception &e) {
    lua_pushfstring(L, "zmq.Poller.posToEvent: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "zmq.Poller.posToEvent: Unknown exception");
  }
  return dub_error(L);
}



// --=============================================== __tostring
static int Poller___tostring(lua_State *L) {
  Poller *self = *((Poller **)dub_checksdata_n(L, 1, "zmq.Poller"));
  lua_pushfstring(L, "zmq.Poller: %p (%f)", self, self-> count());
  
  return 1;
}

// --=============================================== METHODS

static const struct luaL_Reg Poller_member_methods[] = {
  { "__newindex"   , Poller__set_         },
  { "__index"      , Poller__get_         },
  { "new"          , Poller_Poller        },
  { "__gc"         , Poller__Poller       },
  { "poll"         , Poller_poll          },
  { "events"       , Poller_events        },
  { "add"          , Poller_add           },
  { "modify"       , Poller_modify        },
  { "remove"       , Poller_remove        },
  { "count"        , Poller_count         },
  { "idxToPos"     , Poller_idxToPos      },
  { "posToIdx"     , Poller_posToIdx      },
  { "posToFd"      , Poller_posToFd       },
  { "posToEvent"   , Poller_posToEvent    },
  { "__tostring"   , Poller___tostring    },
  { NULL, NULL},
};


extern "C" int luaopen_zmq_Poller(lua_State *L)
{
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "zmq.Poller");
  // <mt>

  // register member methods
  luaL_register(L, NULL, Poller_member_methods);
  // save meta-table in zmq
  dub_register(L, "zmq", "Poller_core");
  // <mt>
  lua_pop(L, 1);
  return 0;
}
