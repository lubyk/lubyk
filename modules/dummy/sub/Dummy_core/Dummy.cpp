#include "dummy/Dummy.h"

#include "lua_cpp_helper.h"





using namespace dummy;


/* ============================ Constructors     ====================== */

/** dummy::Dummy::Dummy()
 * include/dummy/dummy.h:44
 */
static int Dummy_Dummy(lua_State *L) {
  try {
    Dummy * retval__ = new Dummy();
    return retval__->luaInit(L, retval__, "dummy.Dummy");
  } catch (std::exception &e) {
    lua_pushfstring(L, "Dummy: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Dummy: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Dummy_destructor(lua_State *L) {
  Dummy **userdata = (Dummy**)dubL_checksdata_n(L, 1, "dummy.Dummy");

  
  // custom destructor
  Dummy *self = *userdata;
  if (self) self->luaDestroy();
  
  *userdata = NULL;
  return 0;
}



// test if class is deleted
static int Dummy_deleted(lua_State *L) {
  Dummy **userdata = (Dummy**)dubL_checksdata_n(L, 1, "dummy.Dummy");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Dummy__tostring(lua_State *L) {
  Dummy **userdata = (Dummy**)dubL_checksdata_n(L, 1, "dummy.Dummy");
  
  if (!*userdata) {
    lua_pushstring(L, "<dummy.Dummy: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<dummy.Dummy: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** float dummy::Dummy::addSomething(float value)
 * include/dummy/dummy.h:68
 */
static int Dummy_addSomething(lua_State *L) {
  try {
    Dummy *self = *((Dummy**)dubL_checksdata(L, 1, "dummy.Dummy"));
    if (!self) throw dub::Exception("Using deleted dummy.Dummy in addSomething");
    float value = dubL_checknumber(L, 2);
    float  retval__ = self->addSomething(value);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "addSomething: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "addSomething: Unknown exception");
  }
  return lua_error(L);
}



/** float dummy::Dummy::callback(const char *func, float value)
 * include/dummy/dummy.h:48
 */
static int Dummy_callback(lua_State *L) {
  try {
    Dummy *self = *((Dummy**)dubL_checksdata(L, 1, "dummy.Dummy"));
    if (!self) throw dub::Exception("Using deleted dummy.Dummy in callback");
    const char *func = dubL_checkstring(L, 2);
    float value = dubL_checknumber(L, 3);
    float  retval__ = self->callback(func, value);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "callback: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "callback: Unknown exception");
  }
  return lua_error(L);
}



/** static const char* dummy::Dummy::plat()
 * include/dummy/dummy.h:72
 */
static int Dummy_plat(lua_State *L) {
  try {
    const char * retval__ = Dummy::plat();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "plat: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "plat: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Dummy_member_methods[] = {
  {"addSomething"      , Dummy_addSomething},
  {"callback"          , Dummy_callback},
  {"__tostring"        , Dummy__tostring},
  {"__gc"              , Dummy_destructor},
  {"deleted"           , Dummy_deleted},
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
  // save meta-table in dummy.Dummy_
  register_mt(L, "dummy", "Dummy");

  // register class methods in a global namespace table
  luaL_register(L, "dummy", Dummy_namespace_methods);


	return 1;
}
