#include "mimas/Widget.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Widget::Widget()
 * include/mimas/Widget.h:47
 */
static int Widget_Widget(lua_State *L) {
  try {
    Widget * retval__ = new Widget();
    lua_pushclass<Widget>(L, retval__, "mimas.Widget");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Widget.Widget: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Widget.Widget: Unknown exception");
    lua_error(L);
    return 0;
  }
}

/* ============================ Destructor       ====================== */

static int Widget_destructor(lua_State *L) {
  Widget **userdata = (Widget**)luaL_checkudata(L, 1, "mimas.Widget");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Widget__tostring(lua_State *L) {
  Widget **userdata = (Widget**)luaL_checkudata(L, 1, "mimas.Widget");
  
  lua_pushfstring(L, "<mimas.Widget: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::Widget::activateWindow()
 * include/mimas/Widget.h:103
 */
static int Widget_activateWindow(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    self__->activateWindow();
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Widget.activateWindow: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Widget.activateWindow: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** QString mimas::Widget::cssClass() const 
 * include/mimas/Widget.h:52
 */
static int Widget_cssClass(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    QString  retval__ = self__->cssClass();
    lua_pushclass<QString>(L, retval__, "mimas.QString");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Widget.cssClass: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Widget.cssClass: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** float mimas::Widget::hue()
 * include/mimas/Widget.h:93
 */
static int Widget_hue(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    float  retval__ = self__->hue();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Widget.hue: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Widget.hue: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Widget::move(int x, int y)
 * include/mimas/Widget.h:77
 */
static int Widget_move(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    self__->move(x, y);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Widget.move: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Widget.move: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** LuaStackSize mimas::Widget::name(lua_State *L)
 * include/mimas/Widget.h:66
 */
static int Widget_name(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    
    LuaStackSize  retval__ = self__->name(L);
    return retval__;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Widget.name: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Widget.name: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** QObject* mimas::Widget::object()
 * include/mimas/Widget.h:60
 */
static int Widget_object(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    QObject * retval__ = self__->object();
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Widget.object: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Widget.object: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Widget::resize(int w, int h)
 * include/mimas/Widget.h:81
 */
static int Widget_resize(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    int w = luaL_checkint(L, 2);
    int h = luaL_checkint(L, 3);
    self__->resize(w, h);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Widget.resize: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Widget.resize: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Widget::setHue(float hue)
 * include/mimas/Widget.h:89
 */
static int Widget_setHue(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    float hue = luaL_checknumber(L, 2);
    self__->setHue(hue);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Widget.setHue: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Widget.setHue: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Widget::setName(const char *name)
 * include/mimas/Widget.h:73
 */
static int Widget_setName(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    const char *name = luaL_checkstring(L, 2);
    self__->setName(name);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Widget.setName: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Widget.setName: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Widget::setStyle(const char *text)
 * include/mimas/Widget.h:85
 */
static int Widget_setStyle(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    const char *text = luaL_checkstring(L, 2);
    self__->setStyle(text);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Widget.setStyle: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Widget.setStyle: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Widget::show()
 * include/mimas/Widget.h:99
 */
static int Widget_show(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    self__->show();
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Widget.show: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Widget.show: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** QWidget* mimas::Widget::widget()
 * include/mimas/Widget.h:56
 */
static int Widget_widget(lua_State *L) {
  try {
    Widget *self__ = *((Widget**)luaL_checkudata(L, 1, "mimas.Widget"));
    QWidget * retval__ = self__->widget();
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Widget.widget: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Widget.widget: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Widget_member_methods[] = {
  {"activateWindow"    , Widget_activateWindow},
  {"cssClass"          , Widget_cssClass},
  {"hue"               , Widget_hue},
  {"move"              , Widget_move},
  {"name"              , Widget_name},
  {"object"            , Widget_object},
  {"resize"            , Widget_resize},
  {"setHue"            , Widget_setHue},
  {"setName"           , Widget_setName},
  {"setStyle"          , Widget_setStyle},
  {"show"              , Widget_show},
  {"widget"            , Widget_widget},
  {"__tostring"        , Widget__tostring},
  {"__gc"              , Widget_destructor},
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

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Widget_namespace_methods);


	return 1;
}
