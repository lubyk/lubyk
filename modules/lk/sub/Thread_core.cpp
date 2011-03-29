#include "lk/Thread.h"

#include "lua_cpp_helper.h"


using namespace lk;


/* ============================ Constructors     ====================== */

/** lk::Thread::Thread(lubyk::Worker *worker)
 * include/lk/Thread.h:44
 */
static int Thread_Thread(lua_State *L) {
  try {
    lubyk::Worker *worker = *((lubyk::Worker **)luaL_checkudata(L, 1, "lubyk.Worker"));
    Thread * retval__ = new Thread(worker);
    lua_pushclass<Thread>(L, retval__, "lk.Thread");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Thread.Thread: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Thread.Thread: Unknown exception");
  }
}


/* ============================ Destructor       ====================== */

static int Thread_destructor(lua_State *L) {
  Thread **userdata = (Thread**)luaL_checkudata(L, 1, "lk.Thread");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Thread__tostring(lua_State *L) {
  Thread **userdata = (Thread**)luaL_checkudata(L, 1, "lk.Thread");
  
  
  lua_pushfstring(L, "<lk.Thread: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void lk::Thread::join()
 * include/lk/Thread.h:59
 */
static int Thread_join(lua_State *L) {
  try {
    Thread *self__ = *((Thread**)luaL_checkudata(L, 1, "lk.Thread"));
    self__->join();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Thread.join: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Thread.join: Unknown exception");
  }
}


/** void lk::Thread::kill()
 * include/lk/Thread.h:54
 */
static int Thread_kill(lua_State *L) {
  try {
    Thread *self__ = *((Thread**)luaL_checkudata(L, 1, "lk.Thread"));
    self__->kill();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Thread.kill: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Thread.kill: Unknown exception");
  }
}


/** void lk::Thread::quit()
 * include/lk/Thread.h:50
 */
static int Thread_quit(lua_State *L) {
  try {
    Thread *self__ = *((Thread**)luaL_checkudata(L, 1, "lk.Thread"));
    self__->quit();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Thread.quit: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Thread.quit: Unknown exception");
  }
}


/** bool lk::Thread::shouldRun()
 * include/lk/Thread.h:64
 */
static int Thread_shouldRun(lua_State *L) {
  try {
    Thread *self__ = *((Thread**)luaL_checkudata(L, 1, "lk.Thread"));
    bool  retval__ = self__->shouldRun();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Thread.shouldRun: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Thread.shouldRun: Unknown exception");
  }
}


/** void lk::Thread::start(lua_State *L)
 * include/lk/Thread.h:68
 */
static int Thread_start(lua_State *L) {
  try {
    Thread *self__ = *((Thread**)luaL_checkudata(L, 1, "lk.Thread"));
    
    self__->start(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "lk.Thread.start: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lk.Thread.start: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Thread_member_methods[] = {
  {"join"              , Thread_join},
  {"kill"              , Thread_kill},
  {"quit"              , Thread_quit},
  {"shouldRun"         , Thread_shouldRun},
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
int luaload_lk_Thread_core(lua_State *L) {
#else
extern "C" int luaopen_lk_Thread_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "lk.Thread");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Thread_member_methods);
  // save meta-table in lk.Thread_
  register_mt(L, "lk", "Thread");

  // register class methods in a global namespace table
  luaL_register(L, "lk", Thread_namespace_methods);


	return 1;
}
