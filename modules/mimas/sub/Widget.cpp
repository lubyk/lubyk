#include "mimas/Widget.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Widget::Widget(lubyk::Worker *worker)
 * include/mimas/Widget.h:74
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
 * include/mimas/Widget.h:273
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
 * include/mimas/Widget.h:218
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
 * include/mimas/Widget.h:92
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
 * include/mimas/Widget.h:202
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
 * include/mimas/Widget.h:84
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
 * include/mimas/Widget.h:254
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
 * include/mimas/Widget.h:245
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


/** int mimas::Widget::height()
 * include/mimas/Widget.h:139
 */
static int Widget_height(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in height");
    int  retval__ = self__->height();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.height: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.height: Unknown exception");
  }
}


/** void mimas::Widget::hide()
 * include/mimas/Widget.h:214
 */
static int Widget_hide(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in hide");
    self__->hide();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.hide: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.hide: Unknown exception");
  }
}


/** float mimas::Widget::hue()
 * include/mimas/Widget.h:156
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


/** bool mimas::Widget::isFullScreen()
 * include/mimas/Widget.h:232
 */
static int Widget_isFullScreen(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in isFullScreen");
    bool  retval__ = self__->isFullScreen();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.isFullScreen: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.isFullScreen: Unknown exception");
  }
}


/** bool mimas::Widget::isVisible() const 
 * include/mimas/Widget.h:206
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


/** void mimas::Widget::lower()
 * include/mimas/Widget.h:260
 */
static int Widget_lower(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in lower");
    self__->lower();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.lower: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.lower: Unknown exception");
  }
}


/** void mimas::Widget::move(int x, int y)
 * include/mimas/Widget.h:119
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
 * include/mimas/Widget.h:108
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
 * include/mimas/Widget.h:102
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


/** void mimas::Widget::raise()
 * include/mimas/Widget.h:266
 */
static int Widget_raise(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in raise");
    self__->raise();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.raise: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.raise: Unknown exception");
  }
}


/** void mimas::Widget::resize(int w, int h)
 * include/mimas/Widget.h:123
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
 * include/mimas/Widget.h:151
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
 * include/mimas/Widget.h:188
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


/** void mimas::Widget::setMouseTracking(bool enable)
 * include/mimas/Widget.h:194
 */
static int Widget_setMouseTracking(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in setMouseTracking");
    bool enable = lua_toboolean(L, 2);
    self__->setMouseTracking(enable);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.setMouseTracking: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.setMouseTracking: Unknown exception");
  }
}


/** void mimas::Widget::setName(const char *name)
 * include/mimas/Widget.h:115
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


/** void mimas::Widget::setParent(QWidget *parent)
 * include/mimas/Widget.h:98
 */
static int Widget_setParent(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in setParent");
    QWidget *parent = *((QWidget **)luaL_checkudata(L, 2, "mimas.QWidget"));
    self__->setParent(parent);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.setParent: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.setParent: Unknown exception");
  }
}


/** void mimas::Widget::setSizeHint(float w, float h)
 * include/mimas/Widget.h:175
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
 * include/mimas/Widget.h:182
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
 * include/mimas/Widget.h:143
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


/** void mimas::Widget::setStyleSheet(const char *text)
 * include/mimas/Widget.h:147
 */
static int Widget_setStyleSheet(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in setStyleSheet");
    const char *text = luaL_checkstring(L, 2);
    self__->setStyleSheet(text);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.setStyleSheet: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.setStyleSheet: Unknown exception");
  }
}


/** void mimas::Widget::show()
 * include/mimas/Widget.h:210
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


/** void mimas::Widget::showFullScreen(bool enable=true)
 * include/mimas/Widget.h:224
 */
static int Widget_showFullScreen(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in showFullScreen");
    int top__ = lua_gettop(L);
    if (top__ < 2) {
      self__->showFullScreen();
    } else {
      bool enable = lua_toboolean(L, 2);
      self__->showFullScreen(enable);
    }
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.showFullScreen: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.showFullScreen: Unknown exception");
  }
}


/** void mimas::Widget::swapFullScreen()
 * include/mimas/Widget.h:238
 */
static int Widget_swapFullScreen(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in swapFullScreen");
    self__->swapFullScreen();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.swapFullScreen: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.swapFullScreen: Unknown exception");
  }
}


/** LuaStackSize mimas::Widget::textSize(const char *text, lua_State *L)
 * include/mimas/Widget.h:166
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
 * include/mimas/Widget.h:160
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
 * include/mimas/Widget.h:88
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


/** int mimas::Widget::width()
 * include/mimas/Widget.h:135
 */
static int Widget_width(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in width");
    int  retval__ = self__->width();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.width: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.width: Unknown exception");
  }
}


/** int mimas::Widget::x()
 * include/mimas/Widget.h:127
 */
static int Widget_x(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in x");
    int  retval__ = self__->x();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.x: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.x: Unknown exception");
  }
}


/** int mimas::Widget::y()
 * include/mimas/Widget.h:131
 */
static int Widget_y(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Widget in y");
    int  retval__ = self__->y();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Widget.y: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Widget.y: Unknown exception");
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
  {"height"            , Widget_height},
  {"hide"              , Widget_hide},
  {"hue"               , Widget_hue},
  {"isFullScreen"      , Widget_isFullScreen},
  {"isVisible"         , Widget_isVisible},
  {"lower"             , Widget_lower},
  {"move"              , Widget_move},
  {"name"              , Widget_name},
  {"object"            , Widget_object},
  {"raise"             , Widget_raise},
  {"resize"            , Widget_resize},
  {"setHue"            , Widget_setHue},
  {"setMinimumSize"    , Widget_setMinimumSize},
  {"setMouseTracking"  , Widget_setMouseTracking},
  {"setName"           , Widget_setName},
  {"setParent"         , Widget_setParent},
  {"setSizeHint"       , Widget_setSizeHint},
  {"setSizePolicy"     , Widget_setSizePolicy},
  {"setStyle"          , Widget_setStyle},
  {"setStyleSheet"     , Widget_setStyleSheet},
  {"show"              , Widget_show},
  {"showFullScreen"    , Widget_showFullScreen},
  {"swapFullScreen"    , Widget_swapFullScreen},
  {"textSize"          , Widget_textSize},
  {"update"            , Widget_update},
  {"widget"            , Widget_widget},
  {"width"             , Widget_width},
  {"x"                 , Widget_x},
  {"y"                 , Widget_y},
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
