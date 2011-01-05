#include "mimas/Label.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */


/** mimas::Label::Label(const char *title=NULL)
 * include/mimas/Label.h:48
 */
static int Label_Label1(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Label * retval__;
    if (top__ < 1) {
      retval__ = new Label();
    } else {
      const char *title = luaL_checkstring(L, 1);
      retval__ = new Label(title);
    }
    lua_pushclass<Label>(L, retval__, "mimas.Label");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Label.Label: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Label.Label: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** mimas::Label::Label(const char *title, QWidget *parent)
 * include/mimas/Label.h:51
 */
static int Label_Label2(lua_State *L) {
  try {
    const char *title = luaL_checkstring(L, 1);
    QWidget *parent = *((QWidget **)luaL_checkudata(L, 2, "mimas.QWidget"));
    Label * retval__ = new Label(title, parent);
    lua_pushclass<Label>(L, retval__, "mimas.Label");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Label.Label: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Label.Label: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for Label(...) */
static int Label_Label(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TSTRING) {
    type__ = lua_type(L, 2);
    if (top__ < 2) {
      return Label_Label1(L);
    } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "mimas.QWidget")) {
      return Label_Label2(L);
    } else {
      // use any to raise errors
      return Label_Label2(L);
    }
  } else {
    // use any to raise errors
    return Label_Label2(L);
  }
}

/* ============================ Destructor       ====================== */

static int Label_destructor(lua_State *L) {
  Label **userdata = (Label**)luaL_checkudata(L, 1, "mimas.Label");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Label__tostring(lua_State *L) {
  Label **userdata = (Label**)luaL_checkudata(L, 1, "mimas.Label");
  
  lua_pushfstring(L, "<mimas.Label: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** QString mimas::Label::cssClass() const 
 * include/mimas/Label.h:56
 */
static int Label_cssClass(lua_State *L) {
  try {
    Label *self__ = *((Label**)luaL_checkudata(L, 1, "mimas.Label"));
    QString  retval__ = self__->cssClass();
    lua_pushclass<QString>(L, retval__, "mimas.QString");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Label.cssClass: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Label.cssClass: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** float mimas::Label::hue()
 * include/mimas/Label.h:97
 */
static int Label_hue(lua_State *L) {
  try {
    Label *self__ = *((Label**)luaL_checkudata(L, 1, "mimas.Label"));
    float  retval__ = self__->hue();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Label.hue: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Label.hue: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Label::move(int x, int y)
 * include/mimas/Label.h:81
 */
static int Label_move(lua_State *L) {
  try {
    Label *self__ = *((Label**)luaL_checkudata(L, 1, "mimas.Label"));
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    self__->move(x, y);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Label.move: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Label.move: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** LuaStackSize mimas::Label::name(lua_State *L)
 * include/mimas/Label.h:70
 */
static int Label_name(lua_State *L) {
  try {
    Label *self__ = *((Label**)luaL_checkudata(L, 1, "mimas.Label"));
    
    LuaStackSize  retval__ = self__->name(L);
    return retval__;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Label.name: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Label.name: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** QObject* mimas::Label::object()
 * include/mimas/Label.h:64
 */
static int Label_object(lua_State *L) {
  try {
    Label *self__ = *((Label**)luaL_checkudata(L, 1, "mimas.Label"));
    QObject * retval__ = self__->object();
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Label.object: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Label.object: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Label::resize(int w, int h)
 * include/mimas/Label.h:85
 */
static int Label_resize(lua_State *L) {
  try {
    Label *self__ = *((Label**)luaL_checkudata(L, 1, "mimas.Label"));
    int w = luaL_checkint(L, 2);
    int h = luaL_checkint(L, 3);
    self__->resize(w, h);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Label.resize: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Label.resize: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Label::setHue(float hue)
 * include/mimas/Label.h:93
 */
static int Label_setHue(lua_State *L) {
  try {
    Label *self__ = *((Label**)luaL_checkudata(L, 1, "mimas.Label"));
    float hue = luaL_checknumber(L, 2);
    self__->setHue(hue);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Label.setHue: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Label.setHue: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Label::setName(const char *name)
 * include/mimas/Label.h:77
 */
static int Label_setName(lua_State *L) {
  try {
    Label *self__ = *((Label**)luaL_checkudata(L, 1, "mimas.Label"));
    const char *name = luaL_checkstring(L, 2);
    self__->setName(name);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Label.setName: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Label.setName: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Label::setStyle(const char *text)
 * include/mimas/Label.h:89
 */
static int Label_setStyle(lua_State *L) {
  try {
    Label *self__ = *((Label**)luaL_checkudata(L, 1, "mimas.Label"));
    const char *text = luaL_checkstring(L, 2);
    self__->setStyle(text);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Label.setStyle: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Label.setStyle: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Label::setText(const char *text)
 * include/mimas/Label.h:103
 */
static int Label_setText(lua_State *L) {
  try {
    Label *self__ = *((Label**)luaL_checkudata(L, 1, "mimas.Label"));
    const char *text = luaL_checkstring(L, 2);
    self__->setText(text);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Label.setText: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Label.setText: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** QWidget* mimas::Label::widget()
 * include/mimas/Label.h:60
 */
static int Label_widget(lua_State *L) {
  try {
    Label *self__ = *((Label**)luaL_checkudata(L, 1, "mimas.Label"));
    QWidget * retval__ = self__->widget();
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Label.widget: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Label.widget: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Label_member_methods[] = {
  {"cssClass"          , Label_cssClass},
  {"hue"               , Label_hue},
  {"move"              , Label_move},
  {"name"              , Label_name},
  {"object"            , Label_object},
  {"resize"            , Label_resize},
  {"setHue"            , Label_setHue},
  {"setName"           , Label_setName},
  {"setStyle"          , Label_setStyle},
  {"setText"           , Label_setText},
  {"widget"            , Label_widget},
  {"__tostring"        , Label__tostring},
  {"__gc"              , Label_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Label_namespace_methods[] = {
  {"Label"             , Label_Label},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Label_core(lua_State *L) {
#else
extern "C" int luaopen_mimas_Label_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Label");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Label_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Label_namespace_methods);


	return 1;
}
