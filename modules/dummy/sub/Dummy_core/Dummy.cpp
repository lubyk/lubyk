#include "dummy/Dummy.h"

#include "lua_cpp_helper.h"


using namespace dummy;


/* ============================ Constructors     ====================== */


/** dummy::Dummy::Dummy(lubyk::Worker *worker)
 * include/db/dummy.h:48
 */
static int Dummy_Dummy1(lua_State *L) {
  try {
    lubyk::Worker *worker = *((lubyk::Worker **)luaL_checkudata(L, 1, "lubyk.Worker"));
    Dummy * retval__ = new Dummy(worker);
    lua_pushclass<Dummy>(L, retval__, "dummy.Dummy");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "dummy.Dummy.Dummy: %s", e.what());
  } catch (...) {
    return luaL_error(L, "dummy.Dummy.Dummy: Unknown exception");
  }
}


/** dummy::Dummy::Dummy(lubyk::Worker *worker)
 * include/dummy/dummy.h:48
 */
static int Dummy_Dummy2(lua_State *L) {
  try {
    lubyk::Worker *worker = *((lubyk::Worker **)luaL_checkudata(L, 1, "lubyk.Worker"));
    Dummy * retval__ = new Dummy(worker);
    lua_pushclass<Dummy>(L, retval__, "dummy.Dummy");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "dummy.Dummy.Dummy: %s", e.what());
  } catch (...) {
    return luaL_error(L, "dummy.Dummy.Dummy: Unknown exception");
  }
}



/** Overloaded function chooser for Dummy(...) */
static int Dummy_Dummy(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "lubyk.Worker")) {
    type__ = lua_type(L, 2);
    if (top__ < 2) {
      return Dummy_Dummy1(L);
    } else {
      // use any to raise errors
      return Dummy_Dummy1(L);
    }
  } else {
    // use any to raise errors
    return Dummy_Dummy1(L);
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
 * include/db/dummy.h:53
 */
static int Dummy___newindex1(lua_State *L) {
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


/** void dummy::Dummy::__newindex(lua_State *L)
 * include/dummy/dummy.h:53
 */
static int Dummy___newindex2(lua_State *L) {
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



/** Overloaded function chooser for __newindex(...) */
static int Dummy___newindex(lua_State *L) {
  int type__ = lua_type(L, 2);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "dummy.lua_State")) {
    type__ = lua_type(L, 3);
    if (top__ < 3) {
      return Dummy___newindex1(L);
    } else {
      // use any to raise errors
      return Dummy___newindex1(L);
    }
  } else {
    // use any to raise errors
    return Dummy___newindex1(L);
  }
}



/** void dummy::Dummy::callback()
 * include/db/dummy.h:68
 */
static int Dummy_callback1(lua_State *L) {
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


/** void dummy::Dummy::callback()
 * include/dummy/dummy.h:68
 */
static int Dummy_callback2(lua_State *L) {
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



/** Overloaded function chooser for callback(...) */
static int Dummy_callback(lua_State *L) {
  int type__ = lua_type(L, 2);
  int top__  = lua_gettop(L);
  if (top__ < 2) {
    return Dummy_callback1(L);
  } else {
    // use any to raise errors
    return Dummy_callback1(L);
  }
}



/** static const char* dummy::Dummy::plat()
 * include/db/dummy.h:83
 */
static int Dummy_plat1(lua_State *L) {
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


/** static const char* dummy::Dummy::plat()
 * include/dummy/dummy.h:83
 */
static int Dummy_plat2(lua_State *L) {
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



/** Overloaded function chooser for plat(...) */
static int Dummy_plat(lua_State *L) {
  int type__ = lua_type(L, 2);
  int top__  = lua_gettop(L);
  if (top__ < 2) {
    return Dummy_plat1(L);
  } else {
    // use any to raise errors
    return Dummy_plat1(L);
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
