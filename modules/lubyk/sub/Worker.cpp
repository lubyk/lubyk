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
 * include/lubyk/worker.h:120
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



/** void lubyk::Worker::exit(int status)
 * include/lubyk/worker.h:116
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



/** void lubyk::Worker::fdErrorClear(int fd)
 * include/lubyk/worker.h:181
 */
static int Worker_fdErrorClear(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    int fd = dubL_checkint(L, 2);
    self->fdErrorClear(fd);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "fdErrorClear: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "fdErrorClear: Unknown exception");
  }
  return lua_error(L);
}



/** bool lubyk::Worker::fdErrorIsSet(int fd)
 * include/lubyk/worker.h:177
 */
static int Worker_fdErrorIsSet(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    int fd = dubL_checkint(L, 2);
    bool  retval__ = self->fdErrorIsSet(fd);
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "fdErrorIsSet: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "fdErrorIsSet: Unknown exception");
  }
  return lua_error(L);
}



/** void lubyk::Worker::fdErrorSet(int fd)
 * include/lubyk/worker.h:172
 */
static int Worker_fdErrorSet(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    int fd = dubL_checkint(L, 2);
    self->fdErrorSet(fd);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "fdErrorSet: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "fdErrorSet: Unknown exception");
  }
  return lua_error(L);
}



/** void lubyk::Worker::fdReadClear(int fd)
 * include/lubyk/worker.h:149
 */
static int Worker_fdReadClear(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    int fd = dubL_checkint(L, 2);
    self->fdReadClear(fd);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "fdReadClear: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "fdReadClear: Unknown exception");
  }
  return lua_error(L);
}



/** bool lubyk::Worker::fdReadIsSet(int fd)
 * include/lubyk/worker.h:145
 */
static int Worker_fdReadIsSet(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    int fd = dubL_checkint(L, 2);
    bool  retval__ = self->fdReadIsSet(fd);
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "fdReadIsSet: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "fdReadIsSet: Unknown exception");
  }
  return lua_error(L);
}



/** void lubyk::Worker::fdReadSet(int fd)
 * include/lubyk/worker.h:140
 */
static int Worker_fdReadSet(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    int fd = dubL_checkint(L, 2);
    self->fdReadSet(fd);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "fdReadSet: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "fdReadSet: Unknown exception");
  }
  return lua_error(L);
}



/** void lubyk::Worker::fdWriteClear(int fd)
 * include/lubyk/worker.h:165
 */
static int Worker_fdWriteClear(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    int fd = dubL_checkint(L, 2);
    self->fdWriteClear(fd);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "fdWriteClear: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "fdWriteClear: Unknown exception");
  }
  return lua_error(L);
}



/** bool lubyk::Worker::fdWriteIsSet(int fd)
 * include/lubyk/worker.h:161
 */
static int Worker_fdWriteIsSet(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    int fd = dubL_checkint(L, 2);
    bool  retval__ = self->fdWriteIsSet(fd);
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "fdWriteIsSet: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "fdWriteIsSet: Unknown exception");
  }
  return lua_error(L);
}



/** void lubyk::Worker::fdWriteSet(int fd)
 * include/lubyk/worker.h:156
 */
static int Worker_fdWriteSet(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    int fd = dubL_checkint(L, 2);
    self->fdWriteSet(fd);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "fdWriteSet: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "fdWriteSet: Unknown exception");
  }
  return lua_error(L);
}



/** static Worker* lubyk::Worker::getWorker(lua_State *L)
 * include/lubyk/worker.h:210
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



/** int lubyk::Worker::maxFd()
 * include/lubyk/worker.h:203
 */
static int Worker_maxFd(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    int  retval__ = self->maxFd();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "maxFd: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "maxFd: Unknown exception");
  }
  return lua_error(L);
}



/** double lubyk::Worker::now()
 * include/lubyk/worker.h:134
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
 * include/lubyk/worker.h:132
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



/** int lubyk::Worker::select(float msec)
 * include/lubyk/worker.h:188
 */
static int Worker_select(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    float msec = dubL_checknumber(L, 2);
    int  retval__ = self->select(msec);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "select: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "select: Unknown exception");
  }
  return lua_error(L);
}



/** void lubyk::Worker::sleep(double duration)
 * include/lubyk/worker.h:94
 */
static int Worker_sleep(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    double duration = dubL_checknumber(L, 2);
    self->sleep(duration);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "sleep: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "sleep: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize lubyk::Worker::spawn(const char *script, lua_State *L)
 * include/lubyk/worker.h:108
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



/** void lubyk::Worker::test_lock()
 * include/lubyk/worker.h:123
 */
static int Worker_test_lock(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    self->test_lock();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "test_lock: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "test_lock: Unknown exception");
  }
  return lua_error(L);
}



/** void lubyk::Worker::test_unlock()
 * include/lubyk/worker.h:128
 */
static int Worker_test_unlock(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    self->test_unlock();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "test_unlock: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "test_unlock: Unknown exception");
  }
  return lua_error(L);
}



/** void lubyk::Worker::wait(double duration)
 * include/lubyk/worker.h:101
 */
static int Worker_wait(lua_State *L) {
  try {
    Worker *self = *((Worker**)dubL_checksdata(L, 1, "lubyk.Worker"));
    double duration = dubL_checknumber(L, 2);
    self->wait(duration);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "wait: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "wait: Unknown exception");
  }
  return lua_error(L);
}



/** int lubyk::Worker::waitpid(int pid)
 * include/lubyk/worker.h:112
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
  {"exit"              , Worker_exit},
  {"fdErrorClear"      , Worker_fdErrorClear},
  {"fdErrorIsSet"      , Worker_fdErrorIsSet},
  {"fdErrorSet"        , Worker_fdErrorSet},
  {"fdReadClear"       , Worker_fdReadClear},
  {"fdReadIsSet"       , Worker_fdReadIsSet},
  {"fdReadSet"         , Worker_fdReadSet},
  {"fdWriteClear"      , Worker_fdWriteClear},
  {"fdWriteIsSet"      , Worker_fdWriteIsSet},
  {"fdWriteSet"        , Worker_fdWriteSet},
  {"maxFd"             , Worker_maxFd},
  {"now"               , Worker_now},
  {"run"               , Worker_run},
  {"select"            , Worker_select},
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
