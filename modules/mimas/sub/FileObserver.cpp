#include "mimas/FileObserver.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::FileObserver::FileObserver()
 * include/mimas/FileObserver.h:55
 */
static int FileObserver_FileObserver(lua_State *L) {
  try {
    FileObserver * retval__ = new FileObserver();
    // The class inherits from 'LuaCallback', use lua_init instead of pushclass.
    return retval__->luaInit(L, retval__, "mimas.FileObserver");
  } catch (std::exception &e) {
    lua_pushfstring(L, "FileObserver: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "FileObserver: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int FileObserver_destructor(lua_State *L) {
  FileObserver **userdata = (FileObserver**)dubL_checksdata_n(L, 1, "mimas.FileObserver");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int FileObserver__tostring(lua_State *L) {
  FileObserver **userdata = (FileObserver**)dubL_checksdata_n(L, 1, "mimas.FileObserver");
  
  
  lua_pushfstring(L, "<mimas.FileObserver: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::FileObserver::addPath(const char *path)
 * include/mimas/FileObserver.h:67
 */
static int FileObserver_addPath(lua_State *L) {
  try {
    FileObserver *self = *((FileObserver**)dubL_checksdata(L, 1, "mimas.FileObserver"));
    const char *path = dubL_checkstring(L, 2);
    self->addPath(path);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "addPath: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "addPath: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::FileObserver::removePath(const char *path)
 * include/mimas/FileObserver.h:71
 */
static int FileObserver_removePath(lua_State *L) {
  try {
    FileObserver *self = *((FileObserver**)dubL_checksdata(L, 1, "mimas.FileObserver"));
    const char *path = dubL_checkstring(L, 2);
    self->removePath(path);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "removePath: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "removePath: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg FileObserver_member_methods[] = {
  {"addPath"           , FileObserver_addPath},
  {"removePath"        , FileObserver_removePath},
  {"__tostring"        , FileObserver__tostring},
  {"__gc"              , FileObserver_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg FileObserver_namespace_methods[] = {
  {"FileObserver"      , FileObserver_FileObserver},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_FileObserver(lua_State *L) {
#else
extern "C" int luaopen_mimas_FileObserver(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.FileObserver");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, FileObserver_member_methods);
  // save meta-table in mimas.FileObserver_
  register_mt(L, "mimas", "FileObserver");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", FileObserver_namespace_methods);


	return 1;
}
