#include "lk/Mutex.h"

#include "lua_cpp_helper.h"





using namespace lk;


/* ============================ Constructors     ====================== */

/** lk::Mutex::Mutex()
 * include/lk/Mutex.h:40
 */
static int Mutex_Mutex(lua_State *L) {
  try {
    Mutex * retval__ = new Mutex();
    lua_pushclass<Mutex>(L, retval__, "lk.Mutex");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "Mutex: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Mutex: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Mutex_destructor(lua_State *L) {
  Mutex **userdata = (Mutex**)dubL_checksdata_n(L, 1, "lk.Mutex");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Mutex__tostring(lua_State *L) {
  Mutex **userdata = (Mutex**)dubL_checksdata_n(L, 1, "lk.Mutex");
  
  
  lua_pushfstring(L, "<lk.Mutex: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void lk::Mutex::lock()
 * include/lk/Mutex.h:44
 */
static int Mutex_lock(lua_State *L) {
  try {
    Mutex *self = *((Mutex**)dubL_checksdata(L, 1, "lk.Mutex"));
    self->lock();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "lock: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "lock: Unknown exception");
  }
  return lua_error(L);
}



/** int lk::Mutex::lockCount()
 * include/lk/Mutex.h:56
 */
static int Mutex_lockCount(lua_State *L) {
  try {
    Mutex *self = *((Mutex**)dubL_checksdata(L, 1, "lk.Mutex"));
    int  retval__ = self->lockCount();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "lockCount: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "lockCount: Unknown exception");
  }
  return lua_error(L);
}



/** void lk::Mutex::unlock()
 * include/lk/Mutex.h:50
 */
static int Mutex_unlock(lua_State *L) {
  try {
    Mutex *self = *((Mutex**)dubL_checksdata(L, 1, "lk.Mutex"));
    self->unlock();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "unlock: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "unlock: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Mutex_member_methods[] = {
  {"lock"              , Mutex_lock},
  {"lockCount"         , Mutex_lockCount},
  {"unlock"            , Mutex_unlock},
  {"__tostring"        , Mutex__tostring},
  {"__gc"              , Mutex_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Mutex_namespace_methods[] = {
  {"Mutex"             , Mutex_Mutex},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_lk_Mutex(lua_State *L) {
#else
extern "C" int luaopen_lk_Mutex(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "lk.Mutex");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Mutex_member_methods);
  // save meta-table in lk.Mutex_
  register_mt(L, "lk", "Mutex");

  // register class methods in a global namespace table
  luaL_register(L, "lk", Mutex_namespace_methods);


	return 1;
}
