#include "mimas/Widget.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Widget::Widget(lubyk::Worker *worker)
 * include/mimas/Widget.h:68
 */
static int Widget_Widget(lua_State *L) {
  try {
    lubyk::Worker *worker = *((lubyk::Worker **)luaL_checkudata(L, 1, "lubyk.Worker"));
    Widget * retval__ = new Widget(worker);
    lua_pushclass2<Widget>(L, retval__, "mimas.Widget");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.Widget: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.Widget: Unknown exception");
  }
}


/* ============================ Destructor       ====================== */

static int Widget_destructor(lua_State *L) {
  Widget **userdata = (Widget**)luaL_checkudata(L, 1, "mimas.Widget");

  
  // custom destructor
  if (*userdata) (*userdata)->dub_destroy();
  
  *userdata = NULL;
  return 0;
}




// test if class is deleted
static int Widget_deleted(lua_State *L) {
  Widget **userdata = (Widget**)luaL_checkudata(L, 1, "mimas.Widget");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Widget__tostring(lua_State *L) {
  Widget **userdata = (Widget**)luaL_checkudata(L, 1, "mimas.Widget");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.Widget: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.Widget: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::Widget::__newindex(lua_State *L)
 * include/mimas/Widget.h:210
 */
static int Widget___newindex(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in __newindex");
    
    self__->__newindex(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.__newindex: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.__newindex: Unknown exception");
  }
}


/** void mimas::Widget::activateWindow()
 * include/mimas/Widget.h:187
 */
static int Widget_activateWindow(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in activateWindow");
    self__->activateWindow();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.activateWindow: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.activateWindow: Unknown exception");
  }
}


/** void mimas::Widget::addWidget(QWidget *widget, float x=0, float y=0)
 * include/mimas/Widget.h:86
 */
static int Widget_addWidget(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in addWidget");
    int top__ = lua_gettop(L);
    QWidget *widget = *((QWidget **)luaL_checkudata(L, 2, "mimas.QWidget"));
    if (top__ < 3) {
      self__->addWidget(widget);
    } else {
      float x = luaL_checknumber(L, 3);
      if (top__ < 4) {
        self__->addWidget(widget, x);
      } else {
        float y = luaL_checknumber(L, 4);
        self__->addWidget(widget, x, y);
      }
    }
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.addWidget: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.addWidget: Unknown exception");
  }
}


/** bool mimas::Widget::close()
 * include/mimas/Widget.h:173
 */
static int Widget_close(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in close");
    bool  retval__ = self__->close();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.close: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.close: Unknown exception");
  }
}


/** QString mimas::Widget::cssClass() const 
 * include/mimas/Widget.h:78
 */
static int Widget_cssClass(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in cssClass");
    QString  retval__ = self__->cssClass();
    lua_pushclass<QString>(L, retval__, "mimas.QString");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.cssClass: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.cssClass: Unknown exception");
  }
}


/** void mimas::Widget::globalMove(float x, float y)
 * include/mimas/Widget.h:203
 */
static int Widget_globalMove(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in globalMove");
    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    self__->globalMove(x, y);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.globalMove: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.globalMove: Unknown exception");
  }
}


/** LuaStackSize mimas::Widget::globalPosition(lua_State *L)
 * include/mimas/Widget.h:194
 */
static int Widget_globalPosition(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in globalPosition");
    
    LuaStackSize  retval__ = self__->globalPosition(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.globalPosition: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.globalPosition: Unknown exception");
  }
}


/** float mimas::Widget::hue()
 * include/mimas/Widget.h:129
 */
static int Widget_hue(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in hue");
    float  retval__ = self__->hue();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.hue: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.hue: Unknown exception");
  }
}


/** bool mimas::Widget::isVisible() const 
 * include/mimas/Widget.h:178
 */
static int Widget_isVisible(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in isVisible");
    bool  retval__ = self__->isVisible();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.isVisible: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.isVisible: Unknown exception");
  }
}


/** void mimas::Widget::move(int x, int y)
 * include/mimas/Widget.h:110
 */
static int Widget_move(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in move");
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    self__->move(x, y);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.move: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.move: Unknown exception");
  }
}


/** LuaStackSize mimas::Widget::name(lua_State *L)
 * include/mimas/Widget.h:99
 */
static int Widget_name(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in name");
    
    LuaStackSize  retval__ = self__->name(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.name: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.name: Unknown exception");
  }
}


/** QObject* mimas::Widget::object()
 * include/mimas/Widget.h:93
 */
static int Widget_object(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in object");
    QObject * retval__ = self__->object();
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.object: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.object: Unknown exception");
  }
}


/** void mimas::Widget::resize(int w, int h)
 * include/mimas/Widget.h:115
 */
static int Widget_resize(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in resize");
    int w = luaL_checkint(L, 2);
    int h = luaL_checkint(L, 3);
    self__->resize(w, h);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.resize: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.resize: Unknown exception");
  }
}


/** void mimas::Widget::setHue(float hue)
 * include/mimas/Widget.h:124
 */
static int Widget_setHue(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in setHue");
    float hue = luaL_checknumber(L, 2);
    self__->setHue(hue);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.setHue: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.setHue: Unknown exception");
  }
}


/** void mimas::Widget::setMinimumSize(float w, float h)
 * include/mimas/Widget.h:164
 */
static int Widget_setMinimumSize(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in setMinimumSize");
    float w = luaL_checknumber(L, 2);
    float h = luaL_checknumber(L, 3);
    self__->setMinimumSize(w, h);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.setMinimumSize: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.setMinimumSize: Unknown exception");
  }
}


/** void mimas::Widget::setName(const char *name)
 * include/mimas/Widget.h:106
 */
static int Widget_setName(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in setName");
    const char *name = luaL_checkstring(L, 2);
    self__->setName(name);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.setName: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.setName: Unknown exception");
  }
}


/** void mimas::Widget::setSizeHint(float w, float h)
 * include/mimas/Widget.h:149
 */
static int Widget_setSizeHint(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in setSizeHint");
    float w = luaL_checknumber(L, 2);
    float h = luaL_checknumber(L, 3);
    self__->setSizeHint(w, h);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.setSizeHint: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.setSizeHint: Unknown exception");
  }
}


/** void mimas::Widget::setSizePolicy(int horizontal, int vertical)
 * include/mimas/Widget.h:157
 */
static int Widget_setSizePolicy(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in setSizePolicy");
    int horizontal = luaL_checkint(L, 2);
    int vertical = luaL_checkint(L, 3);
    self__->setSizePolicy(horizontal, vertical);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.setSizePolicy: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.setSizePolicy: Unknown exception");
  }
}


/** void mimas::Widget::setStyle(const char *text)
 * include/mimas/Widget.h:120
 */
static int Widget_setStyle(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in setStyle");
    const char *text = luaL_checkstring(L, 2);
    self__->setStyle(text);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.setStyle: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.setStyle: Unknown exception");
  }
}


/** void mimas::Widget::show()
 * include/mimas/Widget.h:182
 */
static int Widget_show(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in show");
    self__->show();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.show: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.show: Unknown exception");
  }
}


/** LuaStackSize mimas::Widget::textSize(const char *text, lua_State *L)
 * include/mimas/Widget.h:140
 */
static int Widget_textSize(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in textSize");
    const char *text = luaL_checkstring(L, 2);
    
    LuaStackSize  retval__ = self__->textSize(text, L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.textSize: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.textSize: Unknown exception");
  }
}


/** void mimas::Widget::update()
 * include/mimas/Widget.h:133
 */
static int Widget_update(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in update");
    self__->update();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.update: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.update: Unknown exception");
  }
}


/** QWidget* mimas::Widget::widget()
 * include/mimas/Widget.h:82
 */
static int Widget_widget(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in widget");
    QWidget * retval__ = self__->widget();
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.widget: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.widget: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Widget_member_methods[] = {
  {"__newindex"        , Widget___newindex},
  {"activateWindow"    , Widget_activateWindow},
  {"addWidget"         , Widget_addWidget},
  {"close"             , Widget_close},
  {"cssClass"          , Widget_cssClass},
  {"globalMove"        , Widget_globalMove},
  {"globalPosition"    , Widget_globalPosition},
  {"hue"               , Widget_hue},
  {"isVisible"         , Widget_isVisible},
  {"move"              , Widget_move},
  {"name"              , Widget_name},
  {"object"            , Widget_object},
  {"resize"            , Widget_resize},
  {"setHue"            , Widget_setHue},
  {"setMinimumSize"    , Widget_setMinimumSize},
  {"setName"           , Widget_setName},
  {"setSizeHint"       , Widget_setSizeHint},
  {"setSizePolicy"     , Widget_setSizePolicy},
  {"setStyle"          , Widget_setStyle},
  {"show"              , Widget_show},
  {"textSize"          , Widget_textSize},
  {"update"            , Widget_update},
  {"widget"            , Widget_widget},
  {"__tostring"        , Widget__tostring},
  {"__gc"              , Widget_destructor},
  {"deleted"           , Widget_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg Widget_namespace_methods[] = {
  {"Widget"            , Widget_Widget},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Widget(lua_State *L) {
#else
extern "C" int luaopen_mimas_Widget(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Widget");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Widget_member_methods);
  // save meta-table in mimas.Widget_
  register_mt(L, "mimas", "Widget");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Widget_namespace_methods);


	return 1;
}
