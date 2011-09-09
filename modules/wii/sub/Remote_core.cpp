#include "wii/Remote.h"

#include "lua_cpp_helper.h"





using namespace wii;


/* ============================ Constructors     ====================== */

/** wii::Remote::Remote(const char *remote_name=NULL)
 * include/wii/Remote.h:53
 */
static int Remote_Remote(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Remote * retval__;
    if (top__ < 1) {
      retval__ = new Remote();
    } else {
      const char *remote_name = dubL_checkstring(L, 1);
      retval__ = new Remote(remote_name);
    }
    // The class inherits from 'LuaCallback', use lua_init instead of pushclass.
    return retval__->luaInit(L, retval__, "wii.Remote");
  } catch (std::exception &e) {
    lua_pushfstring(L, "Remote: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Remote: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Remote_destructor(lua_State *L) {
  Remote **userdata = (Remote**)dubL_checksdata_n(L, 1, "wii.Remote");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Remote__tostring(lua_State *L) {
  Remote **userdata = (Remote**)dubL_checksdata_n(L, 1, "wii.Remote");
  
  
  lua_pushfstring(L, "<wii.Remote: %p %s>", *userdata, (*userdata)->name());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void wii::Remote::connected()
 * include/wii/Remote.h:122
 */
static int Remote_connected(lua_State *L) {
  try {
    Remote *self = *((Remote**)dubL_checksdata(L, 1, "wii.Remote"));
    self->connected();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "connected: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "connected: Unknown exception");
  }
  return lua_error(L);
}



/** void wii::Remote::disconnect()
 * include/wii/Remote.h:61
 */
static int Remote_disconnect(lua_State *L) {
  try {
    Remote *self = *((Remote**)dubL_checksdata(L, 1, "wii.Remote"));
    self->disconnect();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "disconnect: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "disconnect: Unknown exception");
  }
  return lua_error(L);
}



/** const char* wii::Remote::name() const 
 * include/wii/Remote.h:57
 */
static int Remote_name(lua_State *L) {
  try {
    Remote *self = *((Remote**)dubL_checksdata(L, 1, "wii.Remote"));
    const char * retval__ = self->name();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "name: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "name: Unknown exception");
  }
  return lua_error(L);
}



/** void wii::Remote::setLeds(bool led1, bool led2, bool led3, bool led4)
 * include/wii/Remote.h:65
 */
static int Remote_setLeds(lua_State *L) {
  try {
    Remote *self = *((Remote**)dubL_checksdata(L, 1, "wii.Remote"));
    bool led1 = lua_toboolean(L, 2);
    bool led2 = lua_toboolean(L, 3);
    bool led3 = lua_toboolean(L, 4);
    bool led4 = lua_toboolean(L, 5);
    self->setLeds(led1, led2, led3, led4);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setLeds: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setLeds: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Remote_member_methods[] = {
  {"connected"         , Remote_connected},
  {"disconnect"        , Remote_disconnect},
  {"name"              , Remote_name},
  {"setLeds"           , Remote_setLeds},
  {"__tostring"        , Remote__tostring},
  {"__gc"              , Remote_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Remote_namespace_methods[] = {
  {"Remote"            , Remote_Remote},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_wii_Remote_core(lua_State *L) {
#else
extern "C" int luaopen_wii_Remote_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "wii.Remote");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Remote_member_methods);
  // save meta-table in wii.Remote_
  register_mt(L, "wii", "Remote");

  // register class methods in a global namespace table
  luaL_register(L, "wii", Remote_namespace_methods);


	return 1;
}
