#include "mimas/GLSLWidget.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::GLSLWidget::GLSLWidget()
 * include/mimas/GLSLWidget.h:99
 */
static int GLSLWidget_GLSLWidget(lua_State *L) {
  try {
    GLSLWidget * retval__ = new GLSLWidget();
    return retval__->luaInit(L, retval__, "mimas.GLSLWidget");
  } catch (std::exception &e) {
    lua_pushfstring(L, "GLSLWidget: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "GLSLWidget: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int GLSLWidget_destructor(lua_State *L) {
  GLSLWidget **userdata = (GLSLWidget**)dubL_checksdata_n(L, 1, "mimas.GLSLWidget");

  
  // custom destructor
  GLSLWidget *self = *userdata;
  if (self) self->luaDestroy();
  
  *userdata = NULL;
  return 0;
}



// test if class is deleted
static int GLSLWidget_deleted(lua_State *L) {
  GLSLWidget **userdata = (GLSLWidget**)dubL_checksdata_n(L, 1, "mimas.GLSLWidget");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int GLSLWidget__tostring(lua_State *L) {
  GLSLWidget **userdata = (GLSLWidget**)dubL_checksdata_n(L, 1, "mimas.GLSLWidget");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.GLSLWidget: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.GLSLWidget: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::QWidget::activateWindow()
 * mimas/bind/QWidget.h:32
 */
static int QWidget_activateWindow(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in activateWindow");
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
 * mimas/bind/QWidget.h:34
 */
static int QWidget_addAction(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in addAction");
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
 * mimas/bind/QWidget.h:44
 */
static int QWidget_addWidget(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in addWidget");
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



/** bool mimas::QWidget::close()
 * mimas/bind/QWidget.h:26
 */
static int QWidget_close(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in close");
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



/** bool mimas::GLSLWidget::compile(const char *vertex_shader, const char *fragment_shader)
 * include/mimas/GLSLWidget.h:129
 */
static int GLSLWidget_compile(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in compile");
    const char *vertex_shader = dubL_checkstring(L, 2);
    const char *fragment_shader = dubL_checkstring(L, 3);
    bool  retval__ = self->compile(vertex_shader, fragment_shader);
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "compile: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "compile: Unknown exception");
  }
  return lua_error(L);
}



/** QString mimas::GLSLWidget::cssClass() const 
 * include/mimas/GLSLWidget.h:112
 */
static int GLSLWidget_cssClass(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in cssClass");
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
 * mimas/bind/QWidget.h:83
 */
static int QWidget_globalMove(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in globalMove");
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
 * mimas/bind/QWidget.h:74
 */
static int QWidget_globalPosition(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in globalPosition");
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
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in height");
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
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in hide");
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
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in isFullScreen");
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
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in isVisible");
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
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in lower");
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
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in move");
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
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in name");
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
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in object");
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



/** LuaStackSize mimas::GLSLWidget::openGLVersion(lua_State *L)
 * include/mimas/GLSLWidget.h:123
 */
static int GLSLWidget_openGLVersion(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in openGLVersion");
    
    LuaStackSize  retval__ = self->openGLVersion(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "openGLVersion: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "openGLVersion: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::QWidget::position()
 * mimas/bind/QWidget.h:79
 */
static int QWidget_position(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in position");
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
 * mimas/bind/QWidget.h:31
 */
static int QWidget_raise(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in raise");
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
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in resize");
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
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in setMinimumSize");
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
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in setMouseTracking");
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
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in setName");
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
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in setParent");
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
 * mimas/bind/QWidget.h:60
 */
static int QWidget_setSizeHint(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in setSizeHint");
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
 * mimas/bind/QWidget.h:64
 */
static int QWidget_setSizePolicy(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in setSizePolicy");
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
 * mimas/bind/QWidget.h:52
 */
static int QWidget_setStyle(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in setStyle");
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
 * mimas/bind/QWidget.h:53
 */
static int QWidget_setStyleSheet(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in setStyleSheet");
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
 * mimas/bind/QWidget.h:37
 */
static int QWidget_setWindowTitle(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in setWindowTitle");
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
 * mimas/bind/QWidget.h:28
 */
static int QWidget_show(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in show");
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
 * mimas/bind/QWidget.h:66
 */
static int QWidget_showFullScreen(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in showFullScreen");
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



/** LuaStackSize mimas::QWidget::size()
 * mimas/bind/QWidget.h:51
 */
static int QWidget_size(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in size");
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
 * mimas/bind/QWidget.h:70
 */
static int QWidget_swapFullScreen(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in swapFullScreen");
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
 * mimas/bind/QWidget.h:56
 */
static int QWidget_textSize(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in textSize");
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
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in update");
    self->update();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "update: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "update: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::GLSLWidget::updateGL()
 * include/mimas/GLSLWidget.h:119
 */
static int GLSLWidget_updateGL(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in updateGL");
    self->updateGL();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "updateGL: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "updateGL: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::widget()
 * mimas/bind/QWidget.h:43
 */
static int QWidget_widget(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in widget");
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
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in width");
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
 * mimas/bind/QWidget.h:47
 */
static int QWidget_windowTitle(lua_State *L) {
  try {
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in windowTitle");
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
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in x");
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
    GLSLWidget *self = *((GLSLWidget**)dubL_checksdata(L, 1, "mimas.GLSLWidget"));
    if (!self) throw dub::Exception("Using deleted mimas.GLSLWidget in y");
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

static const struct luaL_Reg GLSLWidget_member_methods[] = {
  {"activateWindow"    , QWidget_activateWindow},
  {"addAction"         , QWidget_addAction},
  {"addWidget"         , QWidget_addWidget},
  {"close"             , QWidget_close},
  {"compile"           , GLSLWidget_compile},
  {"cssClass"          , GLSLWidget_cssClass},
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
  {"openGLVersion"     , GLSLWidget_openGLVersion},
  {"position"          , QWidget_position},
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
  {"setWindowTitle"    , QWidget_setWindowTitle},
  {"show"              , QWidget_show},
  {"showFullScreen"    , QWidget_showFullScreen},
  {"size"              , QWidget_size},
  {"swapFullScreen"    , QWidget_swapFullScreen},
  {"textSize"          , QWidget_textSize},
  {"update"            , QWidget_update},
  {"updateGL"          , GLSLWidget_updateGL},
  {"widget"            , QWidget_widget},
  {"width"             , QWidget_width},
  {"windowTitle"       , QWidget_windowTitle},
  {"x"                 , QWidget_x},
  {"y"                 , QWidget_y},
  {"__tostring"        , GLSLWidget__tostring},
  {"__gc"              , GLSLWidget_destructor},
  {"deleted"           , GLSLWidget_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg GLSLWidget_namespace_methods[] = {
  {"GLSLWidget"        , GLSLWidget_GLSLWidget},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_GLSLWidget(lua_State *L) {
#else
extern "C" int luaopen_mimas_GLSLWidget(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.GLSLWidget");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, GLSLWidget_member_methods);
  // save meta-table in mimas.GLSLWidget_
  register_mt(L, "mimas", "GLSLWidget");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", GLSLWidget_namespace_methods);


	return 1;
}
