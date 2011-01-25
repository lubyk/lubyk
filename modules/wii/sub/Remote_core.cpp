#include "wii/Remote.h"

#include "lua_cpp_helper.h"


using namespace wii;


/* ============================ Constructors     ====================== */

/** wii::Remote::Remote(rubyk::Worker *worker, const char *remote_name)
 * include/wii/Remote.h:56
 */
static int Remote_Remote(lua_State *L) {
  try {
    rubyk::Worker *worker = *((rubyk::Worker **)luaL_checkudata(L, 1, "rubyk.Worker"));
    const char *remote_name = luaL_checkstring(L, 2);
    Remote * retval__ = new Remote(worker, remote_name);
    lua_pushclass<Remote>(L, retval__, "wii.Remote");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "wii.Remote.Remote: %s", e.what());
  } catch (...) {
    return luaL_error(L, "wii.Remote.Remote: Unknown exception");
  }
}


/* ============================ Destructor       ====================== */

static int Remote_destructor(lua_State *L) {
  Remote **userdata = (Remote**)luaL_checkudata(L, 1, "wii.Remote");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Remote__tostring(lua_State *L) {
  Remote **userdata = (Remote**)luaL_checkudata(L, 1, "wii.Remote");
  
  
  lua_pushfstring(L, "<wii.Remote: %p %s>", *userdata, (*userdata)->name());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void wii::Remote::__newindex(lua_State *L)
 * include/wii/Remote.h:67
 */
static int Remote___newindex(lua_State *L) {
  try {
    Remote *self__ = *((Remote**)luaL_checkudata(L, 1, "wii.Remote"));
    
    self__->__newindex(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "wii.Remote.__newindex: %s", e.what());
  } catch (...) {
    return luaL_error(L, "wii.Remote.__newindex: Unknown exception");
  }
}


/** void wii::Remote::acceleration(const char *sensor, float x, float y, float z)
 * include/wii/Remote.h:84
 */
static int Remote_acceleration(lua_State *L) {
  try {
    Remote *self__ = *((Remote**)luaL_checkudata(L, 1, "wii.Remote"));
    const char *sensor = luaL_checkstring(L, 2);
    float x = luaL_checknumber(L, 3);
    float y = luaL_checknumber(L, 4);
    float z = luaL_checknumber(L, 5);
    self__->acceleration(sensor, x, y, z);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "wii.Remote.acceleration: %s", e.what());
  } catch (...) {
    return luaL_error(L, "wii.Remote.acceleration: Unknown exception");
  }
}


/** void wii::Remote::button(const char *type, bool pressed)
 * include/wii/Remote.h:102
 */
static int Remote_button(lua_State *L) {
  try {
    Remote *self__ = *((Remote**)luaL_checkudata(L, 1, "wii.Remote"));
    const char *type = luaL_checkstring(L, 2);
    bool pressed = lua_toboolean(L, 3);
    self__->button(type, pressed);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "wii.Remote.button: %s", e.what());
  } catch (...) {
    return luaL_error(L, "wii.Remote.button: Unknown exception");
  }
}


/** const char* wii::Remote::name() const 
 * include/wii/Remote.h:60
 */
static int Remote_name(lua_State *L) {
  try {
    Remote *self__ = *((Remote**)luaL_checkudata(L, 1, "wii.Remote"));
    const char * retval__ = self__->name();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "wii.Remote.name: %s", e.what());
  } catch (...) {
    return luaL_error(L, "wii.Remote.name: Unknown exception");
  }
}


/** void wii::Remote::set_leds(bool led1, bool led2, bool led3, bool led4)
 * include/wii/Remote.h:118
 */
static int Remote_set_leds(lua_State *L) {
  try {
    Remote *self__ = *((Remote**)luaL_checkudata(L, 1, "wii.Remote"));
    bool led1 = lua_toboolean(L, 2);
    bool led2 = lua_toboolean(L, 3);
    bool led3 = lua_toboolean(L, 4);
    bool led4 = lua_toboolean(L, 5);
    self__->set_leds(led1, led2, led3, led4);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "wii.Remote.set_leds: %s", e.what());
  } catch (...) {
    return luaL_error(L, "wii.Remote.set_leds: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Remote_member_methods[] = {
  {"__newindex"        , Remote___newindex},
  {"acceleration"      , Remote_acceleration},
  {"button"            , Remote_button},
  {"name"              , Remote_name},
  {"set_leds"          , Remote_set_leds},
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

  // register class methods in a global namespace table
  luaL_register(L, "wii", Remote_namespace_methods);


	return 1;
}
