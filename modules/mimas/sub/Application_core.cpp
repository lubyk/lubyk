#include "mimas/Application.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */




/* ============================ Destructor       ====================== */

static int Application_destructor(lua_State *L) {
  Application **userdata = (Application**)dubL_checksdata_n(L, 1, "mimas.Application");

  
  // custom destructor
  if (*userdata) (*userdata)->luaDestroy();
  
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
 * include/mimas/Application.h:61
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
 * include/mimas/Application.h:75
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
 * include/mimas/Application.h:82
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



/** void mimas::Application::post(lua_State *L)
 * include/mimas/Application.h:93
 */
static int Application_post(lua_State *L) {
  try {
    Application *self = *((Application**)dubL_checksdata(L, 1, "mimas.Application"));
    if (!self) throw dub::Exception("Using deleted mimas.Application in post");
    
    self->post(L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "post: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "post: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Application::quit()
 * include/mimas/Application.h:102
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
 * include/mimas/Application.h:112
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



/** void mimas::Application::setStyleSheet(const char *text)
 * include/mimas/Application.h:106
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



/** static void mimas::Application::terminate(int sig)
 * include/mimas/Application.h:89
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
  {"post"              , Application_post},
  {"quit"              , Application_quit},
  {"screenSize"        , Application_screenSize},
  {"setStyleSheet"     , Application_setStyleSheet},
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


	return 1;
}
