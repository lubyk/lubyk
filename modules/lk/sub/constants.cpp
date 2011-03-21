#include "lk/constants.h"

#include "lua_cpp_helper.h"

using namespace lk;



/** LuaStackSize lk::findfile(lua_State *L)
 * include/lk/constants.h:38
 */
static int lk_findfile(lua_State *L) {
  try {
    
    LuaStackSize  retval__ = findfile(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.findfile: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.findfile: Unknown exception");
  }
}



// Register namespace

static const struct luaL_Reg lk_functions[] = {

  {"findfile"                      , lk_findfile},

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
