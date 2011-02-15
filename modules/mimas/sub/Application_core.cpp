#include "mimas/Application.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */




/* ============================ Destructor       ====================== */

static int Application_destructor(lua_State *L) {
  Application **userdata = (Application**)luaL_checkudata(L, 1, "mimas.Application");

  
  // custom destructor
  if (*userdata) (*userdata)->dub_destroy();
  
  *userdata = NULL;
  return 0;
}




// test if class is deleted
static int Application_deleted(lua_State *L) {
  Application **userdata = (Application**)luaL_checkudata(L, 1, "mimas.Application");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Application__tostring(lua_State *L) {
  Application **userdata = (Application**)luaL_checkudata(L, 1, "mimas.Application");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.Application: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.Application: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** static LuaStackSize mimas::Application::MakeApplication(lubyk::Worker *worker, lua_State *L)
 * include/mimas/Application.h:72
 */
static int Application_MakeApplication(lua_State *L) {
  try {
    lubyk::Worker *worker = *((lubyk::Worker **)luaL_checkudata(L, 1, "lubyk.Worker"));
    
    LuaStackSize  retval__ = Application::MakeApplication(worker, L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Application.MakeApplication: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Application.MakeApplication: Unknown exception");
  }
}


/** int mimas::Application::exec()
 * include/mimas/Application.h:97
 */
static int Application_exec(lua_State *L) {
  try {
    Application *self__ = *((Application**)luaL_checkudata(L, 1, "mimas.Application"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Application in exec");
    int  retval__ = self__->exec();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Application.exec: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Application.exec: Unknown exception");
  }
}


/** void mimas::Application::post(lua_State *L)
 * include/mimas/Application.h:106
 */
static int Application_post(lua_State *L) {
  try {
    Application *self__ = *((Application**)luaL_checkudata(L, 1, "mimas.Application"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Application in post");
    
    self__->post(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Application.post: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Application.post: Unknown exception");
  }
}


/** void mimas::Application::quit()
 * include/mimas/Application.h:112
 */
static int Application_quit(lua_State *L) {
  try {
    Application *self__ = *((Application**)luaL_checkudata(L, 1, "mimas.Application"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Application in quit");
    self__->quit();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Application.quit: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Application.quit: Unknown exception");
  }
}


/** void mimas::Application::setStyleSheet(const char *text)
 * include/mimas/Application.h:116
 */
static int Application_setStyleSheet(lua_State *L) {
  try {
    Application *self__ = *((Application**)luaL_checkudata(L, 1, "mimas.Application"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Application in setStyleSheet");
    const char *text = luaL_checkstring(L, 2);
    self__->setStyleSheet(text);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Application.setStyleSheet: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Application.setStyleSheet: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Application_member_methods[] = {
  {"exec"              , Application_exec},
  {"post"              , Application_post},
  {"quit"              , Application_quit},
  {"setStyleSheet"     , Application_setStyleSheet},
  {"__tostring"        , Application__tostring},
  {"__gc"              , Application_destructor},
  {"deleted"           , Application_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg Application_namespace_methods[] = {
  {"Application"       , Application_MakeApplication},
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
