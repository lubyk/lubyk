#include "lk/Finalizer.h"

#include "lua_cpp_helper.h"





using namespace lk;


/* ============================ Constructors     ====================== */

/** lk::Finalizer::Finalizer()
 * include/lk/Finalizer.h:44
 */
static int Finalizer_Finalizer(lua_State *L) {
  try {
    Finalizer * retval__ = new Finalizer();
    return retval__->luaInit(L, retval__, "lk.Finalizer");
  } catch (std::exception &e) {
    lua_pushfstring(L, "Finalizer: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Finalizer: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Finalizer_destructor(lua_State *L) {
  Finalizer **userdata = (Finalizer**)dubL_checksdata_n(L, 1, "lk.Finalizer");

  
  // custom destructor
  Finalizer *self = *userdata;
  if (self) self->finalize(L);
  
  *userdata = NULL;
  return 0;
}



// test if class is deleted
static int Finalizer_deleted(lua_State *L) {
  Finalizer **userdata = (Finalizer**)dubL_checksdata_n(L, 1, "lk.Finalizer");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Finalizer__tostring(lua_State *L) {
  Finalizer **userdata = (Finalizer**)dubL_checksdata_n(L, 1, "lk.Finalizer");
  
  if (!*userdata) {
    lua_pushstring(L, "<lk.Finalizer: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<lk.Finalizer: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Finalizer_member_methods[] = {
  {"__tostring"        , Finalizer__tostring},
  {"__gc"              , Finalizer_destructor},
  {"deleted"           , Finalizer_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg Finalizer_namespace_methods[] = {
  {"Finalizer"         , Finalizer_Finalizer},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_lk_Finalizer_core(lua_State *L) {
#else
extern "C" int luaopen_lk_Finalizer_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "lk.Finalizer");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Finalizer_member_methods);
  // save meta-table in lk.Finalizer_
  register_mt(L, "lk", "Finalizer");

  // register class methods in a global namespace table
  luaL_register(L, "lk", Finalizer_namespace_methods);


	return 1;
}
