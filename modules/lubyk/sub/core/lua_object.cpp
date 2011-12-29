#include "lubyk/lua_object.h"
#include "lua_cpp_helper.h"

using namespace lubyk;

LuaObject::LuaObject() throw ()
    : userdata_(NULL),
      worker_(NULL),
      class_name_("??") {
}

ThreadedLuaObject::ThreadedLuaObject() throw()
    : LuaObject(),
      thread_in_env_idx_(0),
      lua_(NULL) {
}
      

LuaObject::~LuaObject() {
  if (userdata_) {
    // Cut link from Lua to this object
    *userdata_ = NULL;
  }
}

/** Deleting from Lua.
 */
void LuaObject::luaDestroy() {
  luaCleanup();
  delete this;
}

void LuaObject::luaCleanup() {
  // Cut link from this object to the Lua.
  userdata_ = NULL;
}

int LuaObject::luaInit(lua_State *L, void *ptr, const char *type_name) throw() {
  worker_ = Worker::getWorker(L);
  // ... <self> or new table
  setupSuper(L, ptr); // creates self if there is no table (without a 'super' field)
  // ... <self>.super = userdata
  // ... <self> <udata>
  setupMetatable(L, type_name);
  // ... <self> <udata>
  lua_pop(L, 1);
  // <self>
  return 1;
}

int ThreadedLuaObject::luaInit(lua_State *L, void *ptr, const char *type_name) throw() {
  worker_ = Worker::getWorker(L);
  // ... <self> or new table
  // creates self if there is no table (without a 'super' field)
  setupSuper(L, ptr);
  // ... <self>.super = userdata
  // ... <self> <udata>
  setupMetatable(L, type_name);
  // ... <self> <udata>
  setupLuaThread(L);
  // <self>
  return 1;
}

void LuaObject::setupSuper(lua_State *L, void *ptr) throw() {
  if (!lua_istable(L, -1)) {
    lua_newtable(L);
  }
  // ... <self>
  userdata_ = (void**)lua_newuserdata(L, sizeof(void**));
  *userdata_ = ptr;
  // ... <self> <udata>
  lua_pushlstring(L, "super", 5);
  // ... <self> <udata> <"super">
  lua_pushvalue(L, -2);
  // ... <self> <udata> <"super"> <udata>
  lua_rawset(L, -4);
  // self.super = udata
  // ... <self> <udata>
}

void LuaObject::setupMetatable(lua_State *L, const char *type_name) throw() {
  // set metatable
  class_name_ = type_name;
  luaL_getmetatable(L, type_name);
  // ... <self> <udata> <mt>
  lua_pushvalue(L, -1);
  // ... <self> <udata> <mt> <mt>
  lua_setmetatable(L, -3);
  // ... <self> <udata> <mt>
  lua_setmetatable(L, -3);
  // ... <self> <udata>
}

// dependency relationship:
// self.super ---> udata ---> fenv ---> thread ---> self
//
//   Thanks to Robert G. Jakabosky for the idea to use lua_xmove
//   instead of weak tables to store the function reference.
void ThreadedLuaObject::setupLuaThread(lua_State *L) throw() {
  // ... <self> <udata>
  // Create env table
  lua_newtable(L);
  // ... <self> <udata> <env>
  lua_pushvalue(L, -1);
  // ... <self> <udata> <env> <env>
  if (!lua_setfenv(L, -3)) {
    luaL_error(L, "Could not set userdata env on '%s'.", lua_typename(L, lua_type(L, -3)));
  }

  // ... <self> <udata> <env>
  if (lua_) {
    // remove from env
    luaL_unref(L, -1, thread_in_env_idx_);
  }

  lua_ = lua_newthread(L);
  // ... <self> <udata> <env> <thread>
  // Store the thread in the Thread/Socket's environment table so it is not GC too soon
  thread_in_env_idx_ = luaL_ref(L, -2);
  // ... <self> <udata> <env>
  lua_pop(L, 2);
  // ... <self>

  // Transfer copies of <self> to thread stack
  lua_pushvalue(L, -1);
  // ... <self> <self>
  lua_xmove(L, lua_, 1);
  // lua_: <self>
  // L:    <self>
}

bool ThreadedLuaObject::pushLuaCallbackl(const char *method, int len) const {
  // FIXME: disable, we should not need this anymore (except in mimas
  //        where we can use the main thread).
  lua_State *L = lua_;
  // <self>
  lua_pushlstring(L, method, len);
  // <self> <"method">
  lua_gettable(L, 1);
  // <self> <?>
  if (lua_isnil(L, -1)) {
    lua_pop(L, 1);
    // printf("%s '%s' callback not set.\n", class_name_, method);
    return false;
  } else {
    lua_pushvalue(L, 1);
    // <self> <func> <self>
    return true;
  }
}


