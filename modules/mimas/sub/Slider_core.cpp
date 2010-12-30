#include "mimas/Slider.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Slider::Slider(int type=(int) VerticalSliderType, QWidget *parent=0)
 * include/mimas/Slider.h:68
 */
static int Slider_Slider(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Slider * retval__;
    if (top__ < 1) {
      retval__ = new Slider();
    } else {
      int type = luaL_checkint(L, 1);
      if (top__ < 2) {
        retval__ = new Slider(type);
      } else {
        QWidget *parent = *((QWidget **)luaL_checkudata(L, 2, "mimas.QWidget"));
        retval__ = new Slider(type, parent);
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


/** QObject* mimas::Slider::object()
 * include/mimas/Slider.h:76
 */
static int Slider_object(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    lua_remove(L, 1);
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


/** void mimas::Slider::setHue(int hue)
 * include/mimas/Slider.h:80
 */
static int Slider_setHue(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    lua_remove(L, 1);
    int hue = luaL_checkint(L, 1);
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


/** void mimas::Slider::setValue(double remote_value)
 * include/mimas/Slider.h:96
 */
static int Slider_setValue(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    lua_remove(L, 1);
    double remote_value = luaL_checknumber(L, 1);
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


/** virtual QSize mimas::Slider::sizeHint() const 
 * include/mimas/Slider.h:84
 */
static int Slider_sizeHint(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    lua_remove(L, 1);
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
 * include/mimas/Slider.h:72
 */
static int Slider_widget(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    lua_remove(L, 1);
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
  {"object"            , Slider_object},
  {"setHue"            , Slider_setHue},
  {"setValue"          , Slider_setValue},
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
