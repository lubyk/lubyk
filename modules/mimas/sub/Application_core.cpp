#include "mimas/Application.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */




/* ============================ Destructor       ====================== */

static int Application_destructor(lua_State *L) {
  Application **userdata = (Application**)dubL_checksdata_n(L, 1, "mimas.Application");

  
  // custom destructor
  Application *self = *userdata;
  if (self) self->luaDestroy();
  
  *userdata = NULL;
  return 0;
}



// test if class is deleted
static int Application_deleted(lua_State *L) {
  Application **userdata = (Application**)dubL_checksdata_n(L, 1, "mimas.Application");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Application__tostring(lua_State *L) {
  Application **userdata = (Application**)dubL_checksdata_n(L, 1, "mimas.Application");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.Application: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.Application: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** static LuaStackSize mimas::Application::MakeApplication(lua_State *L)
 * include/mimas/Application.h:69
 */
static int Application_MakeApplication(lua_State *L) {
  try {
    
    LuaStackSize  retval__ = Application::MakeApplication(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "MakeApplication: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "MakeApplication: Unknown exception");
  }
  return lua_error(L);
}



/** virtual void mimas::Application::dub_destroy()
 * include/mimas/Application.h:95
 */
static int Application_dub_destroy(lua_State *L) {
  try {
    Application *self = *((Application**)dubL_checksdata(L, 1, "mimas.Application"));
    if (!self) throw dub::Exception("Using deleted mimas.Application in dub_destroy");
    self->dub_destroy();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "dub_destroy: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "dub_destroy: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::Application::exec()
 * include/mimas/Application.h:102
 */
static int Application_exec(lua_State *L) {
  try {
    Application *self = *((Application**)dubL_checksdata(L, 1, "mimas.Application"));
    if (!self) throw dub::Exception("Using deleted mimas.Application in exec");
    int  retval__ = self->exec();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "exec: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "exec: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QObject::name()
 * mimas/bind/QObject.h:12
 */
static int QObject_name(lua_State *L) {
  try {
    Application *self = *((Application**)dubL_checksdata(L, 1, "mimas.Application"));
    if (!self) throw dub::Exception("Using deleted mimas.Application in name");
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
    Application *self = *((Application**)dubL_checksdata(L, 1, "mimas.Application"));
    if (!self) throw dub::Exception("Using deleted mimas.Application in object");
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



/** void mimas::Application::processEvents(int maxtime)
 * include/mimas/Application.h:107
 */
static int Application_processEvents(lua_State *L) {
  try {
    Application *self = *((Application**)dubL_checksdata(L, 1, "mimas.Application"));
    if (!self) throw dub::Exception("Using deleted mimas.Application in processEvents");
    int maxtime = dubL_checkint(L, 2);
    self->processEvents(maxtime);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "processEvents: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "processEvents: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Application::quit()
 * include/mimas/Application.h:122
 */
static int Application_quit(lua_State *L) {
  try {
    Application *self = *((Application**)dubL_checksdata(L, 1, "mimas.Application"));
    if (!self) throw dub::Exception("Using deleted mimas.Application in quit");
    self->quit();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "quit: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "quit: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::Application::screenSize(lua_State *L)
 * include/mimas/Application.h:132
 */
static int Application_screenSize(lua_State *L) {
  try {
    Application *self = *((Application**)dubL_checksdata(L, 1, "mimas.Application"));
    if (!self) throw dub::Exception("Using deleted mimas.Application in screenSize");
    
    LuaStackSize  retval__ = self->screenSize(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "screenSize: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "screenSize: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QObject::setName(const char *name)
 * mimas/bind/QObject.h:13
 */
static int QObject_setName(lua_State *L) {
  try {
    Application *self = *((Application**)dubL_checksdata(L, 1, "mimas.Application"));
    if (!self) throw dub::Exception("Using deleted mimas.Application in setName");
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



/** void mimas::Application::setStyleSheet(const char *text)
 * include/mimas/Application.h:126
 */
static int Application_setStyleSheet(lua_State *L) {
  try {
    Application *self = *((Application**)dubL_checksdata(L, 1, "mimas.Application"));
    if (!self) throw dub::Exception("Using deleted mimas.Application in setStyleSheet");
    const char *text = dubL_checkstring(L, 2);
    self->setStyleSheet(text);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setStyleSheet: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setStyleSheet: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Application::singleShot(int msec, QObject *receiver, const char *member)
 * include/mimas/Application.h:143
 */
static int Application_singleShot(lua_State *L) {
  try {
    Application *self = *((Application**)dubL_checksdata(L, 1, "mimas.Application"));
    if (!self) throw dub::Exception("Using deleted mimas.Application in singleShot");
    int msec = dubL_checkint(L, 2);
    QObject *receiver = *((QObject **)dubL_checksdata(L, 3, "mimas.QObject"));
    const char *member = dubL_checkstring(L, 4);
    self->singleShot(msec, receiver, member);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "singleShot: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "singleShot: Unknown exception");
  }
  return lua_error(L);
}



/** static void mimas::Application::terminate(int sig)
 * include/mimas/Application.h:120
 */
static int Application_terminate(lua_State *L) {
  try {
    int sig = dubL_checkint(L, 1);
    Application::terminate(sig);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "terminate: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "terminate: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Application_member_methods[] = {
  {"dub_destroy"       , Application_dub_destroy},
  {"exec"              , Application_exec},
  {"name"              , QObject_name},
  {"object"            , QObject_object},
  {"processEvents"     , Application_processEvents},
  {"quit"              , Application_quit},
  {"screenSize"        , Application_screenSize},
  {"setName"           , QObject_setName},
  {"setStyleSheet"     , Application_setStyleSheet},
  {"singleShot"        , Application_singleShot},
  {"__tostring"        , Application__tostring},
  {"__gc"              , Application_destructor},
  {"deleted"           , Application_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg Application_namespace_methods[] = {
  {"Application"       , Application_MakeApplication},
  {"Application_terminate", Application_terminate},
  {NULL, NULL},
};


static const struct lua_constants_Reg Application_namespace_constants[] = {
  {"Desktop"           , mimas::Application::Desktop},
  {"Plugin"            , mimas::Application::Plugin},
  {NULL, NULL},
};


#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Application_core(lua_State *L) {
#else
extern "C" int luaopen_mimas_Application_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Application");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Application_member_methods);
  // save meta-table in mimas.Application_
  register_mt(L, "mimas", "Application");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Application_namespace_methods);


  // register class enums
  register_constants(L, "mimas.Application_const", Application_namespace_constants);

	return 1;
}
