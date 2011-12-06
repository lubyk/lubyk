#include "wii/Browser.h"

#include "lua_cpp_helper.h"





using namespace wii;


/* ============================ Constructors     ====================== */

/** wii::Browser::Browser()
 * include/wii/Browser.h:51
 */
static int Browser_Browser(lua_State *L) {
  try {
    Browser * retval__ = new Browser();
    return retval__->luaInit(L, retval__, "wii.Browser");
  } catch (std::exception &e) {
    lua_pushfstring(L, "Browser: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Browser: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Browser_destructor(lua_State *L) {
  Browser **userdata = (Browser**)dubL_checksdata_n(L, 1, "wii.Browser");

  
  // custom destructor
  Browser *self = *userdata;
  if (self) self->luaDestroy();
  
  *userdata = NULL;
  return 0;
}



// test if class is deleted
static int Browser_deleted(lua_State *L) {
  Browser **userdata = (Browser**)dubL_checksdata_n(L, 1, "wii.Browser");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Browser__tostring(lua_State *L) {
  Browser **userdata = (Browser**)dubL_checksdata_n(L, 1, "wii.Browser");
  
  if (!*userdata) {
    lua_pushstring(L, "<wii.Browser: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<wii.Browser: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void wii::Browser::find()
 * include/wii/Browser.h:57
 */
static int Browser_find(lua_State *L) {
  try {
    Browser *self = *((Browser**)dubL_checksdata(L, 1, "wii.Browser"));
    if (!self) throw dub::Exception("Using deleted wii.Browser in find");
    self->find();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "find: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "find: Unknown exception");
  }
  return lua_error(L);
}



/** void wii::Browser::findMore()
 * include/wii/Browser.h:105
 */
static int Browser_findMore(lua_State *L) {
  try {
    Browser *self = *((Browser**)dubL_checksdata(L, 1, "wii.Browser"));
    if (!self) throw dub::Exception("Using deleted wii.Browser in findMore");
    self->findMore();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "findMore: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "findMore: Unknown exception");
  }
  return lua_error(L);
}



/** bool wii::Browser::needMore()
 * include/wii/Browser.h:99
 */
static int Browser_needMore(lua_State *L) {
  try {
    Browser *self = *((Browser**)dubL_checksdata(L, 1, "wii.Browser"));
    if (!self) throw dub::Exception("Using deleted wii.Browser in needMore");
    bool  retval__ = self->needMore();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "needMore: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "needMore: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Browser_member_methods[] = {
  {"find"              , Browser_find},
  {"findMore"          , Browser_findMore},
  {"needMore"          , Browser_needMore},
  {"__tostring"        , Browser__tostring},
  {"__gc"              , Browser_destructor},
  {"deleted"           , Browser_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg Browser_namespace_methods[] = {
  {"Browser"           , Browser_Browser},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_wii_Browser_core(lua_State *L) {
#else
extern "C" int luaopen_wii_Browser_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "wii.Browser");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Browser_member_methods);
  // save meta-table in wii.Browser_
  register_mt(L, "wii", "Browser");

  // register class methods in a global namespace table
  luaL_register(L, "wii", Browser_namespace_methods);


	return 1;
}
