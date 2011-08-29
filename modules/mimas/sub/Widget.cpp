#include "mimas/Widget.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Widget::Widget(int window_flags=0)
 * include/mimas/Widget.h:62
 */
static int Widget_Widget(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Widget * retval__;
    if (top__ < 1) {
      retval__ = new Widget();
    } else {
      int window_flags = dubL_checkint(L, 1);
      retval__ = new Widget(window_flags);
    }
    // The class inherits from 'LuaCallback', use lua_init instead of pushclass.
    return retval__->luaInit(L, retval__, "mimas.Widget");
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.Widget: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.Widget: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Widget_destructor(lua_State *L) {
  Widget **userdata = (Widget**)dubL_checksdata_n(L, 1, "mimas.Widget");

  
  // custom destructor
  if (*userdata) (*userdata)->luaDestroy();
  
  *userdata = NULL;
  return 0;
}




// test if class is deleted
static int Widget_deleted(lua_State *L) {
  Widget **userdata = (Widget**)dubL_checksdata_n(L, 1, "mimas.Widget");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Widget__tostring(lua_State *L) {
  Widget **userdata = (Widget**)dubL_checksdata_n(L, 1, "mimas.Widget");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.Widget: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.Widget: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::Widget::activateWindow()
 * include/mimas/Widget.h:215
 */
static int Widget_activateWindow(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in activateWindow");
    self__->activateWindow();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.activateWindow: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.activateWindow: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::addWidget(QWidget *widget, float x=0, float y=0)
 * include/mimas/Widget.h:80
 */
static int Widget_addWidget(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in addWidget");
    int top__ = lua_gettop(L);
    QWidget *widget = *((QWidget **)dubL_checksdata(L, 2, "mimas.QWidget"));
    if (top__ < 3) {
      self__->addWidget(widget);
    } else {
      float x = dubL_checknumber(L, 3);
      if (top__ < 4) {
        self__->addWidget(widget, x);
      } else {
        float y = dubL_checknumber(L, 4);
        self__->addWidget(widget, x, y);
      }
    }
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.addWidget: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.addWidget: Unknown exception");
  }
  return lua_error(L);
}



/** bool mimas::Widget::close()
 * include/mimas/Widget.h:199
 */
static int Widget_close(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in close");
    bool  retval__ = self__->close();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.close: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.close: Unknown exception");
  }
  return lua_error(L);
}



/** QString mimas::Widget::cssClass() const 
 * include/mimas/Widget.h:72
 */
static int Widget_cssClass(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in cssClass");
    QString  retval__ = self__->cssClass();
    lua_pushclass<QString>(L, retval__, "mimas.QString");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.cssClass: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.cssClass: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::Widget::getExistingDirectory(const char *caption, const char *base_dir, int options, lua_State *L)
 * include/mimas/Widget.h:277
 */
static int Widget_getExistingDirectory(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in getExistingDirectory");
    const char *caption = dubL_checkstring(L, 2);
    const char *base_dir = dubL_checkstring(L, 3);
    int options = dubL_checkint(L, 4);
    
    LuaStackSize  retval__ = self__->getExistingDirectory(caption, base_dir, options, L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.getExistingDirectory: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.getExistingDirectory: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::Widget::getOpenFileName(const char *caption, const char *base_dir, const char *filter, int options, lua_State *L)
 * include/mimas/Widget.h:272
 */
static int Widget_getOpenFileName(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in getOpenFileName");
    const char *caption = dubL_checkstring(L, 2);
    const char *base_dir = dubL_checkstring(L, 3);
    const char *filter = dubL_checkstring(L, 4);
    int options = dubL_checkint(L, 5);
    
    LuaStackSize  retval__ = self__->getOpenFileName(caption, base_dir, filter, options, L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.getOpenFileName: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.getOpenFileName: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::globalMove(float x, float y)
 * include/mimas/Widget.h:251
 */
static int Widget_globalMove(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in globalMove");
    float x = dubL_checknumber(L, 2);
    float y = dubL_checknumber(L, 3);
    self__->globalMove(x, y);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.globalMove: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.globalMove: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::Widget::globalPosition(lua_State *L)
 * include/mimas/Widget.h:242
 */
static int Widget_globalPosition(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in globalPosition");
    
    LuaStackSize  retval__ = self__->globalPosition(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.globalPosition: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.globalPosition: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::Widget::height()
 * include/mimas/Widget.h:127
 */
static int Widget_height(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in height");
    int  retval__ = self__->height();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.height: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.height: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::hide()
 * include/mimas/Widget.h:211
 */
static int Widget_hide(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in hide");
    self__->hide();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.hide: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.hide: Unknown exception");
  }
  return lua_error(L);
}



/** float mimas::Widget::hue()
 * include/mimas/Widget.h:153
 */
static int Widget_hue(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in hue");
    float  retval__ = self__->hue();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.hue: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.hue: Unknown exception");
  }
  return lua_error(L);
}



/** bool mimas::Widget::isFullScreen()
 * include/mimas/Widget.h:229
 */
static int Widget_isFullScreen(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in isFullScreen");
    bool  retval__ = self__->isFullScreen();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.isFullScreen: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.isFullScreen: Unknown exception");
  }
  return lua_error(L);
}



/** bool mimas::Widget::isVisible() const 
 * include/mimas/Widget.h:203
 */
static int Widget_isVisible(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in isVisible");
    bool  retval__ = self__->isVisible();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.isVisible: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.isVisible: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::lower()
 * include/mimas/Widget.h:257
 */
static int Widget_lower(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in lower");
    self__->lower();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.lower: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.lower: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::move(int x, int y)
 * include/mimas/Widget.h:107
 */
static int Widget_move(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in move");
    int x = dubL_checkint(L, 2);
    int y = dubL_checkint(L, 3);
    self__->move(x, y);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.move: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.move: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::Widget::name(lua_State *L)
 * include/mimas/Widget.h:96
 */
static int Widget_name(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in name");
    
    LuaStackSize  retval__ = self__->name(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.name: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.name: Unknown exception");
  }
  return lua_error(L);
}



/** QObject* mimas::Widget::object()
 * include/mimas/Widget.h:90
 */
static int Widget_object(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in object");
    QObject * retval__ = self__->object();
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.object: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.object: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::raise()
 * include/mimas/Widget.h:263
 */
static int Widget_raise(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in raise");
    self__->raise();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.raise: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.raise: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::resize(int w, int h)
 * include/mimas/Widget.h:111
 */
static int Widget_resize(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in resize");
    int w = dubL_checkint(L, 2);
    int h = dubL_checkint(L, 3);
    self__->resize(w, h);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.resize: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.resize: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::setHue(float hue)
 * include/mimas/Widget.h:148
 */
static int Widget_setHue(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in setHue");
    float hue = dubL_checknumber(L, 2);
    self__->setHue(hue);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.setHue: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.setHue: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::setMinimumSize(float w, float h)
 * include/mimas/Widget.h:185
 */
static int Widget_setMinimumSize(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in setMinimumSize");
    float w = dubL_checknumber(L, 2);
    float h = dubL_checknumber(L, 3);
    self__->setMinimumSize(w, h);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.setMinimumSize: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.setMinimumSize: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::setMouseTracking(bool enable)
 * include/mimas/Widget.h:191
 */
static int Widget_setMouseTracking(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in setMouseTracking");
    bool enable = lua_toboolean(L, 2);
    self__->setMouseTracking(enable);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.setMouseTracking: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.setMouseTracking: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::setName(const char *name)
 * include/mimas/Widget.h:103
 */
static int Widget_setName(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in setName");
    const char *name = dubL_checkstring(L, 2);
    self__->setName(name);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.setName: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.setName: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::setParent(QWidget *parent)
 * include/mimas/Widget.h:86
 */
static int Widget_setParent(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in setParent");
    QWidget *parent = *((QWidget **)dubL_checksdata(L, 2, "mimas.QWidget"));
    self__->setParent(parent);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.setParent: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.setParent: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::setSizeHint(float w, float h)
 * include/mimas/Widget.h:172
 */
static int Widget_setSizeHint(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in setSizeHint");
    float w = dubL_checknumber(L, 2);
    float h = dubL_checknumber(L, 3);
    self__->setSizeHint(w, h);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.setSizeHint: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.setSizeHint: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::setSizePolicy(int horizontal, int vertical)
 * include/mimas/Widget.h:179
 */
static int Widget_setSizePolicy(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in setSizePolicy");
    int horizontal = dubL_checkint(L, 2);
    int vertical = dubL_checkint(L, 3);
    self__->setSizePolicy(horizontal, vertical);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.setSizePolicy: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.setSizePolicy: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::setStyle(const char *text)
 * include/mimas/Widget.h:140
 */
static int Widget_setStyle(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in setStyle");
    const char *text = dubL_checkstring(L, 2);
    self__->setStyle(text);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.setStyle: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.setStyle: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::setStyleSheet(const char *text)
 * include/mimas/Widget.h:144
 */
static int Widget_setStyleSheet(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in setStyleSheet");
    const char *text = dubL_checkstring(L, 2);
    self__->setStyleSheet(text);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.setStyleSheet: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.setStyleSheet: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::show()
 * include/mimas/Widget.h:207
 */
static int Widget_show(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in show");
    self__->show();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.show: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.show: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::showFullScreen(bool enable=true)
 * include/mimas/Widget.h:221
 */
static int Widget_showFullScreen(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in showFullScreen");
    int top__ = lua_gettop(L);
    if (top__ < 2) {
      self__->showFullScreen();
    } else {
      bool enable = lua_toboolean(L, 2);
      self__->showFullScreen(enable);
    }
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.showFullScreen: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.showFullScreen: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::Widget::size(lua_State *L)
 * include/mimas/Widget.h:133
 */
static int Widget_size(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in size");
    
    LuaStackSize  retval__ = self__->size(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.size: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.size: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::swapFullScreen()
 * include/mimas/Widget.h:235
 */
static int Widget_swapFullScreen(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in swapFullScreen");
    self__->swapFullScreen();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.swapFullScreen: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.swapFullScreen: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::Widget::textSize(const char *text, lua_State *L)
 * include/mimas/Widget.h:163
 */
static int Widget_textSize(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in textSize");
    const char *text = dubL_checkstring(L, 2);
    
    LuaStackSize  retval__ = self__->textSize(text, L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.textSize: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.textSize: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::update()
 * include/mimas/Widget.h:157
 */
static int Widget_update(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in update");
    self__->update();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.update: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.update: Unknown exception");
  }
  return lua_error(L);
}



/** QWidget* mimas::Widget::widget()
 * include/mimas/Widget.h:76
 */
static int Widget_widget(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in widget");
    QWidget * retval__ = self__->widget();
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.widget: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.widget: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::Widget::width()
 * include/mimas/Widget.h:123
 */
static int Widget_width(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in width");
    int  retval__ = self__->width();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.width: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.width: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::Widget::x()
 * include/mimas/Widget.h:115
 */
static int Widget_x(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in x");
    int  retval__ = self__->x();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.x: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.x: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::Widget::y()
 * include/mimas/Widget.h:119
 */
static int Widget_y(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self__) throw dub::Exception("Using deleted mimas.Widget in y");
    int  retval__ = self__->y();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Widget.y: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Widget.y: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Widget_member_methods[] = {
  {"activateWindow"    , Widget_activateWindow},
  {"addWidget"         , Widget_addWidget},
  {"close"             , Widget_close},
  {"cssClass"          , Widget_cssClass},
  {"getExistingDirectory", Widget_getExistingDirectory},
  {"getOpenFileName"   , Widget_getOpenFileName},
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
  {"size"              , Widget_size},
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
