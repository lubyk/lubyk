#include "mimas/SocketNotifier.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::SocketNotifier::SocketNotifier(int fd, int event_type)
 * include/mimas/SocketNotifier.h:56
 */
static int SocketNotifier_SocketNotifier(lua_State *L) {
  try {
    int fd = dubL_checkint(L, 1);
    int event_type = dubL_checkint(L, 2);
    SocketNotifier * retval__ = new SocketNotifier(fd, event_type);
    return retval__->luaInit(L, retval__, "mimas.SocketNotifier");
  } catch (std::exception &e) {
    lua_pushfstring(L, "SocketNotifier: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "SocketNotifier: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int SocketNotifier_destructor(lua_State *L) {
  SocketNotifier **userdata = (SocketNotifier**)dubL_checksdata_n(L, 1, "mimas.SocketNotifier");

  
  // custom destructor
  SocketNotifier *self = *userdata;
  if (self) self->luaDestroy();
  
  *userdata = NULL;
  return 0;
}



// test if class is deleted
static int SocketNotifier_deleted(lua_State *L) {
  SocketNotifier **userdata = (SocketNotifier**)dubL_checksdata_n(L, 1, "mimas.SocketNotifier");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int SocketNotifier__tostring(lua_State *L) {
  SocketNotifier **userdata = (SocketNotifier**)dubL_checksdata_n(L, 1, "mimas.SocketNotifier");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.SocketNotifier: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.SocketNotifier: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::QObject::name()
 * mimas/bind/QObject.h:12
 */
static int QObject_name(lua_State *L) {
  try {
    SocketNotifier *self = *((SocketNotifier**)dubL_checksdata(L, 1, "mimas.SocketNotifier"));
    if (!self) throw dub::Exception("Using deleted mimas.SocketNotifier in name");
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
    SocketNotifier *self = *((SocketNotifier**)dubL_checksdata(L, 1, "mimas.SocketNotifier"));
    if (!self) throw dub::Exception("Using deleted mimas.SocketNotifier in object");
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



/** void mimas::SocketNotifier::setEnabled(bool enabled)
 * include/mimas/SocketNotifier.h:76
 */
static int SocketNotifier_setEnabled(lua_State *L) {
  try {
    SocketNotifier *self = *((SocketNotifier**)dubL_checksdata(L, 1, "mimas.SocketNotifier"));
    if (!self) throw dub::Exception("Using deleted mimas.SocketNotifier in setEnabled");
    bool enabled = lua_toboolean(L, 2);
    self->setEnabled(enabled);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setEnabled: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setEnabled: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QObject::setName(const char *name)
 * mimas/bind/QObject.h:13
 */
static int QObject_setName(lua_State *L) {
  try {
    SocketNotifier *self = *((SocketNotifier**)dubL_checksdata(L, 1, "mimas.SocketNotifier"));
    if (!self) throw dub::Exception("Using deleted mimas.SocketNotifier in setName");
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



/** int mimas::SocketNotifier::socket() const 
 * include/mimas/SocketNotifier.h:69
 */
static int SocketNotifier_socket(lua_State *L) {
  try {
    SocketNotifier *self = *((SocketNotifier**)dubL_checksdata(L, 1, "mimas.SocketNotifier"));
    if (!self) throw dub::Exception("Using deleted mimas.SocketNotifier in socket");
    int  retval__ = self->socket();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "socket: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "socket: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg SocketNotifier_member_methods[] = {
  {"name"              , QObject_name},
  {"object"            , QObject_object},
  {"setEnabled"        , SocketNotifier_setEnabled},
  {"setName"           , QObject_setName},
  {"socket"            , SocketNotifier_socket},
  {"__tostring"        , SocketNotifier__tostring},
  {"__gc"              , SocketNotifier_destructor},
  {"deleted"           , SocketNotifier_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg SocketNotifier_namespace_methods[] = {
  {"SocketNotifier"    , SocketNotifier_SocketNotifier},
  {NULL, NULL},
};


static const struct lua_constants_Reg SocketNotifier_namespace_constants[] = {
  {"Read"              , mimas::SocketNotifier::Read},
  {"Write"             , mimas::SocketNotifier::Write},
  {NULL, NULL},
};


#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_SocketNotifier_core(lua_State *L) {
#else
extern "C" int luaopen_mimas_SocketNotifier_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.SocketNotifier");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, SocketNotifier_member_methods);
  // save meta-table in mimas.SocketNotifier_
  register_mt(L, "mimas", "SocketNotifier");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", SocketNotifier_namespace_methods);


  // register class enums
  register_constants(L, "mimas.SocketNotifier_const", SocketNotifier_namespace_constants);

	return 1;
}
