#include "mimas/Pen.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */


/** mimas::Pen::Pen()
 * include/mimas/Pen.h:46
 */
static int Pen_Pen1(lua_State *L) {
  try {
    Pen * retval__ = new Pen();
    lua_pushclass<Pen>(L, retval__, "mimas.Pen");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Pen.Pen: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Pen.Pen: Unknown exception");
  }
}


/** mimas::Pen::Pen(float width, const Color &color, int style=Qt::SolidLine, int cap=Qt::SquareCap, int join=Qt::MiterJoin)
 * include/mimas/Pen.h:49
 */
static int Pen_Pen2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Pen * retval__;
    float width = luaL_checknumber(L, 1);
    const Color *color = *((const Color **)luaL_checkudata(L, 2, "mimas.Color"));
    if (top__ < 3) {
      retval__ = new Pen(width, *color);
    } else {
      int style = luaL_checkint(L, 3);
      if (top__ < 4) {
        retval__ = new Pen(width, *color, style);
      } else {
        int cap = luaL_checkint(L, 4);
        if (top__ < 5) {
          retval__ = new Pen(width, *color, style, cap);
        } else {
          int join = luaL_checkint(L, 5);
          retval__ = new Pen(width, *color, style, cap, join);
        }
      }
    }
    lua_pushclass<Pen>(L, retval__, "mimas.Pen");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Pen.Pen: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Pen.Pen: Unknown exception");
  }
}



/** Overloaded function chooser for Pen(...) */
static int Pen_Pen(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TNUMBER) {
    return Pen_Pen2(L);
  } else if (top__ < 1) {
    return Pen_Pen1(L);
  } else {
    // use any to raise errors
    return Pen_Pen1(L);
  }
}


/* ============================ Destructor       ====================== */

static int Pen_destructor(lua_State *L) {
  Pen **userdata = (Pen**)luaL_checkudata(L, 1, "mimas.Pen");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Pen__tostring(lua_State *L) {
  Pen **userdata = (Pen**)luaL_checkudata(L, 1, "mimas.Pen");
  
  
  lua_pushfstring(L, "<mimas.Pen: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::Pen::setCapStyle(int style)
 * include/mimas/Pen.h:77
 */
static int Pen_setCapStyle(lua_State *L) {
  try {
    Pen *self__ = *((Pen**)luaL_checkudata(L, 1, "mimas.Pen"));
    int style = luaL_checkint(L, 2);
    self__->setCapStyle(style);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Pen.setCapStyle: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Pen.setCapStyle: Unknown exception");
  }
}


/** void mimas::Pen::setColor(const Color &color)
 * include/mimas/Pen.h:63
 */
static int Pen_setColor(lua_State *L) {
  try {
    Pen *self__ = *((Pen**)luaL_checkudata(L, 1, "mimas.Pen"));
    const Color *color = *((const Color **)luaL_checkudata(L, 2, "mimas.Color"));
    self__->setColor(*color);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Pen.setColor: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Pen.setColor: Unknown exception");
  }
}


/** void mimas::Pen::setHsva(float h, float s=1.0, float v=1.0, float a=1.0)
 * include/mimas/Pen.h:67
 */
static int Pen_setHsva(lua_State *L) {
  try {
    Pen *self__ = *((Pen**)luaL_checkudata(L, 1, "mimas.Pen"));
    int top__ = lua_gettop(L);
    float h = luaL_checknumber(L, 2);
    if (top__ < 3) {
      self__->setHsva(h);
    } else {
      float s = luaL_checknumber(L, 3);
      if (top__ < 4) {
        self__->setHsva(h, s);
      } else {
        float v = luaL_checknumber(L, 4);
        if (top__ < 5) {
          self__->setHsva(h, s, v);
        } else {
          float a = luaL_checknumber(L, 5);
          self__->setHsva(h, s, v, a);
        }
      }
    }
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Pen.setHsva: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Pen.setHsva: Unknown exception");
  }
}


/** void mimas::Pen::setJoinStyle(int style)
 * include/mimas/Pen.h:81
 */
static int Pen_setJoinStyle(lua_State *L) {
  try {
    Pen *self__ = *((Pen**)luaL_checkudata(L, 1, "mimas.Pen"));
    int style = luaL_checkint(L, 2);
    self__->setJoinStyle(style);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Pen.setJoinStyle: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Pen.setJoinStyle: Unknown exception");
  }
}


/** void mimas::Pen::setRgba(float r, float g, float b, float a=1.0)
 * include/mimas/Pen.h:59
 */
static int Pen_setRgba(lua_State *L) {
  try {
    Pen *self__ = *((Pen**)luaL_checkudata(L, 1, "mimas.Pen"));
    int top__ = lua_gettop(L);
    float r = luaL_checknumber(L, 2);
    float g = luaL_checknumber(L, 3);
    float b = luaL_checknumber(L, 4);
    if (top__ < 5) {
      self__->setRgba(r, g, b);
    } else {
      float a = luaL_checknumber(L, 5);
      self__->setRgba(r, g, b, a);
    }
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Pen.setRgba: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Pen.setRgba: Unknown exception");
  }
}


/** void mimas::Pen::setStyle(int style)
 * include/mimas/Pen.h:73
 */
static int Pen_setStyle(lua_State *L) {
  try {
    Pen *self__ = *((Pen**)luaL_checkudata(L, 1, "mimas.Pen"));
    int style = luaL_checkint(L, 2);
    self__->setStyle(style);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Pen.setStyle: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Pen.setStyle: Unknown exception");
  }
}


/** void mimas::Pen::setWidth(float w)
 * include/mimas/Pen.h:55
 */
static int Pen_setWidth(lua_State *L) {
  try {
    Pen *self__ = *((Pen**)luaL_checkudata(L, 1, "mimas.Pen"));
    float w = luaL_checknumber(L, 2);
    self__->setWidth(w);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Pen.setWidth: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Pen.setWidth: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Pen_member_methods[] = {
  {"setCapStyle"       , Pen_setCapStyle},
  {"setColor"          , Pen_setColor},
  {"setHsva"           , Pen_setHsva},
  {"setJoinStyle"      , Pen_setJoinStyle},
  {"setRgba"           , Pen_setRgba},
  {"setStyle"          , Pen_setStyle},
  {"setWidth"          , Pen_setWidth},
  {"__tostring"        , Pen__tostring},
  {"__gc"              , Pen_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Pen_namespace_methods[] = {
  {"Pen"               , Pen_Pen},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Pen(lua_State *L) {
#else
extern "C" int luaopen_mimas_Pen(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Pen");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Pen_member_methods);
  // save meta-table in mimas.Pen_
  register_mt(L, "mimas", "Pen");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Pen_namespace_methods);


	return 1;
}
