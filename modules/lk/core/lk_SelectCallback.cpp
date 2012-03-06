/**
 *
 * MACHINE GENERATED FILE. DO NOT EDIT.
 *
 * Bindings for class SelectCallback
 *
 * This file has been generated by dub 2.1.~.
 */
#include "dub/dub.h"
#include "lk/SelectCallback.h"

using namespace lk;

/** lk::SelectCallback::SelectCallback(int fd)
 * include/lk/SelectCallback.h:72
 */
static int SelectCallback_SelectCallback(lua_State *L) {
  try {
    int fd = dub_checkint(L, 1);
    SelectCallback *retval__ = new SelectCallback(fd);
    retval__->pushobject(L, retval__, "lk.SelectCallback", true);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "lk.SelectCallback.SelectCallback: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "lk.SelectCallback.SelectCallback: Unknown exception");
  }
  return dub_error(L);
}

/** virtual lk::SelectCallback::~SelectCallback()
 * include/lk/SelectCallback.h:75
 */
static int SelectCallback__SelectCallback(lua_State *L) {
  try {
    DubUserdata *userdata = ((DubUserdata*)dub_checksdata_d(L, 1, "lk.SelectCallback"));
    if (userdata->gc) {
      SelectCallback *self = (SelectCallback *)userdata->ptr;
      self->luaDestroy();
    }
    userdata->gc = false;
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "lk.SelectCallback.~SelectCallback: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "lk.SelectCallback.~SelectCallback: Unknown exception");
  }
  return dub_error(L);
}

/** virtual void lk::SelectCallback::callback(bool read, bool write, bool timeout)
 * include/lk/SelectCallback.h:87
 */
static int SelectCallback_callback(lua_State *L) {
  try {
    SelectCallback *self = *((SelectCallback **)dub_checksdata(L, 1, "lk.SelectCallback"));
    bool read = dub_checkboolean(L, 2);
    bool write = dub_checkboolean(L, 3);
    bool timeout = dub_checkboolean(L, 4);
    self->callback(read, write, timeout);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "lk.SelectCallback.callback: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "lk.SelectCallback.callback: Unknown exception");
  }
  return dub_error(L);
}

/** int lk::SelectCallback::fd()
 * include/lk/SelectCallback.h:98
 */
static int SelectCallback_fd(lua_State *L) {
  try {
    SelectCallback *self = *((SelectCallback **)dub_checksdata(L, 1, "lk.SelectCallback"));
    lua_pushnumber(L, self->fd());
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "lk.SelectCallback.fd: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "lk.SelectCallback.fd: Unknown exception");
  }
  return dub_error(L);
}



// --=============================================== __tostring
static int SelectCallback___tostring(lua_State *L) {
  SelectCallback *self = *((SelectCallback **)dub_checksdata_n(L, 1, "lk.SelectCallback"));
  lua_pushfstring(L, "lk.SelectCallback: %p", self);
  
  return 1;
}

// --=============================================== METHODS

static const struct luaL_Reg SelectCallback_member_methods[] = {
  { "new"          , SelectCallback_SelectCallback },
  { "__gc"         , SelectCallback__SelectCallback },
  { "callback"     , SelectCallback_callback },
  { "fd"           , SelectCallback_fd    },
  { "__tostring"   , SelectCallback___tostring },
  { NULL, NULL},
};


extern "C" int luaopen_lk_SelectCallback(lua_State *L)
{
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "lk.SelectCallback");
  // <mt>

  // register member methods
  luaL_register(L, NULL, SelectCallback_member_methods);
  // save meta-table in lk
  dub_register(L, "lk", "SelectCallback_core");
  // <mt>
  lua_pop(L, 1);
  return 0;
}