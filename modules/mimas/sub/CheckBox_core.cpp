#include "mimas/CheckBox.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::CheckBox::CheckBox(const char *title=NULL, QWidget *parent=NULL)
 * include/mimas/CheckBox.h:59
 */
static int CheckBox_CheckBox(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    CheckBox * retval__;
    if (top__ < 1) {
      retval__ = new CheckBox();
    } else {
      const char *title = dubL_checkstring(L, 1);
      if (top__ < 2) {
        retval__ = new CheckBox(title);
      } else {
        QWidget *parent = *((QWidget **)dubL_checksdata(L, 2, "mimas.QWidget"));
        retval__ = new CheckBox(title, parent);
      }
    }
    return retval__->luaInit(L, retval__, "mimas.CheckBox");
  } catch (std::exception &e) {
    lua_pushfstring(L, "CheckBox: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "CheckBox: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int CheckBox_destructor(lua_State *L) {
  CheckBox **userdata = (CheckBox**)dubL_checksdata_n(L, 1, "mimas.CheckBox");

  
  // custom destructor
  CheckBox *self = *userdata;
  if (self) self->luaDestroy();
  
  *userdata = NULL;
  return 0;
}



// test if class is deleted
static int CheckBox_deleted(lua_State *L) {
  CheckBox **userdata = (CheckBox**)dubL_checksdata_n(L, 1, "mimas.CheckBox");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int CheckBox__tostring(lua_State *L) {
  CheckBox **userdata = (CheckBox**)dubL_checksdata_n(L, 1, "mimas.CheckBox");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.CheckBox: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.CheckBox: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::QWidget::activateWindow()
 * mimas/bind/QWidget.h:37
 */
static int QWidget_activateWindow(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in activateWindow");
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
 * mimas/bind/QWidget.h:39
 */
static int QWidget_addAction(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in addAction");
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
 * mimas/bind/QWidget.h:49
 */
static int QWidget_addWidget(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in addWidget");
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
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in adjustSize");
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
 * mimas/bind/QWidget.h:31
 */
static int QWidget_close(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in close");
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



/** QString mimas::CheckBox::cssClass() const 
 * include/mimas/CheckBox.h:69
 */
static int CheckBox_cssClass(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in cssClass");
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
 * mimas/bind/QWidget.h:88
 */
static int QWidget_globalMove(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in globalMove");
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
 * mimas/bind/QWidget.h:79
 */
static int QWidget_globalPosition(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in globalPosition");
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
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in height");
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
 * mimas/bind/QWidget.h:34
 */
static int QWidget_hide(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in hide");
    self->hide();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "hide: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "hide: Unknown exception");
  }
  return lua_error(L);
}



/** float mimas::CheckBox::hue()
 * include/mimas/CheckBox.h:84
 */
static int CheckBox_hue(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in hue");
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



/** bool mimas::QCheckBox::isCheckable()
 * mimas/bind/QCheckBox.h:12
 */
static int QCheckBox_isCheckable(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in isCheckable");
    bool  retval__ = self->isCheckable();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "isCheckable: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "isCheckable: Unknown exception");
  }
  return lua_error(L);
}



/** bool mimas::QCheckBox::isChecked()
 * mimas/bind/QCheckBox.h:15
 */
static int QCheckBox_isChecked(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in isChecked");
    bool  retval__ = self->isChecked();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "isChecked: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "isChecked: Unknown exception");
  }
  return lua_error(L);
}



/** bool mimas::QWidget::isFullScreen()
 * mimas/bind/QWidget.h:38
 */
static int QWidget_isFullScreen(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in isFullScreen");
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
 * mimas/bind/QWidget.h:32
 */
static int QWidget_isVisible(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in isVisible");
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
 * mimas/bind/QWidget.h:35
 */
static int QWidget_lower(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in lower");
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
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in move");
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
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in name");
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
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in object");
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
 * mimas/bind/QWidget.h:84
 */
static int QWidget_position(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in position");
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
 * mimas/bind/QWidget.h:36
 */
static int QWidget_raise(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in raise");
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
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in resize");
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



/** void mimas::QWidget::setAttribute(int attr, bool enabled)
 * mimas/bind/QWidget.h:24
 */
static int QWidget_setAttribute(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in setAttribute");
    int attr = dubL_checkint(L, 2);
    bool enabled = lua_toboolean(L, 3);
    self->setAttribute((Qt::WidgetAttribute)attr, enabled);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setAttribute: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setAttribute: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QCheckBox::setCheckable(bool check)
 * mimas/bind/QCheckBox.h:13
 */
static int QCheckBox_setCheckable(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in setCheckable");
    bool check = lua_toboolean(L, 2);
    self->setCheckable(check);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setCheckable: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setCheckable: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QCheckBox::setChecked(bool check)
 * mimas/bind/QCheckBox.h:16
 */
static int QCheckBox_setChecked(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in setChecked");
    bool check = lua_toboolean(L, 2);
    self->setChecked(check);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setChecked: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setChecked: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::setFocus()
 * mimas/bind/QWidget.h:22
 */
static int QWidget_setFocus(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in setFocus");
    self->setFocus(Qt::OtherFocusReason);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setFocus: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setFocus: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::setFocusPolicy(int policy)
 * mimas/bind/QWidget.h:23
 */
static int QWidget_setFocusPolicy(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in setFocusPolicy");
    int policy = dubL_checkint(L, 2);
    self->setFocusPolicy((Qt::FocusPolicy)policy);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setFocusPolicy: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setFocusPolicy: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::CheckBox::setHue(float hue)
 * include/mimas/CheckBox.h:75
 */
static int CheckBox_setHue(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in setHue");
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
 * mimas/bind/QWidget.h:27
 */
static int QWidget_setMinimumSize(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in setMinimumSize");
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
 * mimas/bind/QWidget.h:30
 */
static int QWidget_setMouseTracking(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in setMouseTracking");
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
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in setName");
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
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in setParent");
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
 * mimas/bind/QWidget.h:65
 */
static int QWidget_setSizeHint(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in setSizeHint");
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
 * mimas/bind/QWidget.h:69
 */
static int QWidget_setSizePolicy(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in setSizePolicy");
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
 * mimas/bind/QWidget.h:57
 */
static int QWidget_setStyle(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in setStyle");
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
 * mimas/bind/QWidget.h:58
 */
static int QWidget_setStyleSheet(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in setStyleSheet");
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



/** void mimas::CheckBox::setText(const char *txt)
 * include/mimas/CheckBox.h:80
 */
static int CheckBox_setText(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in setText");
    const char *txt = dubL_checkstring(L, 2);
    self->setText(txt);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setText: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setText: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QWidget::setWindowTitle(const char *text)
 * mimas/bind/QWidget.h:42
 */
static int QWidget_setWindowTitle(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in setWindowTitle");
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
 * mimas/bind/QWidget.h:33
 */
static int QWidget_show(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in show");
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
 * mimas/bind/QWidget.h:71
 */
static int QWidget_showFullScreen(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in showFullScreen");
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
 * mimas/bind/QWidget.h:56
 */
static int QWidget_size(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in size");
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
 * mimas/bind/QWidget.h:75
 */
static int QWidget_swapFullScreen(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in swapFullScreen");
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
 * mimas/bind/QWidget.h:61
 */
static int QWidget_textSize(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in textSize");
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
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in update");
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
 * mimas/bind/QWidget.h:48
 */
static int QWidget_widget(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in widget");
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
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in width");
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
 * mimas/bind/QWidget.h:52
 */
static int QWidget_windowTitle(lua_State *L) {
  try {
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in windowTitle");
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
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in x");
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
    CheckBox *self = *((CheckBox**)dubL_checksdata(L, 1, "mimas.CheckBox"));
    if (!self) throw dub::Exception("Using deleted mimas.CheckBox in y");
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

static const struct luaL_Reg CheckBox_member_methods[] = {
  {"activateWindow"    , QWidget_activateWindow},
  {"addAction"         , QWidget_addAction},
  {"addWidget"         , QWidget_addWidget},
  {"adjustSize"        , QWidget_adjustSize},
  {"close"             , QWidget_close},
  {"cssClass"          , CheckBox_cssClass},
  {"globalMove"        , QWidget_globalMove},
  {"globalPosition"    , QWidget_globalPosition},
  {"height"            , QWidget_height},
  {"hide"              , QWidget_hide},
  {"hue"               , CheckBox_hue},
  {"isCheckable"       , QCheckBox_isCheckable},
  {"isChecked"         , QCheckBox_isChecked},
  {"isFullScreen"      , QWidget_isFullScreen},
  {"isVisible"         , QWidget_isVisible},
  {"lower"             , QWidget_lower},
  {"move"              , QWidget_move},
  {"name"              , QObject_name},
  {"object"            , QObject_object},
  {"position"          , QWidget_position},
  {"raise"             , QWidget_raise},
  {"resize"            , QWidget_resize},
  {"setAttribute"      , QWidget_setAttribute},
  {"setCheckable"      , QCheckBox_setCheckable},
  {"setChecked"        , QCheckBox_setChecked},
  {"setFocus"          , QWidget_setFocus},
  {"setFocusPolicy"    , QWidget_setFocusPolicy},
  {"setHue"            , CheckBox_setHue},
  {"setMinimumSize"    , QWidget_setMinimumSize},
  {"setMouseTracking"  , QWidget_setMouseTracking},
  {"setName"           , QObject_setName},
  {"setParent"         , QWidget_setParent},
  {"setSizeHint"       , QWidget_setSizeHint},
  {"setSizePolicy"     , QWidget_setSizePolicy},
  {"setStyle"          , QWidget_setStyle},
  {"setStyleSheet"     , QWidget_setStyleSheet},
  {"setText"           , CheckBox_setText},
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
  {"__tostring"        , CheckBox__tostring},
  {"__gc"              , CheckBox_destructor},
  {"deleted"           , CheckBox_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg CheckBox_namespace_methods[] = {
  {"CheckBox"          , CheckBox_CheckBox},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_CheckBox_core(lua_State *L) {
#else
extern "C" int luaopen_mimas_CheckBox_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.CheckBox");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, CheckBox_member_methods);
  // save meta-table in mimas.CheckBox_
  register_mt(L, "mimas", "CheckBox");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", CheckBox_namespace_methods);


	return 1;
}
