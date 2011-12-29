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

  
  // custom destructor
  Debug *self = *userdata;
  if (self) self->luaDestroy();
  
  *userdata = NULL;
  return 0;
}



// test if class is deleted
static int Debug_deleted(lua_State *L) {
  Debug **userdata = (Debug**)dubL_checksdata_n(L, 1, "lk.Debug");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Debug__tostring(lua_State *L) {
  Debug **userdata = (Debug**)dubL_checksdata_n(L, 1, "lk.Debug");
  
  if (!*userdata) {
    lua_pushstring(L, "<lk.Debug: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<lk.Debug: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** static LuaStackSize lk::Debug::userdata(lua_State *L)
 * include/lk/Debug.h:75
 */
static int Debug_userdata(lua_State *L) {
  try {
    
    LuaStackSize  retval__ = Debug::userdata(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "userdata: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "userdata: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Debug_member_methods[] = {
  {"__tostring"        , Debug__tostring},
  {"__gc"              , Debug_destructor},
  {"deleted"           , Debug_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg Debug_namespace_methods[] = {
  {"Debug"             , Debug_Debug},
  {"Debug_userdata"    , Debug_userdata},
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
