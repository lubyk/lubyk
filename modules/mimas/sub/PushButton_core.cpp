#include "mimas/PushButton.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */


/** mimas::PushButton::PushButton(const char *title=NULL)
 * include/mimas/PushButton.h:55
 */
static int PushButton_PushButton1(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    PushButton * retval__;
    if (top__ < 1) {
      retval__ = new PushButton();
    } else {
      const char *title = luaL_checkstring(L, 1);
      retval__ = new PushButton(title);
    }
    lua_pushclass2<PushButton>(L, retval__, "mimas.PushButton");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.PushButton.PushButton: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.PushButton.PushButton: Unknown exception");
  }
}


/** mimas::PushButton::PushButton(const char *title, QWidget *parent)
 * include/mimas/PushButton.h:58
 */
static int PushButton_PushButton2(lua_State *L) {
  try {
    const char *title = luaL_checkstring(L, 1);
    QWidget *parent = *((QWidget **)luaL_checkudata(L, 2, "mimas.QWidget"));
    PushButton * retval__ = new PushButton(title, parent);
    lua_pushclass2<PushButton>(L, retval__, "mimas.PushButton");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.PushButton.PushButton: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.PushButton.PushButton: Unknown exception");
  }
}



/** Overloaded function chooser for PushButton(...) */
static int PushButton_PushButton(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TSTRING) {
    type__ = lua_type(L, 2);
    if (top__ < 2) {
      return PushButton_PushButton1(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "mimas.QWidget")) {
      return PushButton_PushButton2(L);
    } else {
      // use any to raise errors
      return PushButton_PushButton2(L);
    }
  } else {
    // use any to raise errors
    return PushButton_PushButton2(L);
  }
}


/* ============================ Destructor       ====================== */

static int PushButton_destructor(lua_State *L) {
  PushButton **userdata = (PushButton**)luaL_checkudata(L, 1, "mimas.PushButton");

  
  // custom destructor
  if (*userdata) (*userdata)->dub_destroy();
  
  *userdata = NULL;
  return 0;
}




// test if class is deleted
static int PushButton_deleted(lua_State *L) {
  PushButton **userdata = (PushButton**)luaL_checkudata(L, 1, "mimas.PushButton");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int PushButton__tostring(lua_State *L) {
  PushButton **userdata = (PushButton**)luaL_checkudata(L, 1, "mimas.PushButton");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.PushButton: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.PushButton: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::PushButton::add_to_env(lua_State *L)
 * include/mimas/PushButton.h:117
 */
static int PushButton_add_to_env(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    if (!self__) return luaL_error(L, "Using deleted mimas.PushButton in add_to_env");
    lua_State *L = *((lua_State **)luaL_checkudata(L, 2, "mimas.lua_State"));
    self__->add_to_env(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.PushButton.add_to_env: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.PushButton.add_to_env: Unknown exception");
  }
}


/** QString mimas::PushButton::cssClass() const 
 * include/mimas/PushButton.h:66
 */
static int PushButton_cssClass(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    if (!self__) return luaL_error(L, "Using deleted mimas.PushButton in cssClass");
    QString  retval__ = self__->cssClass();
    lua_pushclass<QString>(L, retval__, "mimas.QString");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.PushButton.cssClass: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.PushButton.cssClass: Unknown exception");
  }
}


/** float mimas::PushButton::hue()
 * include/mimas/PushButton.h:108
 */
static int PushButton_hue(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    if (!self__) return luaL_error(L, "Using deleted mimas.PushButton in hue");
    float  retval__ = self__->hue();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.PushButton.hue: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.PushButton.hue: Unknown exception");
  }
}


/** void mimas::PushButton::move(int x, int y)
 * include/mimas/PushButton.h:91
 */
static int PushButton_move(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    if (!self__) return luaL_error(L, "Using deleted mimas.PushButton in move");
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    self__->move(x, y);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.PushButton.move: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.PushButton.move: Unknown exception");
  }
}


/** LuaStackSize mimas::PushButton::name(lua_State *L)
 * include/mimas/PushButton.h:80
 */
static int PushButton_name(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    if (!self__) return luaL_error(L, "Using deleted mimas.PushButton in name");
    lua_State *L = *((lua_State **)luaL_checkudata(L, 2, "mimas.lua_State"));
    LuaStackSize  retval__ = self__->name(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.PushButton.name: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.PushButton.name: Unknown exception");
  }
}


/** QObject* mimas::PushButton::object()
 * include/mimas/PushButton.h:74
 */
static int PushButton_object(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    if (!self__) return luaL_error(L, "Using deleted mimas.PushButton in object");
    QObject * retval__ = self__->object();
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.PushButton.object: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.PushButton.object: Unknown exception");
  }
}


/** void mimas::PushButton::resize(int w, int h)
 * include/mimas/PushButton.h:95
 */
static int PushButton_resize(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    if (!self__) return luaL_error(L, "Using deleted mimas.PushButton in resize");
    int w = luaL_checkint(L, 2);
    int h = luaL_checkint(L, 3);
    self__->resize(w, h);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.PushButton.resize: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.PushButton.resize: Unknown exception");
  }
}


/** void mimas::PushButton::setHue(float hue)
 * include/mimas/PushButton.h:103
 */
static int PushButton_setHue(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    if (!self__) return luaL_error(L, "Using deleted mimas.PushButton in setHue");
    float hue = luaL_checknumber(L, 2);
    self__->setHue(hue);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.PushButton.setHue: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.PushButton.setHue: Unknown exception");
  }
}


/** void mimas::PushButton::setName(const char *name)
 * include/mimas/PushButton.h:87
 */
static int PushButton_setName(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    if (!self__) return luaL_error(L, "Using deleted mimas.PushButton in setName");
    const char *name = luaL_checkstring(L, 2);
    self__->setName(name);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.PushButton.setName: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.PushButton.setName: Unknown exception");
  }
}


/** void mimas::PushButton::setStyle(const char *text)
 * include/mimas/PushButton.h:99
 */
static int PushButton_setStyle(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    if (!self__) return luaL_error(L, "Using deleted mimas.PushButton in setStyle");
    const char *text = luaL_checkstring(L, 2);
    self__->setStyle(text);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.PushButton.setStyle: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.PushButton.setStyle: Unknown exception");
  }
}


/** QWidget* mimas::PushButton::widget()
 * include/mimas/PushButton.h:70
 */
static int PushButton_widget(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    if (!self__) return luaL_error(L, "Using deleted mimas.PushButton in widget");
    QWidget * retval__ = self__->widget();
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.PushButton.widget: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.PushButton.widget: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg PushButton_member_methods[] = {
  {"add_to_env"        , PushButton_add_to_env},
  {"cssClass"          , PushButton_cssClass},
  {"hue"               , PushButton_hue},
  {"move"              , PushButton_move},
  {"name"              , PushButton_name},
  {"object"            , PushButton_object},
  {"resize"            , PushButton_resize},
  {"setHue"            , PushButton_setHue},
  {"setName"           , PushButton_setName},
  {"setStyle"          , PushButton_setStyle},
  {"widget"            , PushButton_widget},
  {"__tostring"        , PushButton__tostring},
  {"__gc"              , PushButton_destructor},
  {"deleted"           , PushButton_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg PushButton_namespace_methods[] = {
  {"PushButton"        , PushButton_PushButton},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_PushButton_core(lua_State *L) {
#else
extern "C" int luaopen_mimas_PushButton_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.PushButton");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, PushButton_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "mimas", PushButton_namespace_methods);


	return 1;
}
