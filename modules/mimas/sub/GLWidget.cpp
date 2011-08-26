#include "mimas/GLWidget.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::GLWidget::GLWidget(lubyk::Worker *worker)
 * include/mimas/GLWidget.h:53
 */
static int GLWidget_GLWidget(lua_State *L) {
  try {
    lubyk::Worker *worker = *((lubyk::Worker **)dubL_checksdata(L, 1, "lubyk.Worker"));
    GLWidget * retval__ = new GLWidget(worker);
    // The class inherits from 'LuaCallback', use lua_init instead of pushclass.
    return retval__->luaInit(L, retval__, "mimas.GLWidget");
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.GLWidget.GLWidget: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.GLWidget.GLWidget: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int GLWidget_destructor(lua_State *L) {
  GLWidget **userdata = (GLWidget**)dubL_checksdata_n(L, 1, "mimas.GLWidget");

  
  // custom destructor
  if (*userdata) (*userdata)->dub_destroy();
  
  *userdata = NULL;
  return 0;
}




// test if class is deleted
static int GLWidget_deleted(lua_State *L) {
  GLWidget **userdata = (GLWidget**)dubL_checksdata_n(L, 1, "mimas.GLWidget");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int GLWidget__tostring(lua_State *L) {
  GLWidget **userdata = (GLWidget**)dubL_checksdata_n(L, 1, "mimas.GLWidget");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.GLWidget: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.GLWidget: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::GLWidget::activateWindow()
 * include/mimas/GLWidget.h:130
 */
static int GLWidget_activateWindow(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)dubL_checksdata(L, 1, "mimas.GLWidget"));
    if (!self__) throw dub::Exception("Using deleted mimas.GLWidget in activateWindow");
    self__->activateWindow();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.GLWidget.activateWindow: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.GLWidget.activateWindow: Unknown exception");
  }
  return lua_error(L);
}



/** bool mimas::GLWidget::close()
 * include/mimas/GLWidget.h:118
 */
static int GLWidget_close(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)dubL_checksdata(L, 1, "mimas.GLWidget"));
    if (!self__) throw dub::Exception("Using deleted mimas.GLWidget in close");
    bool  retval__ = self__->close();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.GLWidget.close: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.GLWidget.close: Unknown exception");
  }
  return lua_error(L);
}



/** QString mimas::GLWidget::cssClass() const 
 * include/mimas/GLWidget.h:65
 */
static int GLWidget_cssClass(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)dubL_checksdata(L, 1, "mimas.GLWidget"));
    if (!self__) throw dub::Exception("Using deleted mimas.GLWidget in cssClass");
    QString  retval__ = self__->cssClass();
    lua_pushclass<QString>(L, retval__, "mimas.QString");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.GLWidget.cssClass: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.GLWidget.cssClass: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::GLWidget::height()
 * include/mimas/GLWidget.h:110
 */
static int GLWidget_height(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)dubL_checksdata(L, 1, "mimas.GLWidget"));
    if (!self__) throw dub::Exception("Using deleted mimas.GLWidget in height");
    int  retval__ = self__->height();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.GLWidget.height: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.GLWidget.height: Unknown exception");
  }
  return lua_error(L);
}



/** bool mimas::GLWidget::isVisible() const 
 * include/mimas/GLWidget.h:122
 */
static int GLWidget_isVisible(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)dubL_checksdata(L, 1, "mimas.GLWidget"));
    if (!self__) throw dub::Exception("Using deleted mimas.GLWidget in isVisible");
    bool  retval__ = self__->isVisible();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.GLWidget.isVisible: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.GLWidget.isVisible: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::GLWidget::move(int x, int y)
 * include/mimas/GLWidget.h:90
 */
static int GLWidget_move(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)dubL_checksdata(L, 1, "mimas.GLWidget"));
    if (!self__) throw dub::Exception("Using deleted mimas.GLWidget in move");
    int x = dubL_checkint(L, 2);
    int y = dubL_checkint(L, 3);
    self__->move(x, y);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.GLWidget.move: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.GLWidget.move: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::GLWidget::name(lua_State *L)
 * include/mimas/GLWidget.h:79
 */
static int GLWidget_name(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)dubL_checksdata(L, 1, "mimas.GLWidget"));
    if (!self__) throw dub::Exception("Using deleted mimas.GLWidget in name");
    
    LuaStackSize  retval__ = self__->name(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.GLWidget.name: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.GLWidget.name: Unknown exception");
  }
  return lua_error(L);
}



/** QObject* mimas::GLWidget::object()
 * include/mimas/GLWidget.h:73
 */
static int GLWidget_object(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)dubL_checksdata(L, 1, "mimas.GLWidget"));
    if (!self__) throw dub::Exception("Using deleted mimas.GLWidget in object");
    QObject * retval__ = self__->object();
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.GLWidget.object: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.GLWidget.object: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::GLWidget::resize(int w, int h)
 * include/mimas/GLWidget.h:94
 */
static int GLWidget_resize(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)dubL_checksdata(L, 1, "mimas.GLWidget"));
    if (!self__) throw dub::Exception("Using deleted mimas.GLWidget in resize");
    int w = dubL_checkint(L, 2);
    int h = dubL_checkint(L, 3);
    self__->resize(w, h);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.GLWidget.resize: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.GLWidget.resize: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::GLWidget::setName(const char *name)
 * include/mimas/GLWidget.h:86
 */
static int GLWidget_setName(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)dubL_checksdata(L, 1, "mimas.GLWidget"));
    if (!self__) throw dub::Exception("Using deleted mimas.GLWidget in setName");
    const char *name = dubL_checkstring(L, 2);
    self__->setName(name);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.GLWidget.setName: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.GLWidget.setName: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::GLWidget::show()
 * include/mimas/GLWidget.h:126
 */
static int GLWidget_show(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)dubL_checksdata(L, 1, "mimas.GLWidget"));
    if (!self__) throw dub::Exception("Using deleted mimas.GLWidget in show");
    self__->show();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.GLWidget.show: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.GLWidget.show: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::GLWidget::updateGL()
 * include/mimas/GLWidget.h:134
 */
static int GLWidget_updateGL(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)dubL_checksdata(L, 1, "mimas.GLWidget"));
    if (!self__) throw dub::Exception("Using deleted mimas.GLWidget in updateGL");
    self__->updateGL();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.GLWidget.updateGL: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.GLWidget.updateGL: Unknown exception");
  }
  return lua_error(L);
}



/** QWidget* mimas::GLWidget::widget()
 * include/mimas/GLWidget.h:69
 */
static int GLWidget_widget(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)dubL_checksdata(L, 1, "mimas.GLWidget"));
    if (!self__) throw dub::Exception("Using deleted mimas.GLWidget in widget");
    QWidget * retval__ = self__->widget();
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.GLWidget.widget: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.GLWidget.widget: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::GLWidget::width()
 * include/mimas/GLWidget.h:106
 */
static int GLWidget_width(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)dubL_checksdata(L, 1, "mimas.GLWidget"));
    if (!self__) throw dub::Exception("Using deleted mimas.GLWidget in width");
    int  retval__ = self__->width();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.GLWidget.width: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.GLWidget.width: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::GLWidget::x()
 * include/mimas/GLWidget.h:98
 */
static int GLWidget_x(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)dubL_checksdata(L, 1, "mimas.GLWidget"));
    if (!self__) throw dub::Exception("Using deleted mimas.GLWidget in x");
    int  retval__ = self__->x();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.GLWidget.x: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.GLWidget.x: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::GLWidget::y()
 * include/mimas/GLWidget.h:102
 */
static int GLWidget_y(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)dubL_checksdata(L, 1, "mimas.GLWidget"));
    if (!self__) throw dub::Exception("Using deleted mimas.GLWidget in y");
    int  retval__ = self__->y();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.GLWidget.y: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.GLWidget.y: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg GLWidget_member_methods[] = {
  {"activateWindow"    , GLWidget_activateWindow},
  {"close"             , GLWidget_close},
  {"cssClass"          , GLWidget_cssClass},
  {"height"            , GLWidget_height},
  {"isVisible"         , GLWidget_isVisible},
  {"move"              , GLWidget_move},
  {"name"              , GLWidget_name},
  {"object"            , GLWidget_object},
  {"resize"            , GLWidget_resize},
  {"setName"           , GLWidget_setName},
  {"show"              , GLWidget_show},
  {"updateGL"          , GLWidget_updateGL},
  {"widget"            , GLWidget_widget},
  {"width"             , GLWidget_width},
  {"x"                 , GLWidget_x},
  {"y"                 , GLWidget_y},
  {"__tostring"        , GLWidget__tostring},
  {"__gc"              , GLWidget_destructor},
  {"deleted"           , GLWidget_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg GLWidget_namespace_methods[] = {
  {"GLWidget"          , GLWidget_GLWidget},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_GLWidget(lua_State *L) {
#else
extern "C" int luaopen_mimas_GLWidget(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.GLWidget");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, GLWidget_member_methods);
  // save meta-table in mimas.GLWidget_
  register_mt(L, "mimas", "GLWidget");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", GLWidget_namespace_methods);


	return 1;
}
