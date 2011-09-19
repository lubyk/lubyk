#include "mimas/Callback.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Callback::Callback()
 * include/mimas/Callback.h:59
 */
static int Callback_Callback(lua_State *L) {
  try {
    Callback * retval__ = new Callback();
    return retval__->luaInit(L, retval__, "mimas.Callback");
  } catch (std::exception &e) {
    lua_pushfstring(L, "Callback: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Callback: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Callback_destructor(lua_State *L) {
  Callback **userdata = (Callback**)dubL_checksdata_n(L, 1, "mimas.Callback");

  
  // custom destructor
  Callback *self = *userdata;
  if (self) self->luaDestroy();
  
  *userdata = NULL;
  return 0;
}



// test if class is deleted
static int Callback_deleted(lua_State *L) {
  Callback **userdata = (Callback**)dubL_checksdata_n(L, 1, "mimas.Callback");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Callback__tostring(lua_State *L) {
  Callback **userdata = (Callback**)dubL_checksdata_n(L, 1, "mimas.Callback");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.Callback: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.Callback: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::Callback::connect(QObject *obj, const char *method, const char *callback)
 * include/mimas/Callback.h:65
 */
static int Callback_connect(lua_State *L) {
  try {
    Callback *self = *((Callback**)dubL_checksdata(L, 1, "mimas.Callback"));
    if (!self) throw dub::Exception("Using deleted mimas.Callback in connect");
    QObject *obj = *((QObject **)dubL_checksdata(L, 2, "mimas.QObject"));
    const char *method = dubL_checkstring(L, 3);
    const char *callback = dubL_checkstring(L, 4);
    self->connect(obj, method, callback);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "connect: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "connect: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QObject::name()
 * mimas/bind/QObject.h:12
 */
static int QObject_name(lua_State *L) {
  try {
    Callback *self = *((Callback**)dubL_checksdata(L, 1, "mimas.Callback"));
    if (!self) throw dub::Exception("Using deleted mimas.Callback in name");
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
    Callback *self = *((Callback**)dubL_checksdata(L, 1, "mimas.Callback"));
    if (!self) throw dub::Exception("Using deleted mimas.Callback in object");
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



/** void mimas::QObject::setName(const char *name)
 * mimas/bind/QObject.h:13
 */
static int QObject_setName(lua_State *L) {
  try {
    Callback *self = *((Callback**)dubL_checksdata(L, 1, "mimas.Callback"));
    if (!self) throw dub::Exception("Using deleted mimas.Callback in setName");
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





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Callback_member_methods[] = {
  {"connect"           , Callback_connect},
  {"name"              , QObject_name},
  {"object"            , QObject_object},
  {"setName"           , QObject_setName},
  {"__tostring"        , Callback__tostring},
  {"__gc"              , Callback_destructor},
  {"deleted"           , Callback_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg Callback_namespace_methods[] = {
  {"Callback"          , Callback_Callback},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Callback_core(lua_State *L) {
#else
extern "C" int luaopen_mimas_Callback_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Callback");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Callback_member_methods);
  // save meta-table in mimas.Callback_
  register_mt(L, "mimas", "Callback");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Callback_namespace_methods);


	return 1;
}
