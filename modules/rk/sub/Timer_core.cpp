#include "rk/Timer.h"

#include "lua_cpp_helper.h"


using namespace rk;


/* ============================ Constructors     ====================== */

/** rk::Timer::Timer(rubyk::Worker *worker, float interval, int lua_func_idx)
 * include/rk/Timer.h:11
 */
static int Timer_Timer(lua_State *L) {
  try {
    rubyk::Worker *worker = *((rubyk::Worker **)luaL_checkudata(L, 1, "rubyk.Worker"));
    float interval = luaL_checknumber(L, 2);
    
    luaL_checktype(L, 3, LUA_TFUNCTION);
    // push on top
    lua_pushvalue(L, 3);
    int lua_func_idx = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_pop(L, 1);
    
    Timer * retval__ = new Timer(worker, interval, lua_func_idx);
    lua_pushclass<Timer>(L, retval__, "rk.Timer");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Timer.Timer: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Timer.Timer: Unknown exception");
    lua_error(L);
    return 0;
  }
}

/* ============================ Destructor       ====================== */

static int Timer_destructor(lua_State *L) {
  Timer **userdata = (Timer**)luaL_checkudata(L, 1, "rk.Timer");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Timer__tostring(lua_State *L) {
  Timer **userdata = (Timer**)luaL_checkudata(L, 1, "rk.Timer");
  
  lua_pushfstring(L, "<rk.Timer: %p %li>", *userdata, (*userdata)->interval());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** time_t rk::Timer::interval()
 * include/rk/Timer.h:31
 */
static int Timer_interval(lua_State *L) {
  try {
    Timer *self__ = *((Timer**)luaL_checkudata(L, 1, "rk.Timer"));
    lua_remove(L, 1);
    time_t  retval__ = self__->interval();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Timer.interval: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Timer.interval: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void rk::Timer::join()
 * include/rk/Timer.h:26
 */
static int Timer_join(lua_State *L) {
  try {
    Timer *self__ = *((Timer**)luaL_checkudata(L, 1, "rk.Timer"));
    lua_remove(L, 1);
    self__->join();
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Timer.join: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Timer.join: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void rk::Timer::start()
 * include/rk/Timer.h:22
 */
static int Timer_start(lua_State *L) {
  try {
    Timer *self__ = *((Timer**)luaL_checkudata(L, 1, "rk.Timer"));
    lua_remove(L, 1);
    self__->start();
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Timer.start: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Timer.start: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void rk::Timer::stop()
 * include/rk/Timer.h:18
 */
static int Timer_stop(lua_State *L) {
  try {
    Timer *self__ = *((Timer**)luaL_checkudata(L, 1, "rk.Timer"));
    lua_remove(L, 1);
    self__->stop();
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("rk.Timer.stop: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "rk.Timer.stop: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Timer_member_methods[] = {
  {"interval"          , Timer_interval},
  {"join"              , Timer_join},
  {"start"             , Timer_start},
  {"stop"              , Timer_stop},
  {"__tostring"        , Timer__tostring},
  {"__gc"              , Timer_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Timer_namespace_methods[] = {
  {"Timer"             , Timer_Timer},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_rk_Timer_core(lua_State *L) {
#else
extern "C" int luaopen_rk_Timer_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "rk.Timer");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Timer_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "rk", Timer_namespace_methods);


	return 1;
}
