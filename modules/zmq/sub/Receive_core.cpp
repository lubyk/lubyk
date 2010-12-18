#include "zmq/Receive.h"

#include "lua_cpp_helper.h"


using namespace zmq;


/* ============================ Constructors     ====================== */

/** zmq::Receive::Receive(rubyk::Worker *worker, const char *location, int lua_func_idx)
 * include/zmq/Receive.h:57
 */
static int Receive_Receive(lua_State *L) {
  try {
    rubyk::Worker *worker = *((rubyk::Worker **)luaL_checkudata(L, 1, "rubyk.Worker"));
    const char *location = luaL_checkstring(L, 2);
    
    luaL_checktype(L, 3, LUA_TFUNCTION);
    // push on top
    lua_pushvalue(L, 3);
    int lua_func_idx = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_pop(L, 1);
    
    Receive * retval__ = new Receive(worker, location, lua_func_idx);
    lua_pushclass<Receive>(L, retval__, "zmq.Receive");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("zmq.Receive.Receive: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "zmq.Receive.Receive: Unknown exception");
    lua_error(L);
    return 0;
  }
}

/* ============================ Destructor       ====================== */

static int Receive_destructor(lua_State *L) {
  Receive **userdata = (Receive**)luaL_checkudata(L, 1, "zmq.Receive");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Receive__tostring(lua_State *L) {
  Receive **userdata = (Receive**)luaL_checkudata(L, 1, "zmq.Receive");
  
  lua_pushfstring(L, "<zmq.Receive: %p %s>", *userdata, (*userdata)->location());
  
  return 1;
}

/* ============================ Member Methods   ====================== */




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Receive_member_methods[] = {
  {"__tostring"        , Receive__tostring},
  {"__gc"              , Receive_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Receive_namespace_methods[] = {
  {"Receive"           , Receive_Receive},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_zmq_Receive_core(lua_State *L) {
#else
extern "C" int luaopen_zmq_Receive_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "zmq.Receive");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Receive_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "zmq", Receive_namespace_methods);


	return 1;
}
