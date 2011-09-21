#include "mdns/Registration.h"

#include "lua_cpp_helper.h"





using namespace mdns;


/* ============================ Constructors     ====================== */

/** mdns::Registration::Registration(const char *service_type, const char *name, uint port)
 * include/mdns/Registration.h:50
 */
static int Registration_Registration(lua_State *L) {
  try {
    const char *service_type = dubL_checkstring(L, 1);
    const char *name = dubL_checkstring(L, 2);
    uint port = dubL_checkint(L, 3);
    Registration * retval__ = new Registration(service_type, name, port);
    return retval__->luaInit(L, retval__, "mdns.Registration");
  } catch (std::exception &e) {
    lua_pushfstring(L, "Registration: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Registration: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Registration_destructor(lua_State *L) {
  Registration **userdata = (Registration**)dubL_checksdata_n(L, 1, "mdns.Registration");

  
  // custom destructor
  Registration *self = *userdata;
  if (self) self->luaDestroy();
  
  *userdata = NULL;
  return 0;
}



// test if class is deleted
static int Registration_deleted(lua_State *L) {
  Registration **userdata = (Registration**)dubL_checksdata_n(L, 1, "mdns.Registration");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Registration__tostring(lua_State *L) {
  Registration **userdata = (Registration**)dubL_checksdata_n(L, 1, "mdns.Registration");
  
  if (!*userdata) {
    lua_pushstring(L, "<mdns.Registration: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mdns.Registration: %p %s>", *userdata, (*userdata)->name());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** int mdns::Registration::fd()
 * include/mdns/Registration.h:56
 */
static int Registration_fd(lua_State *L) {
  try {
    Registration *self = *((Registration**)dubL_checksdata(L, 1, "mdns.Registration"));
    if (!self) throw dub::Exception("Using deleted mdns.Registration in fd");
    int  retval__ = self->fd();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "fd: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "fd: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mdns::Registration::getService(lua_State *L)
 * include/mdns/Registration.h:62
 */
static int Registration_getService(lua_State *L) {
  try {
    Registration *self = *((Registration**)dubL_checksdata(L, 1, "mdns.Registration"));
    if (!self) throw dub::Exception("Using deleted mdns.Registration in getService");
    
    LuaStackSize  retval__ = self->getService(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "getService: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "getService: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Registration_member_methods[] = {
  {"fd"                , Registration_fd},
  {"getService"        , Registration_getService},
  {"__tostring"        , Registration__tostring},
  {"__gc"              , Registration_destructor},
  {"deleted"           , Registration_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg Registration_namespace_methods[] = {
  {"Registration"      , Registration_Registration},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mdns_Registration_core(lua_State *L) {
#else
extern "C" int luaopen_mdns_Registration_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mdns.Registration");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Registration_member_methods);
  // save meta-table in mdns.Registration_
  register_mt(L, "mdns", "Registration");

  // register class methods in a global namespace table
  luaL_register(L, "mdns", Registration_namespace_methods);


	return 1;
}
