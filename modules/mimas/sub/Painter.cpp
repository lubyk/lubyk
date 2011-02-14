#include "mimas/Painter.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Painter::Painter(QWidget *widget)
 * include/mimas/Painter.h:48
 */
static int Painter_Painter(lua_State *L) {
  try {
    QWidget *widget = *((QWidget **)luaL_checkudata(L, 1, "mimas.QWidget"));
    Painter * retval__ = new Painter(widget);
    lua_pushclass2<Painter>(L, retval__, "mimas.Painter");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Painter.Painter: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Painter.Painter: Unknown exception");
  }
}


/* ============================ Destructor       ====================== */

static int Painter_destructor(lua_State *L) {
  Painter **userdata = (Painter**)luaL_checkudata(L, 1, "mimas.Painter");

  
  // custom destructor
  if (*userdata) (*userdata)->dub_destroy();
  
  *userdata = NULL;
  return 0;
}




// test if class is deleted
static int Painter_deleted(lua_State *L) {
  Painter **userdata = (Painter**)luaL_checkudata(L, 1, "mimas.Painter");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Painter__tostring(lua_State *L) {
  Painter **userdata = (Painter**)luaL_checkudata(L, 1, "mimas.Painter");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.Painter: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.Painter: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::Painter::fillRect(int x, int y, int width, int height, const Color &color)
 * include/mimas/Painter.h:55
 */
static int Painter_fillRect(lua_State *L) {
  try {
    Painter *self__ = *((Painter**)luaL_checkudata(L, 1, "mimas.Painter"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Painter in fillRect");
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    int width = luaL_checkint(L, 4);
    int height = luaL_checkint(L, 5);
    const Color *color = *((const Color **)luaL_checkudata(L, 6, "mimas.Color"));
    self__->fillRect(x, y, width, height, *color);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Painter.fillRect: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Painter.fillRect: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Painter_member_methods[] = {
  {"fillRect"          , Painter_fillRect},
  {"__tostring"        , Painter__tostring},
  {"__gc"              , Painter_destructor},
  {"deleted"           , Painter_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg Painter_namespace_methods[] = {
  {"Painter"           , Painter_Painter},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Painter(lua_State *L) {
#else
extern "C" int luaopen_mimas_Painter(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Painter");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Painter_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Painter_namespace_methods);


	return 1;
}
