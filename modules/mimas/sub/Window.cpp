#include "mimas/Window.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Window::Window(lubyk::Worker *worker)
 * include/mimas/Window.h:59
 */
static int Window_Window(lua_State *L) {
  try {
    lubyk::Worker *worker = *((lubyk::Worker **)luaL_checkudata(L, 1, "lubyk.Worker"));
    Window * retval__ = new Window(worker);
    lua_pushclass2<Window>(L, retval__, "mimas.Window");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Window.Window: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Window.Window: Unknown exception");
  }
}


/* ============================ Destructor       ====================== */

static int Window_destructor(lua_State *L) {
  Window **userdata = (Window**)luaL_checkudata(L, 1, "mimas.Window");

  
  // custom destructor
  if (*userdata) (*userdata)->dub_destroy();
  
  *userdata = NULL;
  return 0;
}




// test if class is deleted
static int Window_deleted(lua_State *L) {
  Window **userdata = (Window**)luaL_checkudata(L, 1, "mimas.Window");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Window__tostring(lua_State *L) {
  Window **userdata = (Window**)luaL_checkudata(L, 1, "mimas.Window");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.Window: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.Window: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::Window::__newindex(lua_State *L)
 * include/mimas/Window.h:144
 */
static int Window___newindex(lua_State *L) {
  try {
    Window *self__ = *((Window**)luaL_checkudata(L, 1, "mimas.Window"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Window in __newindex");
    
    self__->__newindex(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Window.__newindex: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Window.__newindex: Unknown exception");
  }
}


/** void mimas::Window::activateWindow()
 * include/mimas/Window.h:137
 */
static int Window_activateWindow(lua_State *L) {
  try {
    Window *self__ = *((Window**)luaL_checkudata(L, 1, "mimas.Window"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Window in activateWindow");
    self__->activateWindow();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Window.activateWindow: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Window.activateWindow: Unknown exception");
  }
}


/** bool mimas::Window::close()
 * include/mimas/Window.h:123
 */
static int Window_close(lua_State *L) {
  try {
    Window *self__ = *((Window**)luaL_checkudata(L, 1, "mimas.Window"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Window in close");
    bool  retval__ = self__->close();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Window.close: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Window.close: Unknown exception");
  }
}


/** QString mimas::Window::cssClass() const 
 * include/mimas/Window.h:69
 */
static int Window_cssClass(lua_State *L) {
  try {
    Window *self__ = *((Window**)luaL_checkudata(L, 1, "mimas.Window"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Window in cssClass");
    QString  retval__ = self__->cssClass();
    lua_pushclass<QString>(L, retval__, "mimas.QString");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Window.cssClass: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Window.cssClass: Unknown exception");
  }
}


/** float mimas::Window::hue()
 * include/mimas/Window.h:111
 */
static int Window_hue(lua_State *L) {
  try {
    Window *self__ = *((Window**)luaL_checkudata(L, 1, "mimas.Window"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Window in hue");
    float  retval__ = self__->hue();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Window.hue: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Window.hue: Unknown exception");
  }
}


/** bool mimas::Window::isVisible() const 
 * include/mimas/Window.h:128
 */
static int Window_isVisible(lua_State *L) {
  try {
    Window *self__ = *((Window**)luaL_checkudata(L, 1, "mimas.Window"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Window in isVisible");
    bool  retval__ = self__->isVisible();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Window.isVisible: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Window.isVisible: Unknown exception");
  }
}


/** void mimas::Window::move(int x, int y)
 * include/mimas/Window.h:94
 */
static int Window_move(lua_State *L) {
  try {
    Window *self__ = *((Window**)luaL_checkudata(L, 1, "mimas.Window"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Window in move");
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    self__->move(x, y);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Window.move: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Window.move: Unknown exception");
  }
}


/** LuaStackSize mimas::Window::name(lua_State *L)
 * include/mimas/Window.h:83
 */
static int Window_name(lua_State *L) {
  try {
    Window *self__ = *((Window**)luaL_checkudata(L, 1, "mimas.Window"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Window in name");
    
    LuaStackSize  retval__ = self__->name(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Window.name: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Window.name: Unknown exception");
  }
}


/** QObject* mimas::Window::object()
 * include/mimas/Window.h:77
 */
static int Window_object(lua_State *L) {
  try {
    Window *self__ = *((Window**)luaL_checkudata(L, 1, "mimas.Window"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Window in object");
    QObject * retval__ = self__->object();
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Window.object: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Window.object: Unknown exception");
  }
}


/** void mimas::Window::resize(int w, int h)
 * include/mimas/Window.h:98
 */
static int Window_resize(lua_State *L) {
  try {
    Window *self__ = *((Window**)luaL_checkudata(L, 1, "mimas.Window"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Window in resize");
    int w = luaL_checkint(L, 2);
    int h = luaL_checkint(L, 3);
    self__->resize(w, h);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Window.resize: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Window.resize: Unknown exception");
  }
}


/** void mimas::Window::setHue(float hue)
 * include/mimas/Window.h:106
 */
static int Window_setHue(lua_State *L) {
  try {
    Window *self__ = *((Window**)luaL_checkudata(L, 1, "mimas.Window"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Window in setHue");
    float hue = luaL_checknumber(L, 2);
    self__->setHue(hue);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Window.setHue: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Window.setHue: Unknown exception");
  }
}


/** void mimas::Window::setName(const char *name)
 * include/mimas/Window.h:90
 */
static int Window_setName(lua_State *L) {
  try {
    Window *self__ = *((Window**)luaL_checkudata(L, 1, "mimas.Window"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Window in setName");
    const char *name = luaL_checkstring(L, 2);
    self__->setName(name);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Window.setName: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Window.setName: Unknown exception");
  }
}


/** void mimas::Window::setStyle(const char *text)
 * include/mimas/Window.h:102
 */
static int Window_setStyle(lua_State *L) {
  try {
    Window *self__ = *((Window**)luaL_checkudata(L, 1, "mimas.Window"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Window in setStyle");
    const char *text = luaL_checkstring(L, 2);
    self__->setStyle(text);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Window.setStyle: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Window.setStyle: Unknown exception");
  }
}


/** void mimas::Window::show()
 * include/mimas/Window.h:132
 */
static int Window_show(lua_State *L) {
  try {
    Window *self__ = *((Window**)luaL_checkudata(L, 1, "mimas.Window"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Window in show");
    self__->show();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Window.show: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Window.show: Unknown exception");
  }
}


/** void mimas::Window::update()
 * include/mimas/Window.h:115
 */
static int Window_update(lua_State *L) {
  try {
    Window *self__ = *((Window**)luaL_checkudata(L, 1, "mimas.Window"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Window in update");
    self__->update();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Window.update: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Window.update: Unknown exception");
  }
}


/** QWidget* mimas::Window::widget()
 * include/mimas/Window.h:73
 */
static int Window_widget(lua_State *L) {
  try {
    Window *self__ = *((Window**)luaL_checkudata(L, 1, "mimas.Window"));
    if (!self__) return luaL_error(L, "Using deleted mimas.Window in widget");
    QWidget * retval__ = self__->widget();
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Window.widget: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Window.widget: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Window_member_methods[] = {
  {"__newindex"        , Window___newindex},
  {"activateWindow"    , Window_activateWindow},
  {"close"             , Window_close},
  {"cssClass"          , Window_cssClass},
  {"hue"               , Window_hue},
  {"isVisible"         , Window_isVisible},
  {"move"              , Window_move},
  {"name"              , Window_name},
  {"object"            , Window_object},
  {"resize"            , Window_resize},
  {"setHue"            , Window_setHue},
  {"setName"           , Window_setName},
  {"setStyle"          , Window_setStyle},
  {"show"              , Window_show},
  {"update"            , Window_update},
  {"widget"            , Window_widget},
  {"__tostring"        , Window__tostring},
  {"__gc"              , Window_destructor},
  {"deleted"           , Window_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg Window_namespace_methods[] = {
  {"Window"            , Window_Window},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Window(lua_State *L) {
#else
extern "C" int luaopen_mimas_Window(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Window");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Window_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Window_namespace_methods);


	return 1;
}
