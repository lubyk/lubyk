#include "mimas/Callback.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Callback::Callback()
 * include/mimas/Callback.h:58
 */
static int Callback_Callback(lua_State *L) {
  try {
    Callback * retval__ = new Callback();
    // The class inherits from 'LuaCallback', use lua_init instead of pushclass.
    return retval__->luaInit(L, retval__, "mimas.Callback");
  } catch (std::exception &e) {
    lua_pushfstring(L, "Callback: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Callback: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Callback_destructor(lua_State *L) {
  Callback **userdata = (Callback**)dubL_checksdata_n(L, 1, "mimas.Callback");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Callback__tostring(lua_State *L) {
  Callback **userdata = (Callback**)dubL_checksdata_n(L, 1, "mimas.Callback");
  
  
  lua_pushfstring(L, "<mimas.Callback: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::Callback::connect(QObject *obj, const char *method, const char *callback)
 * include/mimas/Callback.h:64
 */
static int Callback_connect(lua_State *L) {
  try {
    Callback *self = *((Callback**)dubL_checksdata(L, 1, "mimas.Callback"));
    QObject *obj = *((QObject **)dubL_checksdata(L, 2, "mimas.QObject"));
    const char *method = dubL_checkstring(L, 3);
    const char *callback = dubL_checkstring(L, 4);
    self->connect(obj, method, callback);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "connect: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "connect: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Callback_member_methods[] = {
  {"connect"           , Callback_connect},
  {"__tostring"        , Callback__tostring},
  {"__gc"              , Callback_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Callback_namespace_methods[] = {
  {"Callback"          , Callback_Callback},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Callback_core(lua_State *L) {
#else
extern "C" int luaopen_mimas_Callback_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Callback");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Callback_member_methods);
  // save meta-table in mimas.Callback_
  register_mt(L, "mimas", "Callback");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Callback_namespace_methods);


	return 1;
}
