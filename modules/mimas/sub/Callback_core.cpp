#include "mimas/Callback.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Callback::Callback(rubyk::Worker *worker)
 * include/mimas/Callback.h:60
 */
static int Callback_Callback(lua_State *L) {
  try {
    rubyk::Worker *worker = *((rubyk::Worker **)luaL_checkudata(L, 1, "rubyk.Worker"));
    Callback * retval__ = new Callback(worker);
    lua_pushclass<Callback>(L, retval__, "mimas.Callback");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Callback.Callback: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Callback.Callback: Unknown exception");
  }
}


/* ============================ Destructor       ====================== */

static int Callback_destructor(lua_State *L) {
  Callback **userdata = (Callback**)luaL_checkudata(L, 1, "mimas.Callback");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Callback__tostring(lua_State *L) {
  Callback **userdata = (Callback**)luaL_checkudata(L, 1, "mimas.Callback");
  
  
  lua_pushfstring(L, "<mimas.Callback: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::Callback::connect(QObject *obj, const char *method, const char *callback)
 * include/mimas/Callback.h:66
 */
static int Callback_connect(lua_State *L) {
  try {
    Callback *self__ = *((Callback**)luaL_checkudata(L, 1, "mimas.Callback"));
    QObject *obj = *((QObject **)luaL_checkudata(L, 2, "mimas.QObject"));
    const char *method = luaL_checkstring(L, 3);
    const char *callback = luaL_checkstring(L, 4);
    self__->connect(obj, method, callback);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Callback.connect: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Callback.connect: Unknown exception");
  }
}


/** void mimas::Callback::set_callback(lua_State *L)
 * include/mimas/Callback.h:86
 */
static int Callback_set_callback(lua_State *L) {
  try {
    Callback *self__ = *((Callback**)luaL_checkudata(L, 1, "mimas.Callback"));
    
    self__->set_callback(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Callback.set_callback: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Callback.set_callback: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Callback_member_methods[] = {
  {"connect"           , Callback_connect},
  {"set_callback"      , Callback_set_callback},
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

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Callback_namespace_methods);


	return 1;
}
