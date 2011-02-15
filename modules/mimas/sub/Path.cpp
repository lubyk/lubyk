#include "mimas/Path.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Path::Path()
 * include/mimas/Path.h:45
 */
static int Path_Path(lua_State *L) {
  try {
    Path * retval__ = new Path();
    lua_pushclass<Path>(L, retval__, "mimas.Path");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Path.Path: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Path.Path: Unknown exception");
  }
}


/* ============================ Destructor       ====================== */

static int Path_destructor(lua_State *L) {
  Path **userdata = (Path**)luaL_checkudata(L, 1, "mimas.Path");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Path__tostring(lua_State *L) {
  Path **userdata = (Path**)luaL_checkudata(L, 1, "mimas.Path");
  
  
  lua_pushfstring(L, "<mimas.Path: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::Path::addRect(float x, float y, float w, float h)
 * include/mimas/Path.h:61
 */
static int Path_addRect(lua_State *L) {
  try {
    Path *self__ = *((Path**)luaL_checkudata(L, 1, "mimas.Path"));
    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    float w = luaL_checknumber(L, 4);
    float h = luaL_checknumber(L, 5);
    self__->addRect(x, y, w, h);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Path.addRect: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Path.addRect: Unknown exception");
  }
}


/** void mimas::Path::cubicTo(float c1X, float c1Y, float c2X, float c2Y, float endPointX, float endPointY)
 * include/mimas/Path.h:57
 */
static int Path_cubicTo(lua_State *L) {
  try {
    Path *self__ = *((Path**)luaL_checkudata(L, 1, "mimas.Path"));
    float c1X = luaL_checknumber(L, 2);
    float c1Y = luaL_checknumber(L, 3);
    float c2X = luaL_checknumber(L, 4);
    float c2Y = luaL_checknumber(L, 5);
    float endPointX = luaL_checknumber(L, 6);
    float endPointY = luaL_checknumber(L, 7);
    self__->cubicTo(c1X, c1Y, c2X, c2Y, endPointX, endPointY);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Path.cubicTo: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Path.cubicTo: Unknown exception");
  }
}


/** void mimas::Path::lineTo(float x, float y)
 * include/mimas/Path.h:65
 */
static int Path_lineTo(lua_State *L) {
  try {
    Path *self__ = *((Path**)luaL_checkudata(L, 1, "mimas.Path"));
    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    self__->lineTo(x, y);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Path.lineTo: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Path.lineTo: Unknown exception");
  }
}


/** void mimas::Path::moveTo(float x, float y)
 * include/mimas/Path.h:51
 */
static int Path_moveTo(lua_State *L) {
  try {
    Path *self__ = *((Path**)luaL_checkudata(L, 1, "mimas.Path"));
    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    self__->moveTo(x, y);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Path.moveTo: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Path.moveTo: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Path_member_methods[] = {
  {"addRect"           , Path_addRect},
  {"cubicTo"           , Path_cubicTo},
  {"lineTo"            , Path_lineTo},
  {"moveTo"            , Path_moveTo},
  {"__tostring"        , Path__tostring},
  {"__gc"              , Path_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Path_namespace_methods[] = {
  {"Path"              , Path_Path},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Path(lua_State *L) {
#else
extern "C" int luaopen_mimas_Path(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Path");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Path_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Path_namespace_methods);


	return 1;
}
