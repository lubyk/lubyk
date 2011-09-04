#include "mimas/Slider.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Slider::Slider(int type=(int) VerticalSliderType, QWidget *parent=0)
 * include/mimas/Slider.h:70
 */
static int Slider_Slider(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Slider * retval__;
    if (top__ < 1) {
      retval__ = new Slider();
    } else {
      int type = dubL_checkint(L, 1);
      if (top__ < 2) {
        retval__ = new Slider(type);
      } else {
        QWidget *parent = *((QWidget **)dubL_checksdata(L, 2, "mimas.QWidget"));
        retval__ = new Slider(type, parent);
      }
    }
    // The class inherits from 'LuaCallback', use lua_init instead of pushclass.
    return retval__->luaInit(L, retval__, "mimas.Slider");
  } catch (std::exception &e) {
    lua_pushfstring(L, "Slider: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Slider: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Slider_destructor(lua_State *L) {
  Slider **userdata = (Slider**)dubL_checksdata_n(L, 1, "mimas.Slider");

  
  // custom destructor
  if (*userdata) (*userdata)->luaDestroy();
  
  *userdata = NULL;
  return 0;
}




// test if class is deleted
static int Slider_deleted(lua_State *L) {
  Slider **userdata = (Slider**)dubL_checksdata_n(L, 1, "mimas.Slider");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Slider__tostring(lua_State *L) {
  Slider **userdata = (Slider**)dubL_checksdata_n(L, 1, "mimas.Slider");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.Slider: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.Slider: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** int mimas::Slider::borderWidth() const 
 * include/mimas/Slider.h:171
 */
static int Slider_borderWidth(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in borderWidth");
    int  retval__ = self->borderWidth();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "borderWidth: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "borderWidth: Unknown exception");
  }
  return lua_error(L);
}



/** QString mimas::Slider::cssClass() const 
 * include/mimas/Slider.h:92
 */
static int Slider_cssClass(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in cssClass");
    QString  retval__ = self->cssClass();
    lua_pushclass<QString>(L, retval__, "mimas.QString");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "cssClass: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "cssClass: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::Slider::height()
 * include/mimas/Slider.h:137
 */
static int Slider_height(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in height");
    int  retval__ = self->height();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "height: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "height: Unknown exception");
  }
  return lua_error(L);
}



/** float mimas::Slider::hue()
 * include/mimas/Slider.h:154
 */
static int Slider_hue(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in hue");
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



/** void mimas::Slider::move(int x, int y)
 * include/mimas/Slider.h:117
 */
static int Slider_move(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in move");
    int x = dubL_checkint(L, 2);
    int y = dubL_checkint(L, 3);
    self->move(x, y);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "move: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "move: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::Slider::name(lua_State *L)
 * include/mimas/Slider.h:106
 */
static int Slider_name(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in name");
    
    LuaStackSize  retval__ = self->name(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "name: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "name: Unknown exception");
  }
  return lua_error(L);
}



/** QObject* mimas::Slider::object()
 * include/mimas/Slider.h:100
 */
static int Slider_object(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in object");
    QObject * retval__ = self->object();
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "object: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "object: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Slider::resize(int w, int h)
 * include/mimas/Slider.h:121
 */
static int Slider_resize(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in resize");
    int w = dubL_checkint(L, 2);
    int h = dubL_checkint(L, 3);
    self->resize(w, h);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "resize: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "resize: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Slider::setBorderWidth(int width)
 * include/mimas/Slider.h:167
 */
static int Slider_setBorderWidth(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in setBorderWidth");
    int width = dubL_checkint(L, 2);
    self->setBorderWidth(width);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setBorderWidth: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setBorderWidth: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Slider::setHue(float hue)
 * include/mimas/Slider.h:149
 */
static int Slider_setHue(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in setHue");
    float hue = dubL_checknumber(L, 2);
    self->setHue(hue);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setHue: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setHue: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Slider::setName(const char *name)
 * include/mimas/Slider.h:113
 */
static int Slider_setName(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in setName");
    const char *name = dubL_checkstring(L, 2);
    self->setName(name);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setName: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setName: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Slider::setStyle(const char *text)
 * include/mimas/Slider.h:141
 */
static int Slider_setStyle(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in setStyle");
    const char *text = dubL_checkstring(L, 2);
    self->setStyle(text);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setStyle: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setStyle: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Slider::setStyleSheet(const char *text)
 * include/mimas/Slider.h:145
 */
static int Slider_setStyleSheet(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in setStyleSheet");
    const char *text = dubL_checkstring(L, 2);
    self->setStyleSheet(text);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setStyleSheet: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setStyleSheet: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Slider::setValue(double remote_value)
 * include/mimas/Slider.h:179
 */
static int Slider_setValue(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in setValue");
    double remote_value = dubL_checknumber(L, 2);
    self->setValue(remote_value);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setValue: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setValue: Unknown exception");
  }
  return lua_error(L);
}



/** virtual QSize mimas::Slider::sizeHint() const 
 * include/mimas/Slider.h:159
 */
static int Slider_sizeHint(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in sizeHint");
    QSize  retval__ = self->sizeHint();
    lua_pushclass<QSize>(L, retval__, "mimas.QSize");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "sizeHint: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "sizeHint: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Slider::sliderChangedSlot(double value)
 * include/mimas/Slider.h:188
 */
static int Slider_sliderChangedSlot(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in sliderChangedSlot");
    double value = dubL_checknumber(L, 2);
    self->sliderChangedSlot(value);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "sliderChangedSlot: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "sliderChangedSlot: Unknown exception");
  }
  return lua_error(L);
}



/** QWidget* mimas::Slider::widget()
 * include/mimas/Slider.h:96
 */
static int Slider_widget(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in widget");
    QWidget * retval__ = self->widget();
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "widget: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "widget: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::Slider::width()
 * include/mimas/Slider.h:133
 */
static int Slider_width(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in width");
    int  retval__ = self->width();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "width: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "width: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::Slider::x()
 * include/mimas/Slider.h:125
 */
static int Slider_x(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in x");
    int  retval__ = self->x();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "x: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "x: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::Slider::y()
 * include/mimas/Slider.h:129
 */
static int Slider_y(lua_State *L) {
  try {
    Slider *self = *((Slider**)dubL_checksdata(L, 1, "mimas.Slider"));
    if (!self) throw dub::Exception("Using deleted mimas.Slider in y");
    int  retval__ = self->y();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "y: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "y: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Slider_member_methods[] = {
  {"borderWidth"       , Slider_borderWidth},
  {"cssClass"          , Slider_cssClass},
  {"height"            , Slider_height},
  {"hue"               , Slider_hue},
  {"move"              , Slider_move},
  {"name"              , Slider_name},
  {"object"            , Slider_object},
  {"resize"            , Slider_resize},
  {"setBorderWidth"    , Slider_setBorderWidth},
  {"setHue"            , Slider_setHue},
  {"setName"           , Slider_setName},
  {"setStyle"          , Slider_setStyle},
  {"setStyleSheet"     , Slider_setStyleSheet},
  {"setValue"          , Slider_setValue},
  {"sizeHint"          , Slider_sizeHint},
  {"sliderChangedSlot" , Slider_sliderChangedSlot},
  {"widget"            , Slider_widget},
  {"width"             , Slider_width},
  {"x"                 , Slider_x},
  {"y"                 , Slider_y},
  {"__tostring"        , Slider__tostring},
  {"__gc"              , Slider_destructor},
  {"deleted"           , Slider_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg Slider_namespace_methods[] = {
  {"Slider"            , Slider_Slider},
  {NULL, NULL},
};


static const struct lua_constants_Reg Slider_namespace_constants[] = {
  {"DefaultWidth"      , mimas::Slider::DefaultWidth},
  {"DefaultHeight"     , mimas::Slider::DefaultHeight},
  {"HorizontalSliderType", mimas::Slider::HorizontalSliderType},
  {"VerticalSliderType", mimas::Slider::VerticalSliderType},
  {NULL, NULL},
};


#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Slider_core(lua_State *L) {
#else
extern "C" int luaopen_mimas_Slider_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Slider");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Slider_member_methods);
  // save meta-table in mimas.Slider_
  register_mt(L, "mimas", "Slider");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Slider_namespace_methods);


  // register class enums
  register_constants(L, "mimas.Slider_const", Slider_namespace_constants);

	return 1;
}
