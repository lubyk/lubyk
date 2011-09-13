#include "lk/Debug.h"

#include "lua_cpp_helper.h"





using namespace lk;


/* ============================ Constructors     ====================== */

/** lk::Debug::Debug(lua_State *L)
 * include/lk/Debug.h:44
 */
static int Debug_Debug(lua_State *L) {
  try {
    
    Debug * retval__ = new Debug(L);
    // The class inherits from 'LuaCallback', use lua_init instead of pushclass.
    return retval__->luaInit(L, retval__, "lk.Debug");
  } catch (std::exception &e) {
    lua_pushfstring(L, "Debug: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Debug: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Debug_destructor(lua_State *L) {
  Debug **userdata = (Debug**)dubL_checksdata_n(L, 1, "lk.Debug");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Debug__tostring(lua_State *L) {
  Debug **userdata = (Debug**)dubL_checksdata_n(L, 1, "lk.Debug");
  
  
  lua_pushfstring(L, "<lk.Debug: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Debug_member_methods[] = {
  {"__tostring"        , Debug__tostring},
  {"__gc"              , Debug_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Debug_namespace_methods[] = {
  {"Debug"             , Debug_Debug},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_lk_Debug_core(lua_State *L) {
#else
extern "C" int luaopen_lk_Debug_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "lk.Debug");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Debug_member_methods);
  // save meta-table in lk.Debug_
  register_mt(L, "lk", "Debug");

  // register class methods in a global namespace table
  luaL_register(L, "lk", Debug_namespace_methods);


	return 1;
}
