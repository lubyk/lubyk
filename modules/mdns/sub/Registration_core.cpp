#include "mdns/Registration.h"

#include "lua_cpp_helper.h"


using namespace mdns;


/* ============================ Constructors     ====================== */

/** mdns::Registration::Registration(lubyk::Worker *worker, const char *service_type, const char *name, uint port)
 * include/mdns/registration.h:52
 */
static int Registration_Registration(lua_State *L) {
  try {
    lubyk::Worker *worker = *((lubyk::Worker **)luaL_checkudata(L, 1, "lubyk.Worker"));
    const char *service_type = luaL_checkstring(L, 2);
    const char *name = luaL_checkstring(L, 3);
    uint port = luaL_checkint(L, 4);
    Registration * retval__ = new Registration(worker, service_type, name, port);
    lua_pushclass<Registration>(L, retval__, "mdns.Registration");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mdns.Registration.Registration: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mdns.Registration.Registration: Unknown exception");
  }
}


/* ============================ Destructor       ====================== */

static int Registration_destructor(lua_State *L) {
  Registration **userdata = (Registration**)luaL_checkudata(L, 1, "mdns.Registration");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Registration__tostring(lua_State *L) {
  Registration **userdata = (Registration**)luaL_checkudata(L, 1, "mdns.Registration");
  
  
  lua_pushfstring(L, "<mdns.Registration: %p %s>", *userdata, (*userdata)->name());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** static LuaStackSize mdns::Registration::MakeInstance(lubyk::Worker *worker, const char *service_type, const char *name, uint port, lua_State *L)
 * include/mdns/registration.h:58
 */
static int Registration_MakeInstance(lua_State *L) {
  try {
    lubyk::Worker *worker = *((lubyk::Worker **)luaL_checkudata(L, 1, "lubyk.Worker"));
    const char *service_type = luaL_checkstring(L, 2);
    const char *name = luaL_checkstring(L, 3);
    uint port = luaL_checkint(L, 4);
    lua_State *L = *((lua_State **)luaL_checkudata(L, 5, "mdns.lua_State"));
    LuaStackSize  retval__ = Registration::MakeInstance(worker, service_type, name, port, L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "mdns.Registration.MakeInstance: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mdns.Registration.MakeInstance: Unknown exception");
  }
}


/** virtual void mdns::Registration::registration_done()
 * include/mdns/registration.h:70
 */
static int Registration_registration_done(lua_State *L) {
  try {
    Registration *self__ = *((Registration**)luaL_checkudata(L, 1, "mdns.Registration"));
    self__->registration_done();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mdns.Registration.registration_done: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mdns.Registration.registration_done: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Registration_member_methods[] = {
  {"registration_done" , Registration_registration_done},
  {"__tostring"        , Registration__tostring},
  {"__gc"              , Registration_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Registration_namespace_methods[] = {
  {"Registration"      , Registration_Registration},
  {"Registration_MakeInstance", Registration_MakeInstance},
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

  // register class methods in a global namespace table
  luaL_register(L, "mdns", Registration_namespace_methods);


	return 1;
}
