#include "mimas/GLWidget.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::GLWidget::GLWidget(lubyk::Worker *worker)
 * include/mimas/GLWidget.h:68
 */
static int GLWidget_GLWidget(lua_State *L) {
  try {
    lubyk::Worker *worker = *((lubyk::Worker **)luaL_checkudata(L, 1, "lubyk.Worker"));
    GLWidget * retval__ = new GLWidget(worker);
    lua_pushclass2<GLWidget>(L, retval__, "mimas.GLWidget");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWidget.GLWidget: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWidget.GLWidget: Unknown exception");
  }
}


/* ============================ Destructor       ====================== */

static int GLWidget_destructor(lua_State *L) {
  GLWidget **userdata = (GLWidget**)luaL_checkudata(L, 1, "mimas.GLWidget");

  
  // custom destructor
  if (*userdata) (*userdata)->dub_destroy();
  
  *userdata = NULL;
  return 0;
}




// test if class is deleted
static int GLWidget_deleted(lua_State *L) {
  GLWidget **userdata = (GLWidget**)luaL_checkudata(L, 1, "mimas.GLWidget");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int GLWidget__tostring(lua_State *L) {
  GLWidget **userdata = (GLWidget**)luaL_checkudata(L, 1, "mimas.GLWidget");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.GLWidget: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.GLWidget: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::GLWidget::__newindex(lua_State *L)
 * include/mimas/GLWidget.h:147
 */
static int GLWidget___newindex(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)luaL_checkudata(L, 1, "mimas.GLWidget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWidget in __newindex");
    
    self__->__newindex(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWidget.__newindex: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWidget.__newindex: Unknown exception");
  }
}


/** void mimas::GLWidget::activateWindow(lua_State *L)
 * include/mimas/GLWidget.h:133
 */
static int GLWidget_activateWindow(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)luaL_checkudata(L, 1, "mimas.GLWidget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWidget in activateWindow");
    
    self__->activateWindow(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWidget.activateWindow: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWidget.activateWindow: Unknown exception");
  }
}


/** bool mimas::GLWidget::close()
 * include/mimas/GLWidget.h:119
 */
static int GLWidget_close(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)luaL_checkudata(L, 1, "mimas.GLWidget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWidget in close");
    bool  retval__ = self__->close();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWidget.close: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWidget.close: Unknown exception");
  }
}


/** QString mimas::GLWidget::cssClass() const 
 * include/mimas/GLWidget.h:80
 */
static int GLWidget_cssClass(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)luaL_checkudata(L, 1, "mimas.GLWidget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWidget in cssClass");
    QString  retval__ = self__->cssClass();
    lua_pushclass<QString>(L, retval__, "mimas.QString");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWidget.cssClass: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWidget.cssClass: Unknown exception");
  }
}


/** bool mimas::GLWidget::isVisible() const 
 * include/mimas/GLWidget.h:123
 */
static int GLWidget_isVisible(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)luaL_checkudata(L, 1, "mimas.GLWidget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWidget in isVisible");
    bool  retval__ = self__->isVisible();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWidget.isVisible: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWidget.isVisible: Unknown exception");
  }
}


/** void mimas::GLWidget::move(int x, int y)
 * include/mimas/GLWidget.h:105
 */
static int GLWidget_move(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)luaL_checkudata(L, 1, "mimas.GLWidget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWidget in move");
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    self__->move(x, y);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWidget.move: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWidget.move: Unknown exception");
  }
}


/** LuaStackSize mimas::GLWidget::name(lua_State *L)
 * include/mimas/GLWidget.h:94
 */
static int GLWidget_name(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)luaL_checkudata(L, 1, "mimas.GLWidget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWidget in name");
    
    LuaStackSize  retval__ = self__->name(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWidget.name: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWidget.name: Unknown exception");
  }
}


/** QObject* mimas::GLWidget::object()
 * include/mimas/GLWidget.h:88
 */
static int GLWidget_object(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)luaL_checkudata(L, 1, "mimas.GLWidget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWidget in object");
    QObject * retval__ = self__->object();
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWidget.object: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWidget.object: Unknown exception");
  }
}


/** void mimas::GLWidget::resize(int w, int h, lua_State *L)
 * include/mimas/GLWidget.h:110
 */
static int GLWidget_resize(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)luaL_checkudata(L, 1, "mimas.GLWidget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWidget in resize");
    int w = luaL_checkint(L, 2);
    int h = luaL_checkint(L, 3);
    
    self__->resize(w, h, L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWidget.resize: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWidget.resize: Unknown exception");
  }
}


/** void mimas::GLWidget::setName(const char *name)
 * include/mimas/GLWidget.h:101
 */
static int GLWidget_setName(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)luaL_checkudata(L, 1, "mimas.GLWidget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWidget in setName");
    const char *name = luaL_checkstring(L, 2);
    self__->setName(name);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWidget.setName: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWidget.setName: Unknown exception");
  }
}


/** void mimas::GLWidget::show(lua_State *L)
 * include/mimas/GLWidget.h:128
 */
static int GLWidget_show(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)luaL_checkudata(L, 1, "mimas.GLWidget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWidget in show");
    
    self__->show(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWidget.show: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWidget.show: Unknown exception");
  }
}


/** void mimas::GLWidget::updateGL(lua_State *L)
 * include/mimas/GLWidget.h:139
 */
static int GLWidget_updateGL(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)luaL_checkudata(L, 1, "mimas.GLWidget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWidget in updateGL");
    
    self__->updateGL(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWidget.updateGL: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWidget.updateGL: Unknown exception");
  }
}


/** QWidget* mimas::GLWidget::widget()
 * include/mimas/GLWidget.h:84
 */
static int GLWidget_widget(lua_State *L) {
  try {
    GLWidget *self__ = *((GLWidget**)luaL_checkudata(L, 1, "mimas.GLWidget"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWidget in widget");
    QWidget * retval__ = self__->widget();
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWidget.widget: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWidget.widget: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg GLWidget_member_methods[] = {
  {"__newindex"        , GLWidget___newindex},
  {"activateWindow"    , GLWidget_activateWindow},
  {"close"             , GLWidget_close},
  {"cssClass"          , GLWidget_cssClass},
  {"isVisible"         , GLWidget_isVisible},
  {"move"              , GLWidget_move},
  {"name"              , GLWidget_name},
  {"object"            , GLWidget_object},
  {"resize"            , GLWidget_resize},
  {"setName"           , GLWidget_setName},
  {"show"              , GLWidget_show},
  {"updateGL"          , GLWidget_updateGL},
  {"widget"            , GLWidget_widget},
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
