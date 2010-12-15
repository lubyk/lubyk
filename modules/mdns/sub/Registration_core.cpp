#include "mdns/Registration.h"

#include "lua_cpp_helper.h"


using namespace mdns;


/* ============================ Constructors     ====================== */

/** mdns::Registration::Registration(rubyk::Worker *worker, const char *service_type, const char *name, uint port, int lua_func_idx)
 * include/mdns/Registration.h:47
 */
static int Registration_Registration(lua_State *L) {
  try {
    rubyk::Worker *worker = *((rubyk::Worker **)luaL_checkudata(L, 1, "rubyk.Worker"));
    const char *service_type = luaL_checkstring(L, 2);
    const char *name = luaL_checkstring(L, 3);
    uint port = luaL_checkint(L, 4);
    
    luaL_checktype(L, 5, LUA_TFUNCTION);
    // push on top
    lua_pushvalue(L, 5);
    int lua_func_idx = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_pop(L, 1);
    
    Registration * retval__ = new Registration(worker, service_type, name, port, lua_func_idx);
    lua_pushclass<Registration>(L, retval__, "mdns.Registration");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mdns.Registration.Registration: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mdns.Registration.Registration: Unknown exception");
    lua_error(L);
    return 0;
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


/** virtual void mdns::Registration::registration_done()
 * include/mdns/Registration.h:55
 */
static int Registration_registration_done(lua_State *L) {
  try {
    Registration *self__ = *((Registration**)luaL_checkudata(L, 1, "mdns.Registration"));
    lua_remove(L, 1);
    self__->registration_done();
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mdns.Registration.registration_done: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mdns.Registration.registration_done: Unknown exception");
    lua_error(L);
    return 0;
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
