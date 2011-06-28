#include "mimas/Brush.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */


/** mimas::Brush::Brush(int style=Qt::NoBrush)
 * include/mimas/Brush.h:46
 */
static int Brush_Brush1(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Brush * retval__;
    if (top__ < 1) {
      retval__ = new Brush();
    } else {
      int style = luaL_checkint(L, 1);
      retval__ = new Brush(style);
    }
    lua_pushclass<Brush>(L, retval__, "mimas.Brush");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Brush.Brush: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Brush.Brush: Unknown exception");
  }
}


/** mimas::Brush::Brush(const Color &color, int style=Qt::SolidPattern)
 * include/mimas/Brush.h:48
 */
static int Brush_Brush2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Brush * retval__;
    const Color *color = *((const Color **)luaL_checkudata(L, 1, "mimas.Color"));
    if (top__ < 2) {
      retval__ = new Brush(*color);
    } else {
      int style = luaL_checkint(L, 2);
      retval__ = new Brush(*color, style);
    }
    lua_pushclass<Brush>(L, retval__, "mimas.Brush");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Brush.Brush: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Brush.Brush: Unknown exception");
  }
}


/** mimas::Brush::Brush(float h, float s=1.0, float v=1.0, float a=1.0, int style=Qt::SolidPattern)
 * include/mimas/Brush.h:50
 */
static int Brush_Brush3(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Brush * retval__;
    float h = luaL_checknumber(L, 1);
    if (top__ < 2) {
      retval__ = new Brush(h);
    } else {
      float s = luaL_checknumber(L, 2);
      if (top__ < 3) {
        retval__ = new Brush(h, s);
      } else {
        float v = luaL_checknumber(L, 3);
        if (top__ < 4) {
          retval__ = new Brush(h, s, v);
        } else {
          float a = luaL_checknumber(L, 4);
          if (top__ < 5) {
            retval__ = new Brush(h, s, v, a);
          } else {
            int style = luaL_checkint(L, 5);
            retval__ = new Brush(h, s, v, a, style);
          }
        }
      }
    }
    lua_pushclass<Brush>(L, retval__, "mimas.Brush");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Brush.Brush: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Brush.Brush: Unknown exception");
  }
}



/** Overloaded function chooser for Brush(...) */
static int Brush_Brush(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "mimas.Color")) {
    return Brush_Brush2(L);
  } else if (type__ == LUA_TNUMBER) {
    type__ = lua_type(L, 2);
    if (top__ < 2) {
      return Brush_Brush1(L);
    } else if (type__ == LUA_TNUMBER) {
      return Brush_Brush3(L);
    } else {
      // use any to raise errors
      return Brush_Brush3(L);
    }
  } else {
    // use any to raise errors
    return Brush_Brush3(L);
  }
}


/* ============================ Destructor       ====================== */

static int Brush_destructor(lua_State *L) {
  Brush **userdata = (Brush**)luaL_checkudata(L, 1, "mimas.Brush");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Brush__tostring(lua_State *L) {
  Brush **userdata = (Brush**)luaL_checkudata(L, 1, "mimas.Brush");
  
  
  lua_pushfstring(L, "<mimas.Brush: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::Brush::setColor(const Color &color)
 * include/mimas/Brush.h:60
 */
static int Brush_setColor(lua_State *L) {
  try {
    Brush *self__ = *((Brush**)luaL_checkudata(L, 1, "mimas.Brush"));
    const Color *color = *((const Color **)luaL_checkudata(L, 2, "mimas.Color"));
    self__->setColor(*color);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Brush.setColor: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Brush.setColor: Unknown exception");
  }
}


/** void mimas::Brush::setStyle(int style)
 * include/mimas/Brush.h:56
 */
static int Brush_setStyle(lua_State *L) {
  try {
    Brush *self__ = *((Brush**)luaL_checkudata(L, 1, "mimas.Brush"));
    int style = luaL_checkint(L, 2);
    self__->setStyle(style);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Brush.setStyle: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Brush.setStyle: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Brush_member_methods[] = {
  {"setColor"          , Brush_setColor},
  {"setStyle"          , Brush_setStyle},
  {"__tostring"        , Brush__tostring},
  {"__gc"              , Brush_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Brush_namespace_methods[] = {
  {"Brush"             , Brush_Brush},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Brush(lua_State *L) {
#else
extern "C" int luaopen_mimas_Brush(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Brush");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Brush_member_methods);
  // save meta-table in mimas.Brush_
  register_mt(L, "mimas", "Brush");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Brush_namespace_methods);


	return 1;
}
