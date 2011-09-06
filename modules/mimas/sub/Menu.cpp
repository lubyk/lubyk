#include "mimas/Menu.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Menu::Menu(const char *name)
 * include/mimas/Menu.h:53
 */
static int Menu_Menu(lua_State *L) {
  try {
    const char *name = dubL_checkstring(L, 1);
    Menu * retval__ = new Menu(name);
    // The class inherits from 'LuaCallback', use lua_init instead of pushclass.
    return retval__->luaInit(L, retval__, "mimas.Menu");
  } catch (std::exception &e) {
    lua_pushfstring(L, "Menu: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Menu: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Menu_destructor(lua_State *L) {
  Menu **userdata = (Menu**)dubL_checksdata_n(L, 1, "mimas.Menu");

  
  // custom destructor
  if (*userdata) (*userdata)->luaDestroy();
  
  *userdata = NULL;
  return 0;
}




// test if class is deleted
static int Menu_deleted(lua_State *L) {
  Menu **userdata = (Menu**)dubL_checksdata_n(L, 1, "mimas.Menu");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Menu__tostring(lua_State *L) {
  Menu **userdata = (Menu**)dubL_checksdata_n(L, 1, "mimas.Menu");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.Menu: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.Menu: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::QWidget::activateWindow()
 * mimas/bind/QWidget.h:32
 */
static int QWidget_activateWindow(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
    self->activateWindow();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "activateWindow: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "activateWindow: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QMenu::addAction(Action *action)
 * mimas/bind/QMenu.h:12
 */
static int QMenu_addAction(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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



/** void mimas::QWidget::addAction(Action *action)
 * mimas/bind/QWidget.h:34
 */
static int QWidget_addAction(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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



/** void mimas::QWidget::addWidget(QWidget *widget, float x=0, float y=0)
 * mimas/bind/QWidget.h:41
 */
static int QWidget_addWidget(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
    int top__ = lua_gettop(L);
    QWidget *widget = *((QWidget **)dubL_checksdata(L, 2, "mimas.QWidget"));
    float x = 0;
    float y = 0;
    if (top__ >= 3) {
      x = dubL_checknumber(L, 3);
      if (top__ >= 4) {
        y = dubL_checknumber(L, 4);
      }
    }
    widget->setParent(self);
    widget->move(x, y);
    widget->show();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "addWidget: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "addWidget: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QMenu::clear()
 * mimas/bind/QMenu.h:15
 */
static int QMenu_clear(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
    self->clear();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "clear: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "clear: Unknown exception");
  }
  return lua_error(L);
}



/** bool mimas::QWidget::close()
 * mimas/bind/QWidget.h:26
 */
static int QWidget_close(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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



/** QString mimas::Menu::cssClass() const 
 * include/mimas/Menu.h:59
 */
static int Menu_cssClass(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
    if (!self) throw dub::Exception("Using deleted mimas.Menu in cssClass");
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



/** void mimas::QWidget::globalMove(float x, float y)
 * mimas/bind/QWidget.h:70
 */
static int QWidget_globalMove(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
 * mimas/bind/QWidget.h:67
 */
static int QWidget_globalPosition(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
 * mimas/bind/QWidget.h:29
 */
static int QWidget_hide(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
    self->hide();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "hide: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "hide: Unknown exception");
  }
  return lua_error(L);
}



/** bool mimas::QWidget::isFullScreen()
 * mimas/bind/QWidget.h:33
 */
static int QWidget_isFullScreen(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
 * mimas/bind/QWidget.h:27
 */
static int QWidget_isVisible(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
 * mimas/bind/QWidget.h:30
 */
static int QWidget_lower(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
    self->lower();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "lower: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "lower: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::move(int x, int y)
 * mimas/bind/QWidget.h:13
 */
static int QWidget_move(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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



/** void mimas::QWidget::raise()
 * mimas/bind/QWidget.h:31
 */
static int QWidget_raise(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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



/** void mimas::QWidget::setMinimumSize(float w, float h)
 * mimas/bind/QWidget.h:22
 */
static int QWidget_setMinimumSize(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
 * mimas/bind/QWidget.h:25
 */
static int QWidget_setMouseTracking(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
 * mimas/bind/QWidget.h:54
 */
static int QWidget_setSizeHint(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
 * mimas/bind/QWidget.h:58
 */
static int QWidget_setSizePolicy(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
 * mimas/bind/QWidget.h:46
 */
static int QWidget_setStyle(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
 * mimas/bind/QWidget.h:47
 */
static int QWidget_setStyleSheet(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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



/** void mimas::QWidget::show()
 * mimas/bind/QWidget.h:28
 */
static int QWidget_show(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
 * mimas/bind/QWidget.h:60
 */
static int QWidget_showFullScreen(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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



/** void mimas::QWidget::size()
 * mimas/bind/QWidget.h:45
 */
static int QWidget_size(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
 * mimas/bind/QWidget.h:63
 */
static int QWidget_swapFullScreen(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
    bool enable = !self->isFullScreen();
    if (enable) {
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
 * mimas/bind/QWidget.h:50
 */
static int QWidget_textSize(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
 * mimas/bind/QWidget.h:40
 */
static int QWidget_widget(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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



/** int mimas::QWidget::x()
 * mimas/bind/QWidget.h:15
 */
static int QWidget_x(lua_State *L) {
  try {
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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
    Menu *self = *((Menu**)dubL_checksdata(L, 1, "mimas.Menu"));
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

static const struct luaL_Reg Menu_member_methods[] = {
  {"activateWindow"    , QWidget_activateWindow},
  {"addAction"         , QMenu_addAction},
  {"addAction"         , QWidget_addAction},
  {"addWidget"         , QWidget_addWidget},
  {"clear"             , QMenu_clear},
  {"close"             , QWidget_close},
  {"cssClass"          , Menu_cssClass},
  {"globalMove"        , QWidget_globalMove},
  {"globalPosition"    , QWidget_globalPosition},
  {"height"            , QWidget_height},
  {"hide"              , QWidget_hide},
  {"isFullScreen"      , QWidget_isFullScreen},
  {"isVisible"         , QWidget_isVisible},
  {"lower"             , QWidget_lower},
  {"move"              , QWidget_move},
  {"name"              , QObject_name},
  {"object"            , QObject_object},
  {"raise"             , QWidget_raise},
  {"resize"            , QWidget_resize},
  {"setMinimumSize"    , QWidget_setMinimumSize},
  {"setMouseTracking"  , QWidget_setMouseTracking},
  {"setName"           , QObject_setName},
  {"setParent"         , QWidget_setParent},
  {"setSizeHint"       , QWidget_setSizeHint},
  {"setSizePolicy"     , QWidget_setSizePolicy},
  {"setStyle"          , QWidget_setStyle},
  {"setStyleSheet"     , QWidget_setStyleSheet},
  {"show"              , QWidget_show},
  {"showFullScreen"    , QWidget_showFullScreen},
  {"size"              , QWidget_size},
  {"swapFullScreen"    , QWidget_swapFullScreen},
  {"textSize"          , QWidget_textSize},
  {"update"            , QWidget_update},
  {"widget"            , QWidget_widget},
  {"width"             , QWidget_width},
  {"x"                 , QWidget_x},
  {"y"                 , QWidget_y},
  {"__tostring"        , Menu__tostring},
  {"__gc"              , Menu_destructor},
  {"deleted"           , Menu_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg Menu_namespace_methods[] = {
  {"Menu"              , Menu_Menu},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Menu(lua_State *L) {
#else
extern "C" int luaopen_mimas_Menu(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Menu");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Menu_member_methods);
  // save meta-table in mimas.Menu_
  register_mt(L, "mimas", "Menu");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Menu_namespace_methods);


	return 1;
}
