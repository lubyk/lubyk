#include "mimas/Slider.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Slider::Slider(lubyk::Worker *worker, int type=(int) VerticalSliderType, QWidget *parent=0)
 * include/mimas/Slider.h:73
 */
static int Slider_Slider(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Slider * retval__;
    lubyk::Worker *worker = *((lubyk::Worker **)luaL_checkudata(L, 1, "lubyk.Worker"));
    if (top__ < 2) {
      retval__ = new Slider(worker);
    } else {
      int type = luaL_checkint(L, 2);
      if (top__ < 3) {
        retval__ = new Slider(worker, type);
      } else {
        QWidget *parent = *((QWidget **)luaL_checkudata(L, 3, "mimas.QWidget"));
        retval__ = new Slider(worker, type, parent);
      }
    }
    lua_pushclass2<Slider>(L, retval__, "mimas.Slider");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Slider.Slider: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Slider.Slider: Unknown exception");
  }
}


/* ============================ Destructor       ====================== */

static int Slider_destructor(lua_State *L) {
  Slider **userdata = (Slider**)luaL_checkudata(L, 1, "mimas.Slider");

  
  // custom destructor
  if (*userdata) (*userdata)->dub_destroy();
  
  *userdata = NULL;
  return 0;
}




// test if class is deleted
static int Slider_deleted(lua_State *L) {
  Slider **userdata = (Slider**)luaL_checkudata(L, 1, "mimas.Slider");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Slider__tostring(lua_State *L) {
  Slider **userdata = (Slider**)luaL_checkudata(L, 1, "mimas.Slider");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.Slider: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.Slider: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** int mimas::Slider::borderWidth() const 
 * include/mimas/Slider.h:151
 */
static int Slider_borderWidth(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Slider in borderWidth");
    int  retval__ = self__->borderWidth();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Slider.borderWidth: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Slider.borderWidth: Unknown exception");
  }
}


/** void mimas::Slider::callback(double value)
 * include/mimas/Slider.h:180
 */
static int Slider_callback(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Slider in callback");
    double value = luaL_checknumber(L, 2);
    self__->callback(value);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Slider.callback: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Slider.callback: Unknown exception");
  }
}


/** QString mimas::Slider::cssClass() const 
 * include/mimas/Slider.h:92
 */
static int Slider_cssClass(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Slider in cssClass");
    QString  retval__ = self__->cssClass();
    lua_pushclass<QString>(L, retval__, "mimas.QString");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Slider.cssClass: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Slider.cssClass: Unknown exception");
  }
}


/** float mimas::Slider::hue()
 * include/mimas/Slider.h:134
 */
static int Slider_hue(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Slider in hue");
    float  retval__ = self__->hue();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Slider.hue: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Slider.hue: Unknown exception");
  }
}


/** void mimas::Slider::move(int x, int y)
 * include/mimas/Slider.h:117
 */
static int Slider_move(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Slider in move");
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    self__->move(x, y);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Slider.move: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Slider.move: Unknown exception");
  }
}


/** LuaStackSize mimas::Slider::name(lua_State *L)
 * include/mimas/Slider.h:106
 */
static int Slider_name(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Slider in name");
    
    LuaStackSize  retval__ = self__->name(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Slider.name: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Slider.name: Unknown exception");
  }
}


/** QObject* mimas::Slider::object()
 * include/mimas/Slider.h:100
 */
static int Slider_object(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Slider in object");
    QObject * retval__ = self__->object();
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Slider.object: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Slider.object: Unknown exception");
  }
}


/** void mimas::Slider::resize(int w, int h)
 * include/mimas/Slider.h:121
 */
static int Slider_resize(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Slider in resize");
    int w = luaL_checkint(L, 2);
    int h = luaL_checkint(L, 3);
    self__->resize(w, h);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Slider.resize: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Slider.resize: Unknown exception");
  }
}


/** void mimas::Slider::setBorderWidth(int width)
 * include/mimas/Slider.h:147
 */
static int Slider_setBorderWidth(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Slider in setBorderWidth");
    int width = luaL_checkint(L, 2);
    self__->setBorderWidth(width);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Slider.setBorderWidth: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Slider.setBorderWidth: Unknown exception");
  }
}


/** void mimas::Slider::setHue(float hue)
 * include/mimas/Slider.h:129
 */
static int Slider_setHue(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Slider in setHue");
    float hue = luaL_checknumber(L, 2);
    self__->setHue(hue);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Slider.setHue: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Slider.setHue: Unknown exception");
  }
}


/** void mimas::Slider::setName(const char *name)
 * include/mimas/Slider.h:113
 */
static int Slider_setName(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Slider in setName");
    const char *name = luaL_checkstring(L, 2);
    self__->setName(name);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Slider.setName: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Slider.setName: Unknown exception");
  }
}


/** void mimas::Slider::setStyle(const char *text)
 * include/mimas/Slider.h:125
 */
static int Slider_setStyle(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Slider in setStyle");
    const char *text = luaL_checkstring(L, 2);
    self__->setStyle(text);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Slider.setStyle: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Slider.setStyle: Unknown exception");
  }
}


/** void mimas::Slider::setValue(double remote_value)
 * include/mimas/Slider.h:171
 */
static int Slider_setValue(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Slider in setValue");
    double remote_value = luaL_checknumber(L, 2);
    self__->setValue(remote_value);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Slider.setValue: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Slider.setValue: Unknown exception");
  }
}


/** void mimas::Slider::set_callback(lua_State *L)
 * include/mimas/Slider.h:159
 */
static int Slider_set_callback(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Slider in set_callback");
    
    self__->set_callback(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Slider.set_callback: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Slider.set_callback: Unknown exception");
  }
}


/** virtual QSize mimas::Slider::sizeHint() const 
 * include/mimas/Slider.h:139
 */
static int Slider_sizeHint(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Slider in sizeHint");
    QSize  retval__ = self__->sizeHint();
    lua_pushclass<QSize>(L, retval__, "mimas.QSize");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Slider.sizeHint: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Slider.sizeHint: Unknown exception");
  }
}


/** QWidget* mimas::Slider::widget()
 * include/mimas/Slider.h:96
 */
static int Slider_widget(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Slider in widget");
    QWidget * retval__ = self__->widget();
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Slider.widget: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Slider.widget: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Slider_member_methods[] = {
  {"borderWidth"       , Slider_borderWidth},
  {"callback"          , Slider_callback},
  {"cssClass"          , Slider_cssClass},
  {"hue"               , Slider_hue},
  {"move"              , Slider_move},
  {"name"              , Slider_name},
  {"object"            , Slider_object},
  {"resize"            , Slider_resize},
  {"setBorderWidth"    , Slider_setBorderWidth},
  {"setHue"            , Slider_setHue},
  {"setName"           , Slider_setName},
  {"setStyle"          , Slider_setStyle},
  {"setValue"          , Slider_setValue},
  {"set_callback"      , Slider_set_callback},
  {"sizeHint"          , Slider_sizeHint},
  {"widget"            , Slider_widget},
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
