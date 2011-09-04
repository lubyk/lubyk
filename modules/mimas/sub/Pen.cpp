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
    lua_pushfstring(L, "Pen: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Pen: Unknown exception");
  }
  return lua_error(L);
}



/** mimas::Pen::Pen(float width, const Color &color, int style=Qt::SolidLine, int cap=Qt::SquareCap, int join=Qt::MiterJoin)
 * include/mimas/Pen.h:49
 */
static int Pen_Pen2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Pen * retval__;
    float width = dubL_checknumber(L, 1);
    const Color *color = *((const Color **)dubL_checksdata(L, 2, "mimas.Color"));
    if (top__ < 3) {
      retval__ = new Pen(width, *color);
    } else {
      int style = dubL_checkint(L, 3);
      if (top__ < 4) {
        retval__ = new Pen(width, *color, style);
      } else {
        int cap = dubL_checkint(L, 4);
        if (top__ < 5) {
          retval__ = new Pen(width, *color, style, cap);
        } else {
          int join = dubL_checkint(L, 5);
          retval__ = new Pen(width, *color, style, cap, join);
        }
      }
    }
    lua_pushclass<Pen>(L, retval__, "mimas.Pen");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "Pen: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Pen: Unknown exception");
  }
  return lua_error(L);
}



/** mimas::Pen::Pen(float width, float h, float s=1.0, float v=1.0, float a=1.0)
 * include/mimas/Pen.h:52
 */
static int Pen_Pen3(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Pen * retval__;
    float width = dubL_checknumber(L, 1);
    float h = dubL_checknumber(L, 2);
    if (top__ < 3) {
      retval__ = new Pen(width, h);
    } else {
      float s = dubL_checknumber(L, 3);
      if (top__ < 4) {
        retval__ = new Pen(width, h, s);
      } else {
        float v = dubL_checknumber(L, 4);
        if (top__ < 5) {
          retval__ = new Pen(width, h, s, v);
        } else {
          float a = dubL_checknumber(L, 5);
          retval__ = new Pen(width, h, s, v, a);
        }
      }
    }
    lua_pushclass<Pen>(L, retval__, "mimas.Pen");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "Pen: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Pen: Unknown exception");
  }
  return lua_error(L);
}




/** Overloaded function chooser for Pen(...) */
static int Pen_Pen(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TNUMBER) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TNUMBER) {
      return Pen_Pen3(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "mimas.Color")) {
      return Pen_Pen2(L);
    } else {
      // use any to raise errors
      return Pen_Pen2(L);
    }
  } else if (top__ < 1) {
    return Pen_Pen1(L);
  } else {
    // use any to raise errors
    return Pen_Pen1(L);
  }
}


/* ============================ Destructor       ====================== */

static int Pen_destructor(lua_State *L) {
  Pen **userdata = (Pen**)dubL_checksdata_n(L, 1, "mimas.Pen");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Pen__tostring(lua_State *L) {
  Pen **userdata = (Pen**)dubL_checksdata_n(L, 1, "mimas.Pen");
  
  
  lua_pushfstring(L, "<mimas.Pen: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::Pen::setCapStyle(int style)
 * include/mimas/Pen.h:80
 */
static int Pen_setCapStyle(lua_State *L) {
  try {
    Pen *self = *((Pen**)dubL_checksdata(L, 1, "mimas.Pen"));
    int style = dubL_checkint(L, 2);
    self->setCapStyle(style);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setCapStyle: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setCapStyle: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Pen::setColor(const Color &color)
 * include/mimas/Pen.h:66
 */
static int Pen_setColor(lua_State *L) {
  try {
    Pen *self = *((Pen**)dubL_checksdata(L, 1, "mimas.Pen"));
    const Color *color = *((const Color **)dubL_checksdata(L, 2, "mimas.Color"));
    self->setColor(*color);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setColor: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setColor: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Pen::setHsva(float h, float s=1.0, float v=1.0, float a=1.0)
 * include/mimas/Pen.h:70
 */
static int Pen_setHsva(lua_State *L) {
  try {
    Pen *self = *((Pen**)dubL_checksdata(L, 1, "mimas.Pen"));
    int top__ = lua_gettop(L);
    float h = dubL_checknumber(L, 2);
    if (top__ < 3) {
      self->setHsva(h);
    } else {
      float s = dubL_checknumber(L, 3);
      if (top__ < 4) {
        self->setHsva(h, s);
      } else {
        float v = dubL_checknumber(L, 4);
        if (top__ < 5) {
          self->setHsva(h, s, v);
        } else {
          float a = dubL_checknumber(L, 5);
          self->setHsva(h, s, v, a);
        }
      }
    }
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setHsva: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setHsva: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Pen::setJoinStyle(int style)
 * include/mimas/Pen.h:84
 */
static int Pen_setJoinStyle(lua_State *L) {
  try {
    Pen *self = *((Pen**)dubL_checksdata(L, 1, "mimas.Pen"));
    int style = dubL_checkint(L, 2);
    self->setJoinStyle(style);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setJoinStyle: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setJoinStyle: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Pen::setRgba(float r, float g, float b, float a=1.0)
 * include/mimas/Pen.h:62
 */
static int Pen_setRgba(lua_State *L) {
  try {
    Pen *self = *((Pen**)dubL_checksdata(L, 1, "mimas.Pen"));
    int top__ = lua_gettop(L);
    float r = dubL_checknumber(L, 2);
    float g = dubL_checknumber(L, 3);
    float b = dubL_checknumber(L, 4);
    if (top__ < 5) {
      self->setRgba(r, g, b);
    } else {
      float a = dubL_checknumber(L, 5);
      self->setRgba(r, g, b, a);
    }
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setRgba: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setRgba: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Pen::setStyle(int style)
 * include/mimas/Pen.h:76
 */
static int Pen_setStyle(lua_State *L) {
  try {
    Pen *self = *((Pen**)dubL_checksdata(L, 1, "mimas.Pen"));
    int style = dubL_checkint(L, 2);
    self->setStyle(style);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setStyle: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setStyle: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Pen::setWidth(float w)
 * include/mimas/Pen.h:58
 */
static int Pen_setWidth(lua_State *L) {
  try {
    Pen *self = *((Pen**)dubL_checksdata(L, 1, "mimas.Pen"));
    float w = dubL_checknumber(L, 2);
    self->setWidth(w);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setWidth: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setWidth: Unknown exception");
  }
  return lua_error(L);
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
