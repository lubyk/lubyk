#include "mimas/GLWindow.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::GLWindow::GLWindow(lubyk::Worker *worker)
 * include/mimas/GLWindow.h:65
 */
static int GLWindow_GLWindow(lua_State *L) {
  try {
    lubyk::Worker *worker = *((lubyk::Worker **)luaL_checkudata(L, 1, "lubyk.Worker"));
    GLWindow * retval__ = new GLWindow(worker);
    lua_pushclass2<GLWindow>(L, retval__, "mimas.GLWindow");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWindow.GLWindow: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWindow.GLWindow: Unknown exception");
  }
}


/* ============================ Destructor       ====================== */

static int GLWindow_destructor(lua_State *L) {
  GLWindow **userdata = (GLWindow**)luaL_checkudata(L, 1, "mimas.GLWindow");

  
  // custom destructor
  if (*userdata) (*userdata)->dub_destroy();
  
  *userdata = NULL;
  return 0;
}




// test if class is deleted
static int GLWindow_deleted(lua_State *L) {
  GLWindow **userdata = (GLWindow**)luaL_checkudata(L, 1, "mimas.GLWindow");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int GLWindow__tostring(lua_State *L) {
  GLWindow **userdata = (GLWindow**)luaL_checkudata(L, 1, "mimas.GLWindow");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.GLWindow: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.GLWindow: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::GLWindow::__newindex(lua_State *L)
 * include/mimas/GLWindow.h:142
 */
static int GLWindow___newindex(lua_State *L) {
  try {
    GLWindow *self__ = *((GLWindow**)luaL_checkudata(L, 1, "mimas.GLWindow"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWindow in __newindex");
    
    self__->__newindex(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWindow.__newindex: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWindow.__newindex: Unknown exception");
  }
}


/** void mimas::GLWindow::activateWindow()
 * include/mimas/GLWindow.h:129
 */
static int GLWindow_activateWindow(lua_State *L) {
  try {
    GLWindow *self__ = *((GLWindow**)luaL_checkudata(L, 1, "mimas.GLWindow"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWindow in activateWindow");
    self__->activateWindow();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWindow.activateWindow: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWindow.activateWindow: Unknown exception");
  }
}


/** bool mimas::GLWindow::close()
 * include/mimas/GLWindow.h:114
 */
static int GLWindow_close(lua_State *L) {
  try {
    GLWindow *self__ = *((GLWindow**)luaL_checkudata(L, 1, "mimas.GLWindow"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWindow in close");
    bool  retval__ = self__->close();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWindow.close: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWindow.close: Unknown exception");
  }
}


/** QString mimas::GLWindow::cssClass() const 
 * include/mimas/GLWindow.h:75
 */
static int GLWindow_cssClass(lua_State *L) {
  try {
    GLWindow *self__ = *((GLWindow**)luaL_checkudata(L, 1, "mimas.GLWindow"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWindow in cssClass");
    QString  retval__ = self__->cssClass();
    lua_pushclass<QString>(L, retval__, "mimas.QString");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWindow.cssClass: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWindow.cssClass: Unknown exception");
  }
}


/** bool mimas::GLWindow::isVisible() const 
 * include/mimas/GLWindow.h:118
 */
static int GLWindow_isVisible(lua_State *L) {
  try {
    GLWindow *self__ = *((GLWindow**)luaL_checkudata(L, 1, "mimas.GLWindow"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWindow in isVisible");
    bool  retval__ = self__->isVisible();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWindow.isVisible: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWindow.isVisible: Unknown exception");
  }
}


/** void mimas::GLWindow::move(int x, int y)
 * include/mimas/GLWindow.h:100
 */
static int GLWindow_move(lua_State *L) {
  try {
    GLWindow *self__ = *((GLWindow**)luaL_checkudata(L, 1, "mimas.GLWindow"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWindow in move");
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    self__->move(x, y);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWindow.move: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWindow.move: Unknown exception");
  }
}


/** LuaStackSize mimas::GLWindow::name(lua_State *L)
 * include/mimas/GLWindow.h:89
 */
static int GLWindow_name(lua_State *L) {
  try {
    GLWindow *self__ = *((GLWindow**)luaL_checkudata(L, 1, "mimas.GLWindow"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWindow in name");
    
    LuaStackSize  retval__ = self__->name(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWindow.name: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWindow.name: Unknown exception");
  }
}


/** QObject* mimas::GLWindow::object()
 * include/mimas/GLWindow.h:83
 */
static int GLWindow_object(lua_State *L) {
  try {
    GLWindow *self__ = *((GLWindow**)luaL_checkudata(L, 1, "mimas.GLWindow"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWindow in object");
    QObject * retval__ = self__->object();
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWindow.object: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWindow.object: Unknown exception");
  }
}


/** void mimas::GLWindow::resize(int w, int h)
 * include/mimas/GLWindow.h:104
 */
static int GLWindow_resize(lua_State *L) {
  try {
    GLWindow *self__ = *((GLWindow**)luaL_checkudata(L, 1, "mimas.GLWindow"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWindow in resize");
    int w = luaL_checkint(L, 2);
    int h = luaL_checkint(L, 3);
    self__->resize(w, h);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWindow.resize: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWindow.resize: Unknown exception");
  }
}


/** void mimas::GLWindow::setName(const char *name)
 * include/mimas/GLWindow.h:96
 */
static int GLWindow_setName(lua_State *L) {
  try {
    GLWindow *self__ = *((GLWindow**)luaL_checkudata(L, 1, "mimas.GLWindow"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWindow in setName");
    const char *name = luaL_checkstring(L, 2);
    self__->setName(name);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWindow.setName: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWindow.setName: Unknown exception");
  }
}


/** void mimas::GLWindow::show()
 * include/mimas/GLWindow.h:122
 */
static int GLWindow_show(lua_State *L) {
  try {
    GLWindow *self__ = *((GLWindow**)luaL_checkudata(L, 1, "mimas.GLWindow"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWindow in show");
    self__->show();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWindow.show: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWindow.show: Unknown exception");
  }
}


/** void mimas::GLWindow::updateGL()
 * include/mimas/GLWindow.h:133
 */
static int GLWindow_updateGL(lua_State *L) {
  try {
    GLWindow *self__ = *((GLWindow**)luaL_checkudata(L, 1, "mimas.GLWindow"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWindow in updateGL");
    self__->updateGL();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWindow.updateGL: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWindow.updateGL: Unknown exception");
  }
}


/** QWidget* mimas::GLWindow::widget()
 * include/mimas/GLWindow.h:79
 */
static int GLWindow_widget(lua_State *L) {
  try {
    GLWindow *self__ = *((GLWindow**)luaL_checkudata(L, 1, "mimas.GLWindow"));
    if (!self__) return luaL_error(L, "Using deleted mimas.GLWindow in widget");
    QWidget * retval__ = self__->widget();
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.GLWindow.widget: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.GLWindow.widget: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg GLWindow_member_methods[] = {
  {"__newindex"        , GLWindow___newindex},
  {"activateWindow"    , GLWindow_activateWindow},
  {"close"             , GLWindow_close},
  {"cssClass"          , GLWindow_cssClass},
  {"isVisible"         , GLWindow_isVisible},
  {"move"              , GLWindow_move},
  {"name"              , GLWindow_name},
  {"object"            , GLWindow_object},
  {"resize"            , GLWindow_resize},
  {"setName"           , GLWindow_setName},
  {"show"              , GLWindow_show},
  {"updateGL"          , GLWindow_updateGL},
  {"widget"            , GLWindow_widget},
  {"__tostring"        , GLWindow__tostring},
  {"__gc"              , GLWindow_destructor},
  {"deleted"           , GLWindow_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg GLWindow_namespace_methods[] = {
  {"GLWindow"          , GLWindow_GLWindow},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_GLWindow(lua_State *L) {
#else
extern "C" int luaopen_mimas_GLWindow(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.GLWindow");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, GLWindow_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "mimas", GLWindow_namespace_methods);


	return 1;
}
