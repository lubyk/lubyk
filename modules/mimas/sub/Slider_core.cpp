#include "mimas/Slider.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Slider::Slider(rubyk::Worker *worker, int type=(int) VerticalSliderType, QWidget *parent=0)
 * include/mimas/Slider.h:71
 */
static int Slider_Slider(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Slider * retval__;
    rubyk::Worker *worker = *((rubyk::Worker **)luaL_checkudata(L, 1, "rubyk.Worker"));
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
    lua_pushclass<Slider>(L, retval__, "mimas.Slider");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.Slider: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.Slider: Unknown exception");
    lua_error(L);
    return 0;
  }
}

/* ============================ Destructor       ====================== */

static int Slider_destructor(lua_State *L) {
  Slider **userdata = (Slider**)luaL_checkudata(L, 1, "mimas.Slider");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Slider__tostring(lua_State *L) {
  Slider **userdata = (Slider**)luaL_checkudata(L, 1, "mimas.Slider");
  
  lua_pushfstring(L, "<mimas.Slider: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** int mimas::Slider::borderWidth() const 
 * include/mimas/Slider.h:135
 */
static int Slider_borderWidth(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    int  retval__ = self__->borderWidth();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.borderWidth: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.borderWidth: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Slider::callback(double value)
 * include/mimas/Slider.h:162
 */
static int Slider_callback(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    double value = luaL_checknumber(L, 2);
    self__->callback(value);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.callback: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.callback: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** QString mimas::Slider::cssClass() const 
 * include/mimas/Slider.h:76
 */
static int Slider_cssClass(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    QString  retval__ = self__->cssClass();
    lua_pushclass<QString>(L, retval__, "mimas.QString");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.cssClass: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.cssClass: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** float mimas::Slider::hue()
 * include/mimas/Slider.h:117
 */
static int Slider_hue(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    float  retval__ = self__->hue();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.hue: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.hue: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Slider::move(int x, int y)
 * include/mimas/Slider.h:101
 */
static int Slider_move(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    self__->move(x, y);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.move: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.move: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** LuaStackSize mimas::Slider::name(lua_State *L)
 * include/mimas/Slider.h:90
 */
static int Slider_name(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    
    LuaStackSize  retval__ = self__->name(L);
    return retval__;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.name: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.name: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** QObject* mimas::Slider::object()
 * include/mimas/Slider.h:84
 */
static int Slider_object(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    QObject * retval__ = self__->object();
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.object: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.object: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Slider::resize(int w, int h)
 * include/mimas/Slider.h:105
 */
static int Slider_resize(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    int w = luaL_checkint(L, 2);
    int h = luaL_checkint(L, 3);
    self__->resize(w, h);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.resize: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.resize: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Slider::setBorderWidth(int width)
 * include/mimas/Slider.h:131
 */
static int Slider_setBorderWidth(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    int width = luaL_checkint(L, 2);
    self__->setBorderWidth(width);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.setBorderWidth: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.setBorderWidth: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Slider::setHue(float hue)
 * include/mimas/Slider.h:113
 */
static int Slider_setHue(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    float hue = luaL_checknumber(L, 2);
    self__->setHue(hue);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.setHue: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.setHue: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Slider::setName(const char *name)
 * include/mimas/Slider.h:97
 */
static int Slider_setName(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    const char *name = luaL_checkstring(L, 2);
    self__->setName(name);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.setName: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.setName: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Slider::setStyle(const char *text)
 * include/mimas/Slider.h:109
 */
static int Slider_setStyle(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    const char *text = luaL_checkstring(L, 2);
    self__->setStyle(text);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.setStyle: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.setStyle: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Slider::setValue(double remote_value)
 * include/mimas/Slider.h:153
 */
static int Slider_setValue(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    double remote_value = luaL_checknumber(L, 2);
    self__->setValue(remote_value);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.setValue: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.setValue: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Slider::set_callback(lua_State *L)
 * include/mimas/Slider.h:142
 */
static int Slider_set_callback(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    
    self__->set_callback(L);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.set_callback: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.set_callback: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** virtual QSize mimas::Slider::sizeHint() const 
 * include/mimas/Slider.h:123
 */
static int Slider_sizeHint(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    QSize  retval__ = self__->sizeHint();
    lua_pushclass<QSize>(L, retval__, "mimas.QSize");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.sizeHint: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.sizeHint: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** QWidget* mimas::Slider::widget()
 * include/mimas/Slider.h:80
 */
static int Slider_widget(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    QWidget * retval__ = self__->widget();
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.widget: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.widget: Unknown exception");
    lua_error(L);
    return 0;
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

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Slider_namespace_methods);


  // register class enums
  register_constants(L, "mimas.Slider_const", Slider_namespace_constants);

	return 1;
}
