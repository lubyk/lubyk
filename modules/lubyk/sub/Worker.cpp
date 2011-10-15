#include "lubyk/Worker.h"

#include "lua_cpp_helper.h"





using namespace lubyk;


/* ============================ Constructors     ====================== */

/** lubyk::Worker::Worker()
 * include/lubyk/worker.h:88
 */
static int Worker_Worker(lua_State *L) {
  try {
    Worker * retval__ = new Worker();
    lua_pushclass<Worker>(L, retval__, "lubyk.Worker");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "Worker: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Worker: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Worker_destructor(lua_State *L) {
  Worker **userdata = (Worker**)dubL_checksdata_n(L, 1, "lubyk.Worker");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Worker__tostring(lua_State *L) {
  Worker **userdata = (Worker**)dubL_checksdata_n(L, 1, "lubyk.Worker");
  
  
  lua_pushfstring(L, "<lubyk.Worker: %p %f>", *userdata, (*userdata)->now());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** LuaStackSize lubyk::Worker::execPath(lua_State *L)
 * include/lubyk/worker.h:114
 */
static int Worker_execPath(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    
    LuaStackSize  retval__ = self->execPath(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "execPath: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "execPath: Unknown exception");
  }
  return lua_error(L);
}



/** void lubyk::Worker::execute(const char *cmd)
 * include/lubyk/worker.h:99
 */
static int Worker_execute(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    const char *cmd = dubL_checkstring(L, 2);
    self->execute(cmd);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "execute: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "execute: Unknown exception");
  }
  return lua_error(L);
}



/** void lubyk::Worker::exit(int status)
 * include/lubyk/worker.h:110
 */
static int Worker_exit(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    int status = dubL_checkint(L, 2);
    self->exit(status);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "exit: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "exit: Unknown exception");
  }
  return lua_error(L);
}



/** static Worker* lubyk::Worker::getWorker(lua_State *L)
 * include/lubyk/worker.h:194
 */
static int Worker_getWorker(lua_State *L) {
  try {
    
    Worker * retval__ = Worker::getWorker(L);
    lua_pushclass<Worker>(L, retval__, "lubyk.Worker");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "getWorker: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "getWorker: Unknown exception");
  }
  return lua_error(L);
}



/** double lubyk::Worker::now()
 * include/lubyk/worker.h:118
 */
static int Worker_now(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    double  retval__ = self->now();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "now: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "now: Unknown exception");
  }
  return lua_error(L);
}



/** void lubyk::Worker::run()
 * include/lubyk/worker.h:116
 */
static int Worker_run(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    self->run();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "run: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "run: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize lubyk::Worker::spawn(const char *script, lua_State *L)
 * include/lubyk/worker.h:95
 */
static int Worker_spawn(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    const char *script = dubL_checkstring(L, 2);
    
    LuaStackSize  retval__ = self->spawn(script, L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "spawn: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "spawn: Unknown exception");
  }
  return lua_error(L);
}



/** int lubyk::Worker::waitpid(int pid)
 * include/lubyk/worker.h:106
 */
static int Worker_waitpid(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    int pid = dubL_checkint(L, 2);
    int  retval__ = self->waitpid(pid);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "waitpid: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "waitpid: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Worker_member_methods[] = {
  {"execPath"          , Worker_execPath},
  {"execute"           , Worker_execute},
  {"exit"              , Worker_exit},
  {"now"               , Worker_now},
  {"run"               , Worker_run},
  {"spawn"             , Worker_spawn},
  {"waitpid"           , Worker_waitpid},
  {"__tostring"        , Worker__tostring},
  {"__gc"              , Worker_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Worker_namespace_methods[] = {
  {"Worker"            , Worker_Worker},
  {"Worker_getWorker"  , Worker_getWorker},
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
