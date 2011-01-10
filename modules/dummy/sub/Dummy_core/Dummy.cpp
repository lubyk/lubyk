#include "dummy/Dummy.h"

#include "lua_cpp_helper.h"


using namespace dummy;


/* ============================ Constructors     ====================== */

/** dummy::Dummy::Dummy(rubyk::Worker *worker)
 * include/dummy/dummy.h:48
 */
static int Dummy_Dummy(lua_State *L) {
  try {
    rubyk::Worker *worker = *((rubyk::Worker **)luaL_checkudata(L, 1, "rubyk.Worker"));
    Dummy * retval__ = new Dummy(worker);
    lua_pushclass<Dummy>(L, retval__, "dummy.Dummy");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "dummy.Dummy.Dummy: %s", e.what());
  } catch (...) {
    return luaL_error(L, "dummy.Dummy.Dummy: Unknown exception");
  }
}


/* ============================ Destructor       ====================== */

static int Dummy_destructor(lua_State *L) {
  Dummy **userdata = (Dummy**)luaL_checkudata(L, 1, "dummy.Dummy");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Dummy__tostring(lua_State *L) {
  Dummy **userdata = (Dummy**)luaL_checkudata(L, 1, "dummy.Dummy");
  
  
  lua_pushfstring(L, "<dummy.Dummy: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void dummy::Dummy::__newindex(lua_State *L)
 * include/dummy/dummy.h:53
 */
static int Dummy___newindex(lua_State *L) {
  try {
    Dummy *self__ = *((Dummy**)luaL_checkudata(L, 1, "dummy.Dummy"));
    
    self__->__newindex(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "dummy.Dummy.__newindex: %s", e.what());
  } catch (...) {
    return luaL_error(L, "dummy.Dummy.__newindex: Unknown exception");
  }
}


/** void dummy::Dummy::callback()
 * include/dummy/dummy.h:68
 */
static int Dummy_callback(lua_State *L) {
  try {
    Dummy *self__ = *((Dummy**)luaL_checkudata(L, 1, "dummy.Dummy"));
    self__->callback();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "dummy.Dummy.callback: %s", e.what());
  } catch (...) {
    return luaL_error(L, "dummy.Dummy.callback: Unknown exception");
  }
}


/** static const char* dummy::Dummy::plat()
 * include/dummy/dummy.h:83
 */
static int Dummy_plat(lua_State *L) {
  try {
    const char * retval__ = Dummy::plat();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "dummy.Dummy.plat: %s", e.what());
  } catch (...) {
    return luaL_error(L, "dummy.Dummy.plat: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Dummy_member_methods[] = {
  {"__newindex"        , Dummy___newindex},
  {"callback"          , Dummy_callback},
  {"__tostring"        , Dummy__tostring},
  {"__gc"              , Dummy_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Dummy_namespace_methods[] = {
  {"Dummy"             , Dummy_Dummy},
  {"Dummy_plat"        , Dummy_plat},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_dummy_Dummy_core(lua_State *L) {
#else
extern "C" int luaopen_dummy_Dummy_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "dummy.Dummy");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Dummy_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "dummy", Dummy_namespace_methods);


	return 1;
}
