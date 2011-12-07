#include "mimas/Timer.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Timer::Timer(int timeout)
 * include/mimas/Timer.h:53
 */
static int Timer_Timer(lua_State *L) {
  try {
    int timeout = dubL_checkint(L, 1);
    Timer * retval__ = new Timer(timeout);
    return retval__->luaInit(L, retval__, "mimas.Timer");
  } catch (std::exception &e) {
    lua_pushfstring(L, "Timer: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Timer: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Timer_destructor(lua_State *L) {
  Timer **userdata = (Timer**)dubL_checksdata_n(L, 1, "mimas.Timer");

  
  // custom destructor
  Timer *self = *userdata;
  if (self) self->luaDestroy();
  
  *userdata = NULL;
  return 0;
}



// test if class is deleted
static int Timer_deleted(lua_State *L) {
  Timer **userdata = (Timer**)dubL_checksdata_n(L, 1, "mimas.Timer");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Timer__tostring(lua_State *L) {
  Timer **userdata = (Timer**)dubL_checksdata_n(L, 1, "mimas.Timer");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.Timer: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.Timer: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::QObject::name()
 * mimas/bind/QObject.h:12
 */
static int QObject_name(lua_State *L) {
  try {
    Timer *self = *((Timer**)dubL_checksdata(L, 1, "mimas.Timer"));
    if (!self) throw dub::Exception("Using deleted mimas.Timer in name");
    lua_pushstring(L, self->objectName().toUtf8().data());
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "name: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "name: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QObject::object()
 * mimas/bind/QObject.h:11
 */
static int QObject_object(lua_State *L) {
  try {
    Timer *self = *((Timer**)dubL_checksdata(L, 1, "mimas.Timer"));
    if (!self) throw dub::Exception("Using deleted mimas.Timer in object");
    QObject * retval__ = self;
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "object: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "object: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Timer::setInterval(int msec)
 * include/mimas/Timer.h:79
 */
static int Timer_setInterval(lua_State *L) {
  try {
    Timer *self = *((Timer**)dubL_checksdata(L, 1, "mimas.Timer"));
    if (!self) throw dub::Exception("Using deleted mimas.Timer in setInterval");
    int msec = dubL_checkint(L, 2);
    self->setInterval(msec);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setInterval: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setInterval: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QObject::setName(const char *name)
 * mimas/bind/QObject.h:13
 */
static int QObject_setName(lua_State *L) {
  try {
    Timer *self = *((Timer**)dubL_checksdata(L, 1, "mimas.Timer"));
    if (!self) throw dub::Exception("Using deleted mimas.Timer in setName");
    const char *name = dubL_checkstring(L, 2);
    self->setObjectName(QString(name));
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setName: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setName: Unknown exception");
  }
  return lua_error(L);
}




/** void mimas::Timer::start(int timeout)
 * include/mimas/Timer.h:67
 */
static int Timer_start1(lua_State *L) {
  try {
    Timer *self = *((Timer**)dubL_checksdata(L, 1, "mimas.Timer"));
    if (!self) throw dub::Exception("Using deleted mimas.Timer in start");
    int timeout = dubL_checkint(L, 2);
    self->start(timeout);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "start: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "start: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Timer::start()
 * include/mimas/Timer.h:71
 */
static int Timer_start2(lua_State *L) {
  try {
    Timer *self = *((Timer**)dubL_checksdata(L, 1, "mimas.Timer"));
    if (!self) throw dub::Exception("Using deleted mimas.Timer in start");
    self->start();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "start: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "start: Unknown exception");
  }
  return lua_error(L);
}




/** Overloaded function chooser for start(...) */
static int Timer_start(lua_State *L) {
  int type__ = lua_type(L, 2);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TNUMBER) {
    return Timer_start1(L);
  } else if (top__ < 2) {
    return Timer_start2(L);
  } else {
    // use any to raise errors
    return Timer_start2(L);
  }
}


/** void mimas::Timer::stop()
 * include/mimas/Timer.h:75
 */
static int Timer_stop(lua_State *L) {
  try {
    Timer *self = *((Timer**)dubL_checksdata(L, 1, "mimas.Timer"));
    if (!self) throw dub::Exception("Using deleted mimas.Timer in stop");
    self->stop();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "stop: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "stop: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Timer_member_methods[] = {
  {"name"              , QObject_name},
  {"object"            , QObject_object},
  {"setInterval"       , Timer_setInterval},
  {"setName"           , QObject_setName},
  {"start"             , Timer_start},
  {"stop"              , Timer_stop},
  {"__tostring"        , Timer__tostring},
  {"__gc"              , Timer_destructor},
  {"deleted"           , Timer_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg Timer_namespace_methods[] = {
  {"Timer"             , Timer_Timer},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Timer_core(lua_State *L) {
#else
extern "C" int luaopen_mimas_Timer_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Timer");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Timer_member_methods);
  // save meta-table in mimas.Timer_
  register_mt(L, "mimas", "Timer");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Timer_namespace_methods);


	return 1;
}
