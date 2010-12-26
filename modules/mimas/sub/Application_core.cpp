#include "mimas/Application.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Application::Application(rubyk::Worker *worker)
 * include/mimas/Application.h:59
 */
static int Application_Application(lua_State *L) {
  try {
    rubyk::Worker *worker = *((rubyk::Worker **)luaL_checkudata(L, 1, "rubyk.Worker"));
    Application * retval__ = new Application(worker);
    lua_pushclass<Application>(L, retval__, "mimas.Application");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Application.Application: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Application.Application: Unknown exception");
    lua_error(L);
    return 0;
  }
}

/* ============================ Destructor       ====================== */

static int Application_destructor(lua_State *L) {
  Application **userdata = (Application**)luaL_checkudata(L, 1, "mimas.Application");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Application__tostring(lua_State *L) {
  Application **userdata = (Application**)luaL_checkudata(L, 1, "mimas.Application");
  
  lua_pushfstring(L, "<mimas.Application: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** int mimas::Application::exec()
 * include/mimas/Application.h:65
 */
static int Application_exec(lua_State *L) {
  try {
    Application *self__ = *((Application**)luaL_checkudata(L, 1, "mimas.Application"));
    lua_remove(L, 1);
    int  retval__ = self__->exec();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Application.exec: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Application.exec: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Application::post(int lua_func_idx)
 * include/mimas/Application.h:70
 */
static int Application_post(lua_State *L) {
  try {
    Application *self__ = *((Application**)luaL_checkudata(L, 1, "mimas.Application"));
    lua_remove(L, 1);
    
    luaL_checktype(L, 1, LUA_TFUNCTION);
    // push on top
    lua_pushvalue(L, 1);
    int lua_func_idx = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_pop(L, 1);
    
    self__->post(lua_func_idx);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Application.post: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Application.post: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Application::quit()
 * include/mimas/Application.h:76
 */
static int Application_quit(lua_State *L) {
  try {
    Application *self__ = *((Application**)luaL_checkudata(L, 1, "mimas.Application"));
    lua_remove(L, 1);
    self__->quit();
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Application.quit: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Application.quit: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Application_member_methods[] = {
  {"exec"              , Application_exec},
  {"post"              , Application_post},
  {"quit"              , Application_quit},
  {"__tostring"        , Application__tostring},
  {"__gc"              , Application_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Application_namespace_methods[] = {
  {"Application"       , Application_Application},
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

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Application_namespace_methods);


	return 1;
}
