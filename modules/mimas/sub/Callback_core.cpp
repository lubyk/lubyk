#include "mimas/Callback.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Callback::Callback(rubyk::Worker *worker, int lua_func_idx)
 * include/mimas/Callback.h:57
 */
static int Callback_Callback(lua_State *L) {
  try {
    rubyk::Worker *worker = *((rubyk::Worker **)luaL_checkudata(L, 1, "rubyk.Worker"));
    
    luaL_checktype(L, 2, LUA_TFUNCTION);
    // push on top
    lua_pushvalue(L, 2);
    int lua_func_idx = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_pop(L, 1);
    
    Callback * retval__ = new Callback(worker, lua_func_idx);
    lua_pushclass<Callback>(L, retval__, "mimas.Callback");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Callback.Callback: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Callback.Callback: Unknown exception");
    lua_error(L);
    return 0;
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
 * include/mimas/Callback.h:64
 */
static int Callback_connect(lua_State *L) {
  try {
    Callback *self__ = *((Callback**)luaL_checkudata(L, 1, "mimas.Callback"));
    lua_remove(L, 1);
    QObject *obj = *((QObject **)luaL_checkudata(L, 1, "mimas.QObject"));
    const char *method = luaL_checkstring(L, 2);
    const char *callback = luaL_checkstring(L, 3);
    self__->connect(obj, method, callback);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Callback.connect: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Callback.connect: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Callback::delete_on_call(bool should_delete)
 * include/mimas/Callback.h:70
 */
static int Callback_delete_on_call(lua_State *L) {
  try {
    Callback *self__ = *((Callback**)luaL_checkudata(L, 1, "mimas.Callback"));
    lua_remove(L, 1);
    bool should_delete = lua_toboolean(L, 1);
    self__->delete_on_call(should_delete);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Callback.delete_on_call: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Callback.delete_on_call: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Callback_member_methods[] = {
  {"connect"           , Callback_connect},
  {"delete_on_call"    , Callback_delete_on_call},
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
