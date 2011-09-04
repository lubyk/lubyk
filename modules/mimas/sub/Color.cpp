#include "mimas/Color.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */


/** mimas::Color::Color()
 * include/mimas/Color.h:45
 */
static int Color_Color1(lua_State *L) {
  try {
    Color * retval__ = new Color();
    lua_pushclass<Color>(L, retval__, "mimas.Color");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "Color: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Color: Unknown exception");
  }
  return lua_error(L);
}



/** mimas::Color::Color(float h, float s=1.0, float v=1.0, float a=1.0)
 * include/mimas/Color.h:50
 */
static int Color_Color2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Color * retval__;
    float h = dubL_checknumber(L, 1);
    if (top__ < 2) {
      retval__ = new Color(h);
    } else {
      float s = dubL_checknumber(L, 2);
      if (top__ < 3) {
        retval__ = new Color(h, s);
      } else {
        float v = dubL_checknumber(L, 3);
        if (top__ < 4) {
          retval__ = new Color(h, s, v);
        } else {
          float a = dubL_checknumber(L, 4);
          retval__ = new Color(h, s, v, a);
        }
      }
    }
    lua_pushclass<Color>(L, retval__, "mimas.Color");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "Color: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Color: Unknown exception");
  }
  return lua_error(L);
}




/** Overloaded function chooser for Color(...) */
static int Color_Color(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TNUMBER) {
    return Color_Color2(L);
  } else if (top__ < 1) {
    return Color_Color1(L);
  } else {
    // use any to raise errors
    return Color_Color1(L);
  }
}


/* ============================ Destructor       ====================== */

static int Color_destructor(lua_State *L) {
  Color **userdata = (Color**)dubL_checksdata_n(L, 1, "mimas.Color");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Color__tostring(lua_State *L) {
  Color **userdata = (Color**)dubL_checksdata_n(L, 1, "mimas.Color");
  
  
  lua_pushfstring(L, "<mimas.Color: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** float mimas::Color::alpha() const 
 * include/mimas/Color.h:70
 */
static int Color_alpha(lua_State *L) {
  try {
    Color *self = *((Color**)dubL_checksdata(L, 1, "mimas.Color"));
    float  retval__ = self->alpha();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "alpha: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "alpha: Unknown exception");
  }
  return lua_error(L);
}



/** Color* mimas::Color::colorWithAlpha(float a)
 * include/mimas/Color.h:110
 */
static int Color_colorWithAlpha(lua_State *L) {
  try {
    Color *self = *((Color**)dubL_checksdata(L, 1, "mimas.Color"));
    float a = dubL_checknumber(L, 2);
    Color * retval__ = self->colorWithAlpha(a);
    lua_pushclass<Color>(L, retval__, "mimas.Color");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "colorWithAlpha: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "colorWithAlpha: Unknown exception");
  }
  return lua_error(L);
}



/** Color* mimas::Color::colorWithHue(float h)
 * include/mimas/Color.h:98
 */
static int Color_colorWithHue(lua_State *L) {
  try {
    Color *self = *((Color**)dubL_checksdata(L, 1, "mimas.Color"));
    float h = dubL_checknumber(L, 2);
    Color * retval__ = self->colorWithHue(h);
    lua_pushclass<Color>(L, retval__, "mimas.Color");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "colorWithHue: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "colorWithHue: Unknown exception");
  }
  return lua_error(L);
}



/** Color* mimas::Color::colorWithSaturation(float s)
 * include/mimas/Color.h:102
 */
static int Color_colorWithSaturation(lua_State *L) {
  try {
    Color *self = *((Color**)dubL_checksdata(L, 1, "mimas.Color"));
    float s = dubL_checknumber(L, 2);
    Color * retval__ = self->colorWithSaturation(s);
    lua_pushclass<Color>(L, retval__, "mimas.Color");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "colorWithSaturation: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "colorWithSaturation: Unknown exception");
  }
  return lua_error(L);
}



/** Color* mimas::Color::colorWithValue(float v)
 * include/mimas/Color.h:106
 */
static int Color_colorWithValue(lua_State *L) {
  try {
    Color *self = *((Color**)dubL_checksdata(L, 1, "mimas.Color"));
    float v = dubL_checknumber(L, 2);
    Color * retval__ = self->colorWithValue(v);
    lua_pushclass<Color>(L, retval__, "mimas.Color");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "colorWithValue: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "colorWithValue: Unknown exception");
  }
  return lua_error(L);
}



/** float mimas::Color::hue() const 
 * include/mimas/Color.h:58
 */
static int Color_hue(lua_State *L) {
  try {
    Color *self = *((Color**)dubL_checksdata(L, 1, "mimas.Color"));
    float  retval__ = self->hue();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "hue: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "hue: Unknown exception");
  }
  return lua_error(L);
}



/** float mimas::Color::saturation() const 
 * include/mimas/Color.h:62
 */
static int Color_saturation(lua_State *L) {
  try {
    Color *self = *((Color**)dubL_checksdata(L, 1, "mimas.Color"));
    float  retval__ = self->saturation();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "saturation: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "saturation: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Color::setAlpha(float a)
 * include/mimas/Color.h:94
 */
static int Color_setAlpha(lua_State *L) {
  try {
    Color *self = *((Color**)dubL_checksdata(L, 1, "mimas.Color"));
    float a = dubL_checknumber(L, 2);
    self->setAlpha(a);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setAlpha: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setAlpha: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Color::setHsva(float h, float s=1.0, float v=1.0, float a=1.0)
 * include/mimas/Color.h:78
 */
static int Color_setHsva(lua_State *L) {
  try {
    Color *self = *((Color**)dubL_checksdata(L, 1, "mimas.Color"));
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



/** void mimas::Color::setHue(float h)
 * include/mimas/Color.h:82
 */
static int Color_setHue(lua_State *L) {
  try {
    Color *self = *((Color**)dubL_checksdata(L, 1, "mimas.Color"));
    float h = dubL_checknumber(L, 2);
    self->setHue(h);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setHue: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setHue: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Color::setRgba(float r, float g, float b, float a=1.0)
 * include/mimas/Color.h:74
 */
static int Color_setRgba(lua_State *L) {
  try {
    Color *self = *((Color**)dubL_checksdata(L, 1, "mimas.Color"));
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



/** void mimas::Color::setSaturation(float s)
 * include/mimas/Color.h:86
 */
static int Color_setSaturation(lua_State *L) {
  try {
    Color *self = *((Color**)dubL_checksdata(L, 1, "mimas.Color"));
    float s = dubL_checknumber(L, 2);
    self->setSaturation(s);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setSaturation: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setSaturation: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Color::setValue(float v)
 * include/mimas/Color.h:90
 */
static int Color_setValue(lua_State *L) {
  try {
    Color *self = *((Color**)dubL_checksdata(L, 1, "mimas.Color"));
    float v = dubL_checknumber(L, 2);
    self->setValue(v);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setValue: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setValue: Unknown exception");
  }
  return lua_error(L);
}



/** float mimas::Color::value() const 
 * include/mimas/Color.h:66
 */
static int Color_value(lua_State *L) {
  try {
    Color *self = *((Color**)dubL_checksdata(L, 1, "mimas.Color"));
    float  retval__ = self->value();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "value: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "value: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Color_member_methods[] = {
  {"alpha"             , Color_alpha},
  {"colorWithAlpha"    , Color_colorWithAlpha},
  {"colorWithHue"      , Color_colorWithHue},
  {"colorWithSaturation", Color_colorWithSaturation},
  {"colorWithValue"    , Color_colorWithValue},
  {"hue"               , Color_hue},
  {"saturation"        , Color_saturation},
  {"setAlpha"          , Color_setAlpha},
  {"setHsva"           , Color_setHsva},
  {"setHue"            , Color_setHue},
  {"setRgba"           , Color_setRgba},
  {"setSaturation"     , Color_setSaturation},
  {"setValue"          , Color_setValue},
  {"value"             , Color_value},
  {"__tostring"        , Color__tostring},
  {"__gc"              , Color_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Color_namespace_methods[] = {
  {"Color"             , Color_Color},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Color(lua_State *L) {
#else
extern "C" int luaopen_mimas_Color(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Color");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Color_member_methods);
  // save meta-table in mimas.Color_
  register_mt(L, "mimas", "Color");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Color_namespace_methods);


	return 1;
}
