#include "mimas/DataSource.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::DataSource::DataSource()
 * include/mimas/DataSource.h:57
 */
static int DataSource_DataSource(lua_State *L) {
  try {
    DataSource * retval__ = new DataSource();
    // The class inherits from 'LuaCallback', use lua_init instead of pushclass.
    return retval__->luaInit(L, retval__, "mimas.DataSource");
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.DataSource.DataSource: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.DataSource.DataSource: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int DataSource_destructor(lua_State *L) {
  DataSource **userdata = (DataSource**)dubL_checksdata_n(L, 1, "mimas.DataSource");

  
  // custom destructor
  if (*userdata) (*userdata)->dub_destroy();
  
  *userdata = NULL;
  return 0;
}




// test if class is deleted
static int DataSource_deleted(lua_State *L) {
  DataSource **userdata = (DataSource**)dubL_checksdata_n(L, 1, "mimas.DataSource");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int DataSource__tostring(lua_State *L) {
  DataSource **userdata = (DataSource**)dubL_checksdata_n(L, 1, "mimas.DataSource");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.DataSource: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.DataSource: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::DataSource::emitLayoutChanged()
 * include/mimas/DataSource.h:68
 */
static int DataSource_emitLayoutChanged(lua_State *L) {
  try {
    DataSource *self__ = *((DataSource**)dubL_checksdata(L, 1, "mimas.DataSource"));
    if (!self__) throw dub::Exception("Using deleted mimas.DataSource in emitLayoutChanged");
    self__->emitLayoutChanged();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.DataSource.emitLayoutChanged: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.DataSource.emitLayoutChanged: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::DataSource::reset()
 * include/mimas/DataSource.h:63
 */
static int DataSource_reset(lua_State *L) {
  try {
    DataSource *self__ = *((DataSource**)dubL_checksdata(L, 1, "mimas.DataSource"));
    if (!self__) throw dub::Exception("Using deleted mimas.DataSource in reset");
    self__->reset();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.DataSource.reset: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.DataSource.reset: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg DataSource_member_methods[] = {
  {"emitLayoutChanged" , DataSource_emitLayoutChanged},
  {"reset"             , DataSource_reset},
  {"__tostring"        , DataSource__tostring},
  {"__gc"              , DataSource_destructor},
  {"deleted"           , DataSource_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg DataSource_namespace_methods[] = {
  {"DataSource"        , DataSource_DataSource},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_DataSource(lua_State *L) {
#else
extern "C" int luaopen_mimas_DataSource(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.DataSource");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, DataSource_member_methods);
  // save meta-table in mimas.DataSource_
  register_mt(L, "mimas", "DataSource");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", DataSource_namespace_methods);


	return 1;
}
