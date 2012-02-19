#include "mdns/Service.h"

#include "lua_cpp_helper.h"





using namespace mdns;


/* ============================ Constructors     ====================== */

/** mdns::Service::Service(std::string service_type, const char *name, int interface_index, const char *type, const char *domain, bool is_add)
 * include/mdns/Service.h:61
 */
static int Service_Service(lua_State *L) {
  try {
    std::string *service_type = *((std::string **)dubL_checksdata(L, 1, "std.string"));
    const char *name = dubL_checkstring(L, 2);
    int interface_index = dubL_checkint(L, 3);
    const char *type = dubL_checkstring(L, 4);
    const char *domain = dubL_checkstring(L, 5);
    bool is_add = lua_toboolean(L, 6);
    Service * retval__ = new Service(*service_type, name, interface_index, type, domain, is_add);
    return retval__->luaInit(L, retval__, "mdns.Service");
  } catch (std::exception &e) {
    lua_pushfstring(L, "Service: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Service: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Service_destructor(lua_State *L) {
  Service **userdata = (Service**)dubL_checksdata_n(L, 1, "mdns.Service");

  
  // custom destructor
  Service *self = *userdata;
  if (self) self->luaDestroy();
  
  *userdata = NULL;
  return 0;
}



// test if class is deleted
static int Service_deleted(lua_State *L) {
  Service **userdata = (Service**)dubL_checksdata_n(L, 1, "mdns.Service");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Service__tostring(lua_State *L) {
  Service **userdata = (Service**)dubL_checksdata_n(L, 1, "mdns.Service");
  
  if (!*userdata) {
    lua_pushstring(L, "<mdns.Service: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mdns.Service: %p %s>", *userdata, (*userdata)->name());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** LuaStackSize mdns::Service::fd(lua_State *L)
 * include/mdns/Service.h:65
 */
static int Service_fd(lua_State *L) {
  try {
    Service *self = *((Service**)dubL_checksdata(L, 1, "mdns.Service"));
    if (!self) throw dub::Exception("Using deleted mdns.Service in fd");
    
    LuaStackSize  retval__ = self->fd(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "fd: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "fd: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mdns::Service::info(lua_State *L)
 * include/mdns/Service.h:89
 */
static int Service_info(lua_State *L) {
  try {
    Service *self = *((Service**)dubL_checksdata(L, 1, "mdns.Service"));
    if (!self) throw dub::Exception("Using deleted mdns.Service in info");
    
    LuaStackSize  retval__ = self->info(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "info: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "info: Unknown exception");
  }
  return lua_error(L);
}



/** bool mdns::Service::isAdd()
 * include/mdns/Service.h:74
 */
static int Service_isAdd(lua_State *L) {
  try {
    Service *self = *((Service**)dubL_checksdata(L, 1, "mdns.Service"));
    if (!self) throw dub::Exception("Using deleted mdns.Service in isAdd");
    bool  retval__ = self->isAdd();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "isAdd: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "isAdd: Unknown exception");
  }
  return lua_error(L);
}



/** const char* mdns::Service::name()
 * include/mdns/Service.h:78
 */
static int Service_name(lua_State *L) {
  try {
    Service *self = *((Service**)dubL_checksdata(L, 1, "mdns.Service"));
    if (!self) throw dub::Exception("Using deleted mdns.Service in name");
    const char * retval__ = self->name();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "name: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "name: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mdns::Service::serviceType(lua_State *L)
 * include/mdns/Service.h:82
 */
static int Service_serviceType(lua_State *L) {
  try {
    Service *self = *((Service**)dubL_checksdata(L, 1, "mdns.Service"));
    if (!self) throw dub::Exception("Using deleted mdns.Service in serviceType");
    
    LuaStackSize  retval__ = self->serviceType(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "serviceType: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "serviceType: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Service_member_methods[] = {
  {"fd"                , Service_fd},
  {"info"              , Service_info},
  {"isAdd"             , Service_isAdd},
  {"name"              , Service_name},
  {"serviceType"       , Service_serviceType},
  {"__tostring"        , Service__tostring},
  {"__gc"              , Service_destructor},
  {"deleted"           , Service_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg Service_namespace_methods[] = {
  {"Service"           , Service_Service},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mdns_Service_core(lua_State *L) {
#else
extern "C" int luaopen_mdns_Service_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mdns.Service");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Service_member_methods);
  // save meta-table in mdns.Service_
  register_mt(L, "mdns", "Service");

  // register class methods in a global namespace table
  luaL_register(L, "mdns", Service_namespace_methods);


	return 1;
}
