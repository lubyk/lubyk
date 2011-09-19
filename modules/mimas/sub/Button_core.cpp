#include "mimas/Button.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Button::Button(const char *title=NULL, QWidget *parent=NULL)
 * include/mimas/Button.h:57
 */
static int Button_Button(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Button * retval__;
    if (top__ < 1) {
      retval__ = new Button();
    } else {
      const char *title = dubL_checkstring(L, 1);
      if (top__ < 2) {
        retval__ = new Button(title);
      } else {
        QWidget *parent = *((QWidget **)dubL_checksdata(L, 2, "mimas.QWidget"));
        retval__ = new Button(title, parent);
      }
    }
    return retval__->luaInit(L, retval__, "mimas.Button");
  } catch (std::exception &e) {
    lua_pushfstring(L, "Button: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Button: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Button_destructor(lua_State *L) {
  Button **userdata = (Button**)dubL_checksdata_n(L, 1, "mimas.Button");

  
  // custom destructor
  Button *self = *userdata;
  if (self) self->luaDestroy();
  
  *userdata = NULL;
  return 0;
}



// test if class is deleted
static int Button_deleted(lua_State *L) {
  Button **userdata = (Button**)dubL_checksdata_n(L, 1, "mimas.Button");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Button__tostring(lua_State *L) {
  Button **userdata = (Button**)dubL_checksdata_n(L, 1, "mimas.Button");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.Button: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.Button: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** QString mimas::Button::cssClass() const 
 * include/mimas/Button.h:65
 */
static int Button_cssClass(lua_State *L) {
  try {
    Button *self = *((Button**)dubL_checksdata(L, 1, "mimas.Button"));
    if (!self) throw dub::Exception("Using deleted mimas.Button in cssClass");
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



/** int mimas::Button::height()
 * include/mimas/Button.h:110
 */
static int Button_height(lua_State *L) {
  try {
    Button *self = *((Button**)dubL_checksdata(L, 1, "mimas.Button"));
    if (!self) throw dub::Exception("Using deleted mimas.Button in height");
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



/** float mimas::Button::hue()
 * include/mimas/Button.h:127
 */
static int Button_hue(lua_State *L) {
  try {
    Button *self = *((Button**)dubL_checksdata(L, 1, "mimas.Button"));
    if (!self) throw dub::Exception("Using deleted mimas.Button in hue");
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



/** void mimas::Button::move(int x, int y)
 * include/mimas/Button.h:90
 */
static int Button_move(lua_State *L) {
  try {
    Button *self = *((Button**)dubL_checksdata(L, 1, "mimas.Button"));
    if (!self) throw dub::Exception("Using deleted mimas.Button in move");
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



/** LuaStackSize mimas::Button::name(lua_State *L)
 * include/mimas/Button.h:79
 */
static int Button_name(lua_State *L) {
  try {
    Button *self = *((Button**)dubL_checksdata(L, 1, "mimas.Button"));
    if (!self) throw dub::Exception("Using deleted mimas.Button in name");
    
    LuaStackSize  retval__ = self->name(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "name: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "name: Unknown exception");
  }
  return lua_error(L);
}



/** QObject* mimas::Button::object()
 * include/mimas/Button.h:73
 */
static int Button_object(lua_State *L) {
  try {
    Button *self = *((Button**)dubL_checksdata(L, 1, "mimas.Button"));
    if (!self) throw dub::Exception("Using deleted mimas.Button in object");
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



/** void mimas::Button::resize(int w, int h)
 * include/mimas/Button.h:94
 */
static int Button_resize(lua_State *L) {
  try {
    Button *self = *((Button**)dubL_checksdata(L, 1, "mimas.Button"));
    if (!self) throw dub::Exception("Using deleted mimas.Button in resize");
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



/** void mimas::Button::setHue(float hue)
 * include/mimas/Button.h:122
 */
static int Button_setHue(lua_State *L) {
  try {
    Button *self = *((Button**)dubL_checksdata(L, 1, "mimas.Button"));
    if (!self) throw dub::Exception("Using deleted mimas.Button in setHue");
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



/** void mimas::Button::setName(const char *name)
 * include/mimas/Button.h:86
 */
static int Button_setName(lua_State *L) {
  try {
    Button *self = *((Button**)dubL_checksdata(L, 1, "mimas.Button"));
    if (!self) throw dub::Exception("Using deleted mimas.Button in setName");
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



/** void mimas::Button::setStyle(const char *text)
 * include/mimas/Button.h:114
 */
static int Button_setStyle(lua_State *L) {
  try {
    Button *self = *((Button**)dubL_checksdata(L, 1, "mimas.Button"));
    if (!self) throw dub::Exception("Using deleted mimas.Button in setStyle");
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



/** void mimas::Button::setStyleSheet(const char *text)
 * include/mimas/Button.h:118
 */
static int Button_setStyleSheet(lua_State *L) {
  try {
    Button *self = *((Button**)dubL_checksdata(L, 1, "mimas.Button"));
    if (!self) throw dub::Exception("Using deleted mimas.Button in setStyleSheet");
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



/** QWidget* mimas::Button::widget()
 * include/mimas/Button.h:69
 */
static int Button_widget(lua_State *L) {
  try {
    Button *self = *((Button**)dubL_checksdata(L, 1, "mimas.Button"));
    if (!self) throw dub::Exception("Using deleted mimas.Button in widget");
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



/** int mimas::Button::width()
 * include/mimas/Button.h:106
 */
static int Button_width(lua_State *L) {
  try {
    Button *self = *((Button**)dubL_checksdata(L, 1, "mimas.Button"));
    if (!self) throw dub::Exception("Using deleted mimas.Button in width");
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



/** int mimas::Button::x()
 * include/mimas/Button.h:98
 */
static int Button_x(lua_State *L) {
  try {
    Button *self = *((Button**)dubL_checksdata(L, 1, "mimas.Button"));
    if (!self) throw dub::Exception("Using deleted mimas.Button in x");
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



/** int mimas::Button::y()
 * include/mimas/Button.h:102
 */
static int Button_y(lua_State *L) {
  try {
    Button *self = *((Button**)dubL_checksdata(L, 1, "mimas.Button"));
    if (!self) throw dub::Exception("Using deleted mimas.Button in y");
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

static const struct luaL_Reg Button_member_methods[] = {
  {"cssClass"          , Button_cssClass},
  {"height"            , Button_height},
  {"hue"               , Button_hue},
  {"move"              , Button_move},
  {"name"              , Button_name},
  {"object"            , Button_object},
  {"resize"            , Button_resize},
  {"setHue"            , Button_setHue},
  {"setName"           , Button_setName},
  {"setStyle"          , Button_setStyle},
  {"setStyleSheet"     , Button_setStyleSheet},
  {"widget"            , Button_widget},
  {"width"             , Button_width},
  {"x"                 , Button_x},
  {"y"                 , Button_y},
  {"__tostring"        , Button__tostring},
  {"__gc"              , Button_destructor},
  {"deleted"           , Button_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg Button_namespace_methods[] = {
  {"Button"            , Button_Button},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Button_core(lua_State *L) {
#else
extern "C" int luaopen_mimas_Button_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Button");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Button_member_methods);
  // save meta-table in mimas.Button_
  register_mt(L, "mimas", "Button");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Button_namespace_methods);


	return 1;
}
