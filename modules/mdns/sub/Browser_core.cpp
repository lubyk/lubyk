#include "mdns/Browser.h"

#include "lua_cpp_helper.h"


using namespace mdns;


/* ============================ Constructors     ====================== */




/* ============================ Destructor       ====================== */

static int Browser_destructor(lua_State *L) {
  Browser **userdata = (Browser**)luaL_checkudata(L, 1, "mdns.Browser");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Browser__tostring(lua_State *L) {
  Browser **userdata = (Browser**)luaL_checkudata(L, 1, "mdns.Browser");
  
  
  lua_pushfstring(L, "<mdns.Browser: %p %s>", *userdata, (*userdata)->service_type());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** static LuaStackSize mdns::Browser::MakeInstance(lubyk::Worker *worker, const char *service_type, lua_State *L)
 * include/mdns/browser.h:62
 */
static int Browser_MakeInstance(lua_State *L) {
  try {
    lubyk::Worker *worker = *((lubyk::Worker **)luaL_checkudata(L, 1, "lubyk.Worker"));
    const char *service_type = luaL_checkstring(L, 2);
    
    LuaStackSize  retval__ = Browser::MakeInstance(worker, service_type, L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "mdns.Browser.MakeInstance: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mdns.Browser.MakeInstance: Unknown exception");
  }
}


/** virtual void mdns::Browser::add_device(const Location &location)
 * include/mdns/browser.h:75
 */
static int Browser_add_device(lua_State *L) {
  try {
    Browser *self__ = *((Browser**)luaL_checkudata(L, 1, "mdns.Browser"));
    const Location *location = *((const Location **)luaL_checkudata(L, 2, "mdns.Location"));
    self__->add_device(*location);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mdns.Browser.add_device: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mdns.Browser.add_device: Unknown exception");
  }
}


/** virtual void mdns::Browser::remove_device(const char *name)
 * include/mdns/browser.h:122
 */
static int Browser_remove_device(lua_State *L) {
  try {
    Browser *self__ = *((Browser**)luaL_checkudata(L, 1, "mdns.Browser"));
    const char *name = luaL_checkstring(L, 2);
    self__->remove_device(name);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mdns.Browser.remove_device: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mdns.Browser.remove_device: Unknown exception");
  }
}


/** const char* mdns::Browser::service_type()
 * include/mdns/browser.h:149
 */
static int Browser_service_type(lua_State *L) {
  try {
    Browser *self__ = *((Browser**)luaL_checkudata(L, 1, "mdns.Browser"));
    const char * retval__ = self__->service_type();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mdns.Browser.service_type: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mdns.Browser.service_type: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Browser_member_methods[] = {
  {"add_device"        , Browser_add_device},
  {"remove_device"     , Browser_remove_device},
  {"service_type"      , Browser_service_type},
  {"__tostring"        , Browser__tostring},
  {"__gc"              , Browser_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Browser_namespace_methods[] = {
  {"Browser"           , Browser_MakeInstance},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mdns_Browser_core(lua_State *L) {
#else
extern "C" int luaopen_mdns_Browser_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mdns.Browser");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Browser_member_methods);
  // save meta-table in mdns.Browser_
  register_mt(L, "mdns", "Browser");

  // register class methods in a global namespace table
  luaL_register(L, "mdns", Browser_namespace_methods);


	return 1;
}
