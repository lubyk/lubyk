#include "mimas/PushButton.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */


/** mimas::PushButton::PushButton(const char *title=NULL)
 * include/mimas/PushButton.h:53
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
    lua_pushclass<PushButton>(L, retval__, "mimas.PushButton");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.PushButton.PushButton: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.PushButton.PushButton: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** mimas::PushButton::PushButton(const char *title, QWidget *parent)
 * include/mimas/PushButton.h:56
 */
static int PushButton_PushButton2(lua_State *L) {
  try {
    const char *title = luaL_checkstring(L, 1);
    QWidget *parent = *((QWidget **)luaL_checkudata(L, 2, "mimas.QWidget"));
    PushButton * retval__ = new PushButton(title, parent);
    lua_pushclass<PushButton>(L, retval__, "mimas.PushButton");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.PushButton.PushButton: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.PushButton.PushButton: Unknown exception");
    lua_error(L);
    return 0;
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
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int PushButton__tostring(lua_State *L) {
  PushButton **userdata = (PushButton**)luaL_checkudata(L, 1, "mimas.PushButton");
  
  lua_pushfstring(L, "<mimas.PushButton: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** QString mimas::PushButton::cssClass() const 
 * include/mimas/PushButton.h:61
 */
static int PushButton_cssClass(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    QString  retval__ = self__->cssClass();
    lua_pushclass<QString>(L, retval__, "mimas.QString");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.PushButton.cssClass: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.PushButton.cssClass: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** float mimas::PushButton::hue()
 * include/mimas/PushButton.h:102
 */
static int PushButton_hue(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    float  retval__ = self__->hue();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.PushButton.hue: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.PushButton.hue: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::PushButton::move(int x, int y)
 * include/mimas/PushButton.h:86
 */
static int PushButton_move(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    self__->move(x, y);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.PushButton.move: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.PushButton.move: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** LuaStackSize mimas::PushButton::name(lua_State *L)
 * include/mimas/PushButton.h:75
 */
static int PushButton_name(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    
    LuaStackSize  retval__ = self__->name(L);
    return retval__;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.PushButton.name: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.PushButton.name: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** QObject* mimas::PushButton::object()
 * include/mimas/PushButton.h:69
 */
static int PushButton_object(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    QObject * retval__ = self__->object();
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.PushButton.object: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.PushButton.object: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::PushButton::resize(int w, int h)
 * include/mimas/PushButton.h:90
 */
static int PushButton_resize(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    int w = luaL_checkint(L, 2);
    int h = luaL_checkint(L, 3);
    self__->resize(w, h);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.PushButton.resize: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.PushButton.resize: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::PushButton::setHue(float hue)
 * include/mimas/PushButton.h:98
 */
static int PushButton_setHue(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    float hue = luaL_checknumber(L, 2);
    self__->setHue(hue);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.PushButton.setHue: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.PushButton.setHue: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::PushButton::setName(const char *name)
 * include/mimas/PushButton.h:82
 */
static int PushButton_setName(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    const char *name = luaL_checkstring(L, 2);
    self__->setName(name);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.PushButton.setName: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.PushButton.setName: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::PushButton::setStyle(const char *text)
 * include/mimas/PushButton.h:94
 */
static int PushButton_setStyle(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    const char *text = luaL_checkstring(L, 2);
    self__->setStyle(text);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.PushButton.setStyle: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.PushButton.setStyle: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** QWidget* mimas::PushButton::widget()
 * include/mimas/PushButton.h:65
 */
static int PushButton_widget(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    QWidget * retval__ = self__->widget();
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.PushButton.widget: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.PushButton.widget: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg PushButton_member_methods[] = {
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
