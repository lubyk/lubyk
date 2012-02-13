#include "mimas/Widget.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */


/** mimas::Widget::Widget(int window_flags)
 * include/mimas/Widget.h:61
 */
static int Widget_Widget1(lua_State *L) {
  try {
    int window_flags = dubL_checkint(L, 1);
    Widget * retval__ = new Widget(window_flags);
    return retval__->luaInit(L, retval__, "mimas.Widget");
  } catch (std::exception &e) {
    lua_pushfstring(L, "Widget: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Widget: Unknown exception");
  }
  return lua_error(L);
}



/** mimas::Widget::Widget(QWidget *parent=0, int window_flags=0)
 * include/mimas/Widget.h:67
 */
static int Widget_Widget2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Widget * retval__;
    if (top__ < 1) {
      retval__ = new Widget();
    } else {
      QWidget *parent = *((QWidget **)dubL_checksdata(L, 1, "mimas.QWidget"));
      if (top__ < 2) {
        retval__ = new Widget(parent);
      } else {
        int window_flags = dubL_checkint(L, 2);
        retval__ = new Widget(parent, window_flags);
      }
    }
    return retval__->luaInit(L, retval__, "mimas.Widget");
  } catch (std::exception &e) {
    lua_pushfstring(L, "Widget: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Widget: Unknown exception");
  }
  return lua_error(L);
}




/** Overloaded function chooser for Widget(...) */
static int Widget_Widget(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TNUMBER) {
    return Widget_Widget1(L);
  } else if (top__ < 1) {
    return Widget_Widget2(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "mimas.QWidget")) {
    return Widget_Widget2(L);
  } else {
    // use any to raise errors
    return Widget_Widget2(L);
  }
}


/* ============================ Destructor       ====================== */

static int Widget_destructor(lua_State *L) {
  Widget **userdata = (Widget**)dubL_checksdata_n(L, 1, "mimas.Widget");

  
  // custom destructor
  Widget *self = *userdata;
  if (self) self->luaDestroy();
  
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


/** void mimas::QWidget::activateWindow()
 * mimas/bind/QWidget.h:33
 */
static int QWidget_activateWindow(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in activateWindow");
    self->activateWindow();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "activateWindow: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "activateWindow: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::addAction(Action *action)
 * mimas/bind/QWidget.h:35
 */
static int QWidget_addAction(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in addAction");
    Action *action = *((Action **)dubL_checksdata(L, 2, "mimas.Action"));
    self->addAction(action);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "addAction: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "addAction: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::addWidget(QWidget *widget)
 * mimas/bind/QWidget.h:45
 */
static int QWidget_addWidget(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in addWidget");
    QWidget *widget = *((QWidget **)dubL_checksdata(L, 2, "mimas.QWidget"));
    widget->setParent(self);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "addWidget: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "addWidget: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::adjustSize()
 * mimas/bind/QWidget.h:21
 */
static int QWidget_adjustSize(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in adjustSize");
    self->adjustSize();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "adjustSize: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "adjustSize: Unknown exception");
  }
  return lua_error(L);
}



/** bool mimas::QWidget::close()
 * mimas/bind/QWidget.h:27
 */
static int QWidget_close(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in close");
    bool  retval__ = self->close();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "close: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "close: Unknown exception");
  }
  return lua_error(L);
}



/** QString mimas::Widget::cssClass() const 
 * include/mimas/Widget.h:76
 */
static int Widget_cssClass(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in cssClass");
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



/** LuaStackSize mimas::Widget::getExistingDirectory(const char *caption, const char *base_dir, int options, lua_State *L)
 * include/mimas/Widget.h:109
 */
static int Widget_getExistingDirectory(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in getExistingDirectory");
    const char *caption = dubL_checkstring(L, 2);
    const char *base_dir = dubL_checkstring(L, 3);
    int options = dubL_checkint(L, 4);
    
    LuaStackSize  retval__ = self->getExistingDirectory(caption, base_dir, options, L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "getExistingDirectory: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "getExistingDirectory: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::Widget::getOpenFileName(const char *caption, const char *base_dir, const char *filter, int options, lua_State *L)
 * include/mimas/Widget.h:106
 */
static int Widget_getOpenFileName(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in getOpenFileName");
    const char *caption = dubL_checkstring(L, 2);
    const char *base_dir = dubL_checkstring(L, 3);
    const char *filter = dubL_checkstring(L, 4);
    int options = dubL_checkint(L, 5);
    
    LuaStackSize  retval__ = self->getOpenFileName(caption, base_dir, filter, options, L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "getOpenFileName: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "getOpenFileName: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::globalMove(float x, float y)
 * mimas/bind/QWidget.h:84
 */
static int QWidget_globalMove(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in globalMove");
    float x = dubL_checknumber(L, 2);
    float y = dubL_checknumber(L, 3);
    self->move(
      self->mapToParent(
        self->mapFromGlobal(QPoint(x, y))
      )
    );
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "globalMove: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "globalMove: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::QWidget::globalPosition()
 * mimas/bind/QWidget.h:75
 */
static int QWidget_globalPosition(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in globalPosition");
    QPoint pt = self->mapToGlobal(QPoint(0, 0));
    lua_pushnumber(L, pt.x());
    lua_pushnumber(L, pt.y());
    return 2;
  } catch (std::exception &e) {
    lua_pushfstring(L, "globalPosition: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "globalPosition: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::QWidget::height()
 * mimas/bind/QWidget.h:18
 */
static int QWidget_height(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in height");
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



/** void mimas::QWidget::hide()
 * mimas/bind/QWidget.h:30
 */
static int QWidget_hide(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in hide");
    self->hide();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "hide: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "hide: Unknown exception");
  }
  return lua_error(L);
}



/** float mimas::Widget::hue()
 * include/mimas/Widget.h:96
 */
static int Widget_hue(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in hue");
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



/** bool mimas::QWidget::isFullScreen()
 * mimas/bind/QWidget.h:34
 */
static int QWidget_isFullScreen(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in isFullScreen");
    bool  retval__ = self->isFullScreen();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "isFullScreen: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "isFullScreen: Unknown exception");
  }
  return lua_error(L);
}



/** bool mimas::QWidget::isVisible()
 * mimas/bind/QWidget.h:28
 */
static int QWidget_isVisible(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in isVisible");
    bool  retval__ = self->isVisible();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "isVisible: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "isVisible: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::lower()
 * mimas/bind/QWidget.h:31
 */
static int QWidget_lower(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in lower");
    self->lower();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "lower: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "lower: Unknown exception");
  }
  return lua_error(L);
}



/** static bool mimas::Widget::mouse(ThreadedLuaObject *obj, QMouseEvent *event)
 * include/mimas/Widget.h:114
 */
static int Widget_mouse(lua_State *L) {
  try {
    ThreadedLuaObject *obj = *((ThreadedLuaObject **)dubL_checksdata(L, 1, "mimas.ThreadedLuaObject"));
    QMouseEvent *event = *((QMouseEvent **)dubL_checksdata(L, 2, "mimas.QMouseEvent"));
    bool  retval__ = Widget::mouse(obj, event);
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mouse: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mouse: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::move(int x, int y)
 * mimas/bind/QWidget.h:13
 */
static int QWidget_move(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in move");
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



/** void mimas::QObject::name()
 * mimas/bind/QObject.h:12
 */
static int QObject_name(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in name");
    lua_pushstring(L, self->objectName().toUtf8().data());
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "name: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "name: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QObject::object()
 * mimas/bind/QObject.h:11
 */
static int QObject_object(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in object");
    QObject * retval__ = self;
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "object: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "object: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::QWidget::position()
 * mimas/bind/QWidget.h:80
 */
static int QWidget_position(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in position");
    lua_pushnumber(L, self->x());
    lua_pushnumber(L, self->y());
    return 2;
  } catch (std::exception &e) {
    lua_pushfstring(L, "position: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "position: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::raise()
 * mimas/bind/QWidget.h:32
 */
static int QWidget_raise(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in raise");
    self->raise();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "raise: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "raise: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::resize(int w, int h)
 * mimas/bind/QWidget.h:14
 */
static int QWidget_resize(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in resize");
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



/** void mimas::Widget::setCssClass(const char *name)
 * include/mimas/Widget.h:84
 */
static int Widget_setCssClass(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in setCssClass");
    const char *name = dubL_checkstring(L, 2);
    self->setCssClass(name);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setCssClass: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setCssClass: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Widget::setHue(float hue)
 * include/mimas/Widget.h:91
 */
static int Widget_setHue(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in setHue");
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



/** void mimas::QWidget::setMinimumSize(float w, float h)
 * mimas/bind/QWidget.h:23
 */
static int QWidget_setMinimumSize(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in setMinimumSize");
    float w = dubL_checknumber(L, 2);
    float h = dubL_checknumber(L, 3);
    self->setMinimumSize(w, h);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setMinimumSize: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setMinimumSize: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::setMouseTracking(bool enable)
 * mimas/bind/QWidget.h:26
 */
static int QWidget_setMouseTracking(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in setMouseTracking");
    bool enable = lua_toboolean(L, 2);
    self->setMouseTracking(enable);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setMouseTracking: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setMouseTracking: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QObject::setName(const char *name)
 * mimas/bind/QObject.h:13
 */
static int QObject_setName(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in setName");
    const char *name = dubL_checkstring(L, 2);
    self->setObjectName(QString(name));
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setName: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setName: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::setParent(QWidget *parent)
 * mimas/bind/QWidget.h:19
 */
static int QWidget_setParent(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in setParent");
    QWidget *parent = *((QWidget **)dubL_checksdata(L, 2, "mimas.QWidget"));
    self->setParent(parent);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setParent: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setParent: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::setSizeHint(float w, float h)
 * mimas/bind/QWidget.h:61
 */
static int QWidget_setSizeHint(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in setSizeHint");
    float w = dubL_checknumber(L, 2);
    float h = dubL_checknumber(L, 3);
    self->size_hint_ = QSize(w, h);
    self->updateGeometry();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setSizeHint: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setSizeHint: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::setSizePolicy(int horizontal, int vertical)
 * mimas/bind/QWidget.h:65
 */
static int QWidget_setSizePolicy(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in setSizePolicy");
    int horizontal = dubL_checkint(L, 2);
    int vertical = dubL_checkint(L, 3);
    self->setSizePolicy((QSizePolicy::Policy)horizontal, (QSizePolicy::Policy)vertical);
    self->updateGeometry();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setSizePolicy: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setSizePolicy: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::setStyle(const char *text)
 * mimas/bind/QWidget.h:53
 */
static int QWidget_setStyle(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in setStyle");
    const char *text = dubL_checkstring(L, 2);
    self->setStyleSheet(QString(".%1 { %2 }").arg(self->cssClass()).arg(text));
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setStyle: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setStyle: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::setStyleSheet(const char *text)
 * mimas/bind/QWidget.h:54
 */
static int QWidget_setStyleSheet(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in setStyleSheet");
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



/** void mimas::QWidget::setWindowTitle(const char *text)
 * mimas/bind/QWidget.h:38
 */
static int QWidget_setWindowTitle(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in setWindowTitle");
    const char *text = dubL_checkstring(L, 2);
    self->setWindowTitle(text);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setWindowTitle: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setWindowTitle: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::show()
 * mimas/bind/QWidget.h:29
 */
static int QWidget_show(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in show");
    self->show();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "show: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "show: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::showFullScreen(bool enable=true)
 * mimas/bind/QWidget.h:67
 */
static int QWidget_showFullScreen(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in showFullScreen");
    int top__ = lua_gettop(L);
    bool enable;
    if (top__ >= 2) {
      enable = lua_toboolean(L, 2);
    } else {
      enable = true;
    }
    if (enable) {
      self->showFullScreen();
    } else {
      self->showNormal();
    }
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "showFullScreen: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "showFullScreen: Unknown exception");
  }
  return lua_error(L);
}



/** static void mimas::Widget::showHide(ThreadedLuaObject *obj, bool shown)
 * include/mimas/Widget.h:120
 */
static int Widget_showHide(lua_State *L) {
  try {
    ThreadedLuaObject *obj = *((ThreadedLuaObject **)dubL_checksdata(L, 1, "mimas.ThreadedLuaObject"));
    bool shown = lua_toboolean(L, 2);
    Widget::showHide(obj, shown);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "showHide: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "showHide: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::QWidget::size()
 * mimas/bind/QWidget.h:52
 */
static int QWidget_size(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in size");
    QRect rect = self->geometry();
    lua_pushnumber(L, rect.width());
    lua_pushnumber(L, rect.height());
    return 2;
  } catch (std::exception &e) {
    lua_pushfstring(L, "size: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "size: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::swapFullScreen()
 * mimas/bind/QWidget.h:71
 */
static int QWidget_swapFullScreen(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in swapFullScreen");
    if (!self->isFullScreen()) {
      self->showFullScreen();
    } else {
      self->showNormal();
    }
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "swapFullScreen: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "swapFullScreen: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::textSize(const char *text)
 * mimas/bind/QWidget.h:57
 */
static int QWidget_textSize(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in textSize");
    const char *text = dubL_checkstring(L, 2);
    lua_pushnumber(L, self->fontMetrics().width(text));
    lua_pushnumber(L, self->fontMetrics().height());
    return 2;
  } catch (std::exception &e) {
    lua_pushfstring(L, "textSize: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "textSize: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::update()
 * mimas/bind/QWidget.h:20
 */
static int QWidget_update(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in update");
    self->update();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "update: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "update: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::widget()
 * mimas/bind/QWidget.h:44
 */
static int QWidget_widget(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in widget");
    QWidget* retval__ = self;
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "widget: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "widget: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::QWidget::width()
 * mimas/bind/QWidget.h:17
 */
static int QWidget_width(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in width");
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



/** LuaStackSize mimas::QWidget::windowTitle()
 * mimas/bind/QWidget.h:48
 */
static int QWidget_windowTitle(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in windowTitle");
    lua_pushstring(L, self->windowTitle().toUtf8());
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "windowTitle: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "windowTitle: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::QWidget::x()
 * mimas/bind/QWidget.h:15
 */
static int QWidget_x(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in x");
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



/** int mimas::QWidget::y()
 * mimas/bind/QWidget.h:16
 */
static int QWidget_y(lua_State *L) {
  try {
    Widget *self = *((Widget**)dubL_checksdata(L, 1, "mimas.Widget"));
    if (!self) throw dub::Exception("Using deleted mimas.Widget in y");
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

static const struct luaL_Reg Widget_member_methods[] = {
  {"activateWindow"    , QWidget_activateWindow},
  {"addAction"         , QWidget_addAction},
  {"addWidget"         , QWidget_addWidget},
  {"adjustSize"        , QWidget_adjustSize},
  {"close"             , QWidget_close},
  {"cssClass"          , Widget_cssClass},
  {"getExistingDirectory", Widget_getExistingDirectory},
  {"getOpenFileName"   , Widget_getOpenFileName},
  {"globalMove"        , QWidget_globalMove},
  {"globalPosition"    , QWidget_globalPosition},
  {"height"            , QWidget_height},
  {"hide"              , QWidget_hide},
  {"hue"               , Widget_hue},
  {"isFullScreen"      , QWidget_isFullScreen},
  {"isVisible"         , QWidget_isVisible},
  {"lower"             , QWidget_lower},
  {"move"              , QWidget_move},
  {"name"              , QObject_name},
  {"object"            , QObject_object},
  {"position"          , QWidget_position},
  {"raise"             , QWidget_raise},
  {"resize"            , QWidget_resize},
  {"setCssClass"       , Widget_setCssClass},
  {"setHue"            , Widget_setHue},
  {"setMinimumSize"    , QWidget_setMinimumSize},
  {"setMouseTracking"  , QWidget_setMouseTracking},
  {"setName"           , QObject_setName},
  {"setParent"         , QWidget_setParent},
  {"setSizeHint"       , QWidget_setSizeHint},
  {"setSizePolicy"     , QWidget_setSizePolicy},
  {"setStyle"          , QWidget_setStyle},
  {"setStyleSheet"     , QWidget_setStyleSheet},
  {"setWindowTitle"    , QWidget_setWindowTitle},
  {"show"              , QWidget_show},
  {"showFullScreen"    , QWidget_showFullScreen},
  {"size"              , QWidget_size},
  {"swapFullScreen"    , QWidget_swapFullScreen},
  {"textSize"          , QWidget_textSize},
  {"update"            , QWidget_update},
  {"widget"            , QWidget_widget},
  {"width"             , QWidget_width},
  {"windowTitle"       , QWidget_windowTitle},
  {"x"                 , QWidget_x},
  {"y"                 , QWidget_y},
  {"__tostring"        , Widget__tostring},
  {"__gc"              , Widget_destructor},
  {"deleted"           , Widget_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg Widget_namespace_methods[] = {
  {"Widget"            , Widget_Widget},
  {"Widget_mouse"      , Widget_mouse},
  {"Widget_showHide"   , Widget_showHide},
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
