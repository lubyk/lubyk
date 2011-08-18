#include "lubyk/Worker.h"

#include "lua_cpp_helper.h"


using namespace lubyk;


/* ============================ Constructors     ====================== */

/** lubyk::Worker::Worker(lua_State *L)
 * include/lubyk/worker.h:79
 */
static int Worker_Worker(lua_State *L) {
  try {
    
    Worker * retval__ = new Worker(L);
    lua_pushclass<Worker>(L, retval__, "lubyk.Worker");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "lubyk.Worker.Worker: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lubyk.Worker.Worker: Unknown exception");
  }
}



/* ============================ Destructor       ====================== */

static int Worker_destructor(lua_State *L) {
  Worker **userdata = (Worker**)luaL_checkudata(L, 1, "lubyk.Worker");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Worker__tostring(lua_State *L) {
  Worker **userdata = (Worker**)luaL_checkudata(L, 1, "lubyk.Worker");
  
  
  lua_pushfstring(L, "<lubyk.Worker: %p %f>", *userdata, (*userdata)->now());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** LuaStackSize lubyk::Worker::execPath(lua_State *L)
 * include/lubyk/worker.h:111
 */
static int Worker_execPath(lua_State *L) {
  try {
    Worker *self__ = *((Worker**)dubL_checkudata(L, 1, "lubyk.Worker"));
    
    LuaStackSize  retval__ = self__->execPath(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "lubyk.Worker.execPath: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lubyk.Worker.execPath: Unknown exception");
  }
}



/** void lubyk::Worker::exit(int status)
 * include/lubyk/worker.h:107
 */
static int Worker_exit(lua_State *L) {
  try {
    Worker *self__ = *((Worker**)dubL_checkudata(L, 1, "lubyk.Worker"));
    int status = dubL_checkint(L, 2);
    self__->exit(status);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "lubyk.Worker.exit: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lubyk.Worker.exit: Unknown exception");
  }
}



/** double lubyk::Worker::now()
 * include/lubyk/worker.h:125
 */
static int Worker_now(lua_State *L) {
  try {
    Worker *self__ = *((Worker**)dubL_checkudata(L, 1, "lubyk.Worker"));
    double  retval__ = self__->now();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "lubyk.Worker.now: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lubyk.Worker.now: Unknown exception");
  }
}



/** void lubyk::Worker::run()
 * include/lubyk/worker.h:123
 */
static int Worker_run(lua_State *L) {
  try {
    Worker *self__ = *((Worker**)dubL_checkudata(L, 1, "lubyk.Worker"));
    self__->run();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "lubyk.Worker.run: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lubyk.Worker.run: Unknown exception");
  }
}



/** void lubyk::Worker::sleep(double duration)
 * include/lubyk/worker.h:85
 */
static int Worker_sleep(lua_State *L) {
  try {
    Worker *self__ = *((Worker**)dubL_checkudata(L, 1, "lubyk.Worker"));
    double duration = dubL_checknumber(L, 2);
    self__->sleep(duration);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "lubyk.Worker.sleep: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lubyk.Worker.sleep: Unknown exception");
  }
}



/** LuaStackSize lubyk::Worker::spawn(const char *script, lua_State *L)
 * include/lubyk/worker.h:99
 */
static int Worker_spawn(lua_State *L) {
  try {
    Worker *self__ = *((Worker**)dubL_checkudata(L, 1, "lubyk.Worker"));
    const char *script = dubL_checkstring(L, 2);
    
    LuaStackSize  retval__ = self__->spawn(script, L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "lubyk.Worker.spawn: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lubyk.Worker.spawn: Unknown exception");
  }
}



/** void lubyk::Worker::test_lock()
 * include/lubyk/worker.h:114
 */
static int Worker_test_lock(lua_State *L) {
  try {
    Worker *self__ = *((Worker**)dubL_checkudata(L, 1, "lubyk.Worker"));
    self__->test_lock();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "lubyk.Worker.test_lock: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lubyk.Worker.test_lock: Unknown exception");
  }
}



/** void lubyk::Worker::test_unlock()
 * include/lubyk/worker.h:119
 */
static int Worker_test_unlock(lua_State *L) {
  try {
    Worker *self__ = *((Worker**)dubL_checkudata(L, 1, "lubyk.Worker"));
    self__->test_unlock();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "lubyk.Worker.test_unlock: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lubyk.Worker.test_unlock: Unknown exception");
  }
}



/** void lubyk::Worker::wait(double duration)
 * include/lubyk/worker.h:92
 */
static int Worker_wait(lua_State *L) {
  try {
    Worker *self__ = *((Worker**)dubL_checkudata(L, 1, "lubyk.Worker"));
    double duration = dubL_checknumber(L, 2);
    self__->wait(duration);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "lubyk.Worker.wait: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lubyk.Worker.wait: Unknown exception");
  }
}



/** int lubyk::Worker::waitpid(int pid)
 * include/lubyk/worker.h:103
 */
static int Worker_waitpid(lua_State *L) {
  try {
    Worker *self__ = *((Worker**)dubL_checkudata(L, 1, "lubyk.Worker"));
    int pid = dubL_checkint(L, 2);
    int  retval__ = self__->waitpid(pid);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "lubyk.Worker.waitpid: %s", e.what());
  } catch (...) {
    return luaL_error(L, "lubyk.Worker.waitpid: Unknown exception");
  }
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Worker_member_methods[] = {
  {"execPath"          , Worker_execPath},
  {"exit"              , Worker_exit},
  {"now"               , Worker_now},
  {"run"               , Worker_run},
  {"sleep"             , Worker_sleep},
  {"spawn"             , Worker_spawn},
  {"test_lock"         , Worker_test_lock},
  {"test_unlock"       , Worker_test_unlock},
  {"wait"              , Worker_wait},
  {"waitpid"           , Worker_waitpid},
  {"__tostring"        , Worker__tostring},
  {"__gc"              , Worker_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Worker_namespace_methods[] = {
  {"Worker"            , Worker_Worker},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_lubyk_Worker(lua_State *L) {
#else
extern "C" int luaopen_lubyk_Worker(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "lubyk.Worker");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Worker_member_methods);
  // save meta-table in lubyk.Worker_
  register_mt(L, "lubyk", "Worker");

  // register class methods in a global namespace table
  luaL_register(L, "lubyk", Worker_namespace_methods);


	return 1;
}
