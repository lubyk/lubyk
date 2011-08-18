#include "lk/constants.h"

#include "lua_cpp_helper.h"

using namespace lk;



/** LuaStackSize lk::findFile(lua_State *L)
 * include/lk/constants.h:60
 */
static int lk_findFile(lua_State *L) {
  try {
    
    LuaStackSize  retval__ = findFile(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.findFile: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.findFile: Unknown exception");
  }
}




/** static const char* lk::pushnexttemplate(lua_State *L, const char *path)
 * include/lk/constants.h:47
 */
static int lk_pushnexttemplate(lua_State *L) {
  try {
    
    const char *path = dubL_checkstring(L, 2);
    const char * retval__ = pushnexttemplate(L, path);
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.pushnexttemplate: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.pushnexttemplate: Unknown exception");
  }
}




/** static int lk::readable(const char *filename)
 * include/lk/constants.h:39
 */
static int lk_readable(lua_State *L) {
  try {
    const char *filename = dubL_checkstring(L, 1);
    int  retval__ = readable(filename);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.readable: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.readable: Unknown exception");
  }
}




// Register namespace

static const struct luaL_Reg lk_functions[] = {

  {"findFile"                      , lk_findFile},
  {"pushnexttemplate"              , lk_pushnexttemplate},
  {"readable"                      , lk_readable},

  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_lk_constants(lua_State *L) {
#else
extern "C" int luaopen_lk_constants(lua_State *L) {
#endif
  // register functions
  luaL_register(L, "lk", lk_functions);


  return 0;
}
