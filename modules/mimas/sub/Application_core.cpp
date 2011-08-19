#include "mimas/Application.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */




/* ============================ Destructor       ====================== */

static int Application_destructor(lua_State *L) {
  Application **userdata = (Application**)dubL_checksdata_n(L, 1, "mimas.Application");

  
  // custom destructor
  if (*userdata) (*userdata)->dub_destroy();
  
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


/** static LuaStackSize mimas::Application::MakeApplication(lubyk::Worker *worker, lua_State *L)
 * include/mimas/Application.h:64
 */
static int Application_MakeApplication(lua_State *L) {
  try {
    lubyk::Worker *worker = *((lubyk::Worker **)dubL_checksdata(L, 1, "lubyk.Worker"));
    
    LuaStackSize  retval__ = Application::MakeApplication(worker, L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Application.MakeApplication: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Application.MakeApplication: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::Application::exec()
 * include/mimas/Application.h:89
 */
static int Application_exec(lua_State *L) {
  try {
    Application *self__ = *((Application**)dubL_checksdata(L, 1, "mimas.Application"));
    if (!self__) throw dub::Exception("Using deleted mimas.Application in exec");
    int  retval__ = self__->exec();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Application.exec: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Application.exec: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Application::post(lua_State *L)
 * include/mimas/Application.h:101
 */
static int Application_post(lua_State *L) {
  try {
    Application *self__ = *((Application**)dubL_checksdata(L, 1, "mimas.Application"));
    if (!self__) throw dub::Exception("Using deleted mimas.Application in post");
    
    self__->post(L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Application.post: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Application.post: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Application::quit()
 * include/mimas/Application.h:107
 */
static int Application_quit(lua_State *L) {
  try {
    Application *self__ = *((Application**)dubL_checksdata(L, 1, "mimas.Application"));
    if (!self__) throw dub::Exception("Using deleted mimas.Application in quit");
    self__->quit();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Application.quit: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Application.quit: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::Application::screenSize(lua_State *L)
 * include/mimas/Application.h:117
 */
static int Application_screenSize(lua_State *L) {
  try {
    Application *self__ = *((Application**)dubL_checksdata(L, 1, "mimas.Application"));
    if (!self__) throw dub::Exception("Using deleted mimas.Application in screenSize");
    
    LuaStackSize  retval__ = self__->screenSize(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Application.screenSize: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Application.screenSize: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Application::setStyleSheet(const char *text)
 * include/mimas/Application.h:111
 */
static int Application_setStyleSheet(lua_State *L) {
  try {
    Application *self__ = *((Application**)dubL_checksdata(L, 1, "mimas.Application"));
    if (!self__) throw dub::Exception("Using deleted mimas.Application in setStyleSheet");
    const char *text = dubL_checkstring(L, 2);
    self__->setStyleSheet(text);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Application.setStyleSheet: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Application.setStyleSheet: Unknown exception");
  }
  return lua_error(L);
}



/** static void mimas::Application::terminate(int sig)
 * include/mimas/Application.h:96
 */
static int Application_terminate(lua_State *L) {
  try {
    int sig = dubL_checkint(L, 1);
    Application::terminate(sig);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Application.terminate: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Application.terminate: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Application_member_methods[] = {
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
