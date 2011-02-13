#include "wii/Browser.h"

#include "lua_cpp_helper.h"


using namespace wii;


/* ============================ Constructors     ====================== */

/** wii::Browser::Browser(lubyk::Worker *worker)
 * include/wii/Browser.h:50
 */
static int Browser_Browser(lua_State *L) {
  try {
    lubyk::Worker *worker = *((lubyk::Worker **)luaL_checkudata(L, 1, "lubyk.Worker"));
    Browser * retval__ = new Browser(worker);
    lua_pushclass<Browser>(L, retval__, "wii.Browser");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "wii.Browser.Browser: %s", e.what());
  } catch (...) {
    return luaL_error(L, "wii.Browser.Browser: Unknown exception");
  }
}


/* ============================ Destructor       ====================== */

static int Browser_destructor(lua_State *L) {
  Browser **userdata = (Browser**)luaL_checkudata(L, 1, "wii.Browser");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Browser__tostring(lua_State *L) {
  Browser **userdata = (Browser**)luaL_checkudata(L, 1, "wii.Browser");
  
  
  lua_pushfstring(L, "<wii.Browser: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void wii::Browser::__newindex(lua_State *L)
 * include/wii/Browser.h:57
 */
static int Browser___newindex(lua_State *L) {
  try {
    Browser *self__ = *((Browser**)luaL_checkudata(L, 1, "wii.Browser"));
    
    self__->__newindex(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "wii.Browser.__newindex: %s", e.what());
  } catch (...) {
    return luaL_error(L, "wii.Browser.__newindex: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Browser_member_methods[] = {
  {"__newindex"        , Browser___newindex},
  {"__tostring"        , Browser__tostring},
  {"__gc"              , Browser_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Browser_namespace_methods[] = {
  {"Browser"           , Browser_Browser},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_wii_Browser_core(lua_State *L) {
#else
extern "C" int luaopen_wii_Browser_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "wii.Browser");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Browser_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "wii", Browser_namespace_methods);


	return 1;
}
