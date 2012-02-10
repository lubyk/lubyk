#include "mimas/Painter.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Painter::Painter(QWidget *widget)
 * include/mimas/Painter.h:53
 */
static int Painter_Painter(lua_State *L) {
  try {
    QWidget *widget = *((QWidget **)dubL_checksdata(L, 1, "mimas.QWidget"));
    Painter * retval__ = new Painter(widget);
    lua_pushclass2<Painter>(L, retval__, "mimas.Painter");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "Painter: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Painter: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Painter_destructor(lua_State *L) {
  Painter **userdata = (Painter**)dubL_checksdata_n(L, 1, "mimas.Painter");

  
  // custom destructor
  Painter *self = *userdata;
  if (self) self->noDestroy(L);
  
  *userdata = NULL;
  return 0;
}



// test if class is deleted
static int Painter_deleted(lua_State *L) {
  Painter **userdata = (Painter**)dubL_checksdata_n(L, 1, "mimas.Painter");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Painter__tostring(lua_State *L) {
  Painter **userdata = (Painter**)dubL_checksdata_n(L, 1, "mimas.Painter");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.Painter: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.Painter: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::Painter::drawChord(int x, int y, int width, int height, float startAngle, float spanAngle)
 * include/mimas/Painter.h:125
 */
static int Painter_drawChord(lua_State *L) {
  try {
    Painter *self = *((Painter**)dubL_checksdata(L, 1, "mimas.Painter"));
    if (!self) throw dub::Exception("Using deleted mimas.Painter in drawChord");
    int x = dubL_checkint(L, 2);
    int y = dubL_checkint(L, 3);
    int width = dubL_checkint(L, 4);
    int height = dubL_checkint(L, 5);
    float startAngle = dubL_checknumber(L, 6);
    float spanAngle = dubL_checknumber(L, 7);
    self->drawChord(x, y, width, height, startAngle, spanAngle);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "drawChord: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "drawChord: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Painter::drawEllipse(int x, int y, int width, int height)
 * include/mimas/Painter.h:144
 */
static int Painter_drawEllipse(lua_State *L) {
  try {
    Painter *self = *((Painter**)dubL_checksdata(L, 1, "mimas.Painter"));
    if (!self) throw dub::Exception("Using deleted mimas.Painter in drawEllipse");
    int x = dubL_checkint(L, 2);
    int y = dubL_checkint(L, 3);
    int width = dubL_checkint(L, 4);
    int height = dubL_checkint(L, 5);
    self->drawEllipse(x, y, width, height);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "drawEllipse: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "drawEllipse: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Painter::drawLine(int x1, int y1, int x2, int y2)
 * include/mimas/Painter.h:118
 */
static int Painter_drawLine(lua_State *L) {
  try {
    Painter *self = *((Painter**)dubL_checksdata(L, 1, "mimas.Painter"));
    if (!self) throw dub::Exception("Using deleted mimas.Painter in drawLine");
    int x1 = dubL_checkint(L, 2);
    int y1 = dubL_checkint(L, 3);
    int x2 = dubL_checkint(L, 4);
    int y2 = dubL_checkint(L, 5);
    self->drawLine(x1, y1, x2, y2);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "drawLine: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "drawLine: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Painter::drawPath(const Path &path)
 * include/mimas/Painter.h:74
 */
static int Painter_drawPath(lua_State *L) {
  try {
    Painter *self = *((Painter**)dubL_checksdata(L, 1, "mimas.Painter"));
    if (!self) throw dub::Exception("Using deleted mimas.Painter in drawPath");
    const Path *path = *((const Path **)dubL_checksdata(L, 2, "mimas.Path"));
    self->drawPath(*path);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "drawPath: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "drawPath: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Painter::drawPoint(int x, int y)
 * include/mimas/Painter.h:112
 */
static int Painter_drawPoint(lua_State *L) {
  try {
    Painter *self = *((Painter**)dubL_checksdata(L, 1, "mimas.Painter"));
    if (!self) throw dub::Exception("Using deleted mimas.Painter in drawPoint");
    int x = dubL_checkint(L, 2);
    int y = dubL_checkint(L, 3);
    self->drawPoint(x, y);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "drawPoint: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "drawPoint: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Painter::drawRect(float x, float y, float w, float h)
 * include/mimas/Painter.h:131
 */
static int Painter_drawRect(lua_State *L) {
  try {
    Painter *self = *((Painter**)dubL_checksdata(L, 1, "mimas.Painter"));
    if (!self) throw dub::Exception("Using deleted mimas.Painter in drawRect");
    float x = dubL_checknumber(L, 2);
    float y = dubL_checknumber(L, 3);
    float w = dubL_checknumber(L, 4);
    float h = dubL_checknumber(L, 5);
    self->drawRect(x, y, w, h);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "drawRect: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "drawRect: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Painter::drawRoundedRect(float x, float y, float w, float h, float xRadius, lua_State *L)
 * include/mimas/Painter.h:137
 */
static int Painter_drawRoundedRect(lua_State *L) {
  try {
    Painter *self = *((Painter**)dubL_checksdata(L, 1, "mimas.Painter"));
    if (!self) throw dub::Exception("Using deleted mimas.Painter in drawRoundedRect");
    float x = dubL_checknumber(L, 2);
    float y = dubL_checknumber(L, 3);
    float w = dubL_checknumber(L, 4);
    float h = dubL_checknumber(L, 5);
    float xRadius = dubL_checknumber(L, 6);
    
    self->drawRoundedRect(x, y, w, h, xRadius, L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "drawRoundedRect: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "drawRoundedRect: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Painter::drawText(float x, float y, float w, float h, int flags, const char *text)
 * include/mimas/Painter.h:150
 */
static int Painter_drawText(lua_State *L) {
  try {
    Painter *self = *((Painter**)dubL_checksdata(L, 1, "mimas.Painter"));
    if (!self) throw dub::Exception("Using deleted mimas.Painter in drawText");
    float x = dubL_checknumber(L, 2);
    float y = dubL_checknumber(L, 3);
    float w = dubL_checknumber(L, 4);
    float h = dubL_checknumber(L, 5);
    int flags = dubL_checkint(L, 6);
    const char *text = dubL_checkstring(L, 7);
    self->drawText(x, y, w, h, flags, text);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "drawText: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "drawText: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Painter::fillRect(int x, int y, int width, int height, const Color &color)
 * include/mimas/Painter.h:70
 */
static int Painter_fillRect(lua_State *L) {
  try {
    Painter *self = *((Painter**)dubL_checksdata(L, 1, "mimas.Painter"));
    if (!self) throw dub::Exception("Using deleted mimas.Painter in fillRect");
    int x = dubL_checkint(L, 2);
    int y = dubL_checkint(L, 3);
    int width = dubL_checkint(L, 4);
    int height = dubL_checkint(L, 5);
    const Color *color = *((const Color **)dubL_checksdata(L, 6, "mimas.Color"));
    self->fillRect(x, y, width, height, *color);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "fillRect: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "fillRect: Unknown exception");
  }
  return lua_error(L);
}




/** void mimas::Painter::setBrush(const Brush &brush)
 * include/mimas/Painter.h:90
 */
static int Painter_setBrush1(lua_State *L) {
  try {
    Painter *self = *((Painter**)dubL_checksdata(L, 1, "mimas.Painter"));
    if (!self) throw dub::Exception("Using deleted mimas.Painter in setBrush");
    const Brush *brush = *((const Brush **)dubL_checksdata(L, 2, "mimas.Brush"));
    self->setBrush(*brush);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setBrush: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setBrush: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Painter::setBrush(const Color &color)
 * include/mimas/Painter.h:94
 */
static int Painter_setBrush2(lua_State *L) {
  try {
    Painter *self = *((Painter**)dubL_checksdata(L, 1, "mimas.Painter"));
    if (!self) throw dub::Exception("Using deleted mimas.Painter in setBrush");
    const Color *color = *((const Color **)dubL_checksdata(L, 2, "mimas.Color"));
    self->setBrush(*color);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setBrush: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setBrush: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Painter::setBrush(float h, float s=1.0, float v=1.0, float a=1.0)
 * include/mimas/Painter.h:106
 */
static int Painter_setBrush3(lua_State *L) {
  try {
    Painter *self = *((Painter**)dubL_checksdata(L, 1, "mimas.Painter"));
    if (!self) throw dub::Exception("Using deleted mimas.Painter in setBrush");
    int top__ = lua_gettop(L);
    float h = dubL_checknumber(L, 2);
    if (top__ < 3) {
      self->setBrush(h);
    } else {
      float s = dubL_checknumber(L, 3);
      if (top__ < 4) {
        self->setBrush(h, s);
      } else {
        float v = dubL_checknumber(L, 4);
        if (top__ < 5) {
          self->setBrush(h, s, v);
        } else {
          float a = dubL_checknumber(L, 5);
          self->setBrush(h, s, v, a);
        }
      }
    }
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setBrush: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setBrush: Unknown exception");
  }
  return lua_error(L);
}




/** Overloaded function chooser for setBrush(...) */
static int Painter_setBrush(lua_State *L) {
  int type__ = lua_type(L, 2);
  if (type__ == LUA_TNUMBER) {
    return Painter_setBrush3(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "mimas.Color")) {
    return Painter_setBrush2(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "mimas.Brush")) {
    return Painter_setBrush1(L);
  } else {
    // use any to raise errors
    return Painter_setBrush1(L);
  }
}



/** void mimas::Painter::setPen(const Pen &pen)
 * include/mimas/Painter.h:78
 */
static int Painter_setPen1(lua_State *L) {
  try {
    Painter *self = *((Painter**)dubL_checksdata(L, 1, "mimas.Painter"));
    if (!self) throw dub::Exception("Using deleted mimas.Painter in setPen");
    const Pen *pen = *((const Pen **)dubL_checksdata(L, 2, "mimas.Pen"));
    self->setPen(*pen);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setPen: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setPen: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Painter::setPen(float width, const Color &color)
 * include/mimas/Painter.h:82
 */
static int Painter_setPen2(lua_State *L) {
  try {
    Painter *self = *((Painter**)dubL_checksdata(L, 1, "mimas.Painter"));
    if (!self) throw dub::Exception("Using deleted mimas.Painter in setPen");
    float width = dubL_checknumber(L, 2);
    const Color *color = *((const Color **)dubL_checksdata(L, 3, "mimas.Color"));
    self->setPen(width, *color);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setPen: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setPen: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Painter::setPen(float width, float h, float s=1.0, float v=1.0, float a=1.0)
 * include/mimas/Painter.h:86
 */
static int Painter_setPen3(lua_State *L) {
  try {
    Painter *self = *((Painter**)dubL_checksdata(L, 1, "mimas.Painter"));
    if (!self) throw dub::Exception("Using deleted mimas.Painter in setPen");
    int top__ = lua_gettop(L);
    float width = dubL_checknumber(L, 2);
    float h = dubL_checknumber(L, 3);
    if (top__ < 4) {
      self->setPen(width, h);
    } else {
      float s = dubL_checknumber(L, 4);
      if (top__ < 5) {
        self->setPen(width, h, s);
      } else {
        float v = dubL_checknumber(L, 5);
        if (top__ < 6) {
          self->setPen(width, h, s, v);
        } else {
          float a = dubL_checknumber(L, 6);
          self->setPen(width, h, s, v, a);
        }
      }
    }
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setPen: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setPen: Unknown exception");
  }
  return lua_error(L);
}




/** Overloaded function chooser for setPen(...) */
static int Painter_setPen(lua_State *L) {
  int type__ = lua_type(L, 2);
  if (type__ == LUA_TNUMBER) {
    type__ = lua_type(L, 3);
    if (type__ == LUA_TNUMBER) {
      return Painter_setPen3(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 3, "mimas.Color")) {
      return Painter_setPen2(L);
    } else {
      // use any to raise errors
      return Painter_setPen2(L);
    }
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "mimas.Pen")) {
    return Painter_setPen1(L);
  } else {
    // use any to raise errors
    return Painter_setPen1(L);
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Painter_member_methods[] = {
  {"drawChord"         , Painter_drawChord},
  {"drawEllipse"       , Painter_drawEllipse},
  {"drawLine"          , Painter_drawLine},
  {"drawPath"          , Painter_drawPath},
  {"drawPoint"         , Painter_drawPoint},
  {"drawRect"          , Painter_drawRect},
  {"drawRoundedRect"   , Painter_drawRoundedRect},
  {"drawText"          , Painter_drawText},
  {"fillRect"          , Painter_fillRect},
  {"setBrush"          , Painter_setBrush},
  {"setPen"            , Painter_setPen},
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
  // save meta-table in mimas.Painter_
  register_mt(L, "mimas", "Painter");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Painter_namespace_methods);


	return 1;
}
