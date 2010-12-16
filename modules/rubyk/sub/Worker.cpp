#include "rubyk/Worker.h"

#include "lua_cpp_helper.h"


using namespace rubyk;


/* ============================ Constructors     ====================== */

/** rubyk::Worker::Worker(lua_State *L)
 * include/rubyk/worker.h:64
 */
static int Worker_Worker(lua_State *L) {
  try {
    
    Worker * retval__ = new Worker(L);
    lua_pushclass<Worker>(L, retval__, "rubyk.Worker");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("rubyk.Worker.Worker: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rubyk.Worker.Worker: Unknown exception");
    lua_error(L);
    return 0;
  }
}

/* ============================ Destructor       ====================== */

static int Worker_destructor(lua_State *L) {
  Worker **userdata = (Worker**)luaL_checkudata(L, 1, "rubyk.Worker");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Worker__tostring(lua_State *L) {
  Worker **userdata = (Worker**)luaL_checkudata(L, 1, "rubyk.Worker");
  
  lua_pushfstring(L, "<rubyk.Worker: %p %f>", *userdata, (*userdata)->now());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** double rubyk::Worker::now()
 * include/rubyk/worker.h:79
 */
static int Worker_now(lua_State *L) {
  try {
    Worker *self__ = *((Worker**)luaL_checkudata(L, 1, "rubyk.Worker"));
    lua_remove(L, 1);
    double  retval__ = self__->now();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("rubyk.Worker.now: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rubyk.Worker.now: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void rubyk::Worker::sleep(double duration)
 * include/rubyk/worker.h:74
 */
static int Worker_sleep(lua_State *L) {
  try {
    Worker *self__ = *((Worker**)luaL_checkudata(L, 1, "rubyk.Worker"));
    lua_remove(L, 1);
    double duration = luaL_checknumber(L, 1);
    self__->sleep(duration);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("rubyk.Worker.sleep: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rubyk.Worker.sleep: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Worker_member_methods[] = {
  {"now"               , Worker_now},
  {"sleep"             , Worker_sleep},
  {"__tostring"        , Worker__tostring},
  {"__gc"              , Worker_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Worker_namespace_methods[] = {
  {"Worker"            , Worker_Worker},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_rubyk_Worker(lua_State *L) {
#else
extern "C" int luaopen_rubyk_Worker(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "rubyk.Worker");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Worker_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "rubyk", Worker_namespace_methods);


	return 1;
}
