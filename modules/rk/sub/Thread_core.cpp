#include "rk/Thread.h"

#include "lua_cpp_helper.h"


using namespace rk;


/* ============================ Constructors     ====================== */

/** rk::Thread::Thread(rubyk::Worker *worker)
 * include/rk/Thread.h:44
 */
static int Thread_Thread(lua_State *L) {
  try {
    rubyk::Worker *worker = *((rubyk::Worker **)luaL_checkudata(L, 1, "rubyk.Worker"));
    Thread * retval__ = new Thread(worker);
    lua_pushclass<Thread>(L, retval__, "rk.Thread");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Thread.Thread: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Thread.Thread: Unknown exception");
  }
}


/* ============================ Destructor       ====================== */

static int Thread_destructor(lua_State *L) {
  Thread **userdata = (Thread**)luaL_checkudata(L, 1, "rk.Thread");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Thread__tostring(lua_State *L) {
  Thread **userdata = (Thread**)luaL_checkudata(L, 1, "rk.Thread");
  
  
  lua_pushfstring(L, "<rk.Thread: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void rk::Thread::join()
 * include/rk/Thread.h:59
 */
static int Thread_join(lua_State *L) {
  try {
    Thread *self__ = *((Thread**)luaL_checkudata(L, 1, "rk.Thread"));
    self__->join();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Thread.join: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Thread.join: Unknown exception");
  }
}


/** void rk::Thread::kill()
 * include/rk/Thread.h:54
 */
static int Thread_kill(lua_State *L) {
  try {
    Thread *self__ = *((Thread**)luaL_checkudata(L, 1, "rk.Thread"));
    self__->kill();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Thread.kill: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Thread.kill: Unknown exception");
  }
}


/** void rk::Thread::quit()
 * include/rk/Thread.h:50
 */
static int Thread_quit(lua_State *L) {
  try {
    Thread *self__ = *((Thread**)luaL_checkudata(L, 1, "rk.Thread"));
    self__->quit();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Thread.quit: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Thread.quit: Unknown exception");
  }
}


/** bool rk::Thread::should_run()
 * include/rk/Thread.h:64
 */
static int Thread_should_run(lua_State *L) {
  try {
    Thread *self__ = *((Thread**)luaL_checkudata(L, 1, "rk.Thread"));
    bool  retval__ = self__->should_run();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Thread.should_run: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Thread.should_run: Unknown exception");
  }
}


/** void rk::Thread::start(lua_State *L)
 * include/rk/Thread.h:68
 */
static int Thread_start(lua_State *L) {
  try {
    Thread *self__ = *((Thread**)luaL_checkudata(L, 1, "rk.Thread"));
    
    self__->start(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "rk.Thread.start: %s", e.what());
  } catch (...) {
    return luaL_error(L, "rk.Thread.start: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Thread_member_methods[] = {
  {"join"              , Thread_join},
  {"kill"              , Thread_kill},
  {"quit"              , Thread_quit},
  {"should_run"        , Thread_should_run},
  {"start"             , Thread_start},
  {"__tostring"        , Thread__tostring},
  {"__gc"              , Thread_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Thread_namespace_methods[] = {
  {"Thread"            , Thread_Thread},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_rk_Thread_core(lua_State *L) {
#else
extern "C" int luaopen_rk_Thread_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "rk.Thread");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Thread_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "rk", Thread_namespace_methods);


	return 1;
}
