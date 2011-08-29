#include "mimas/Label.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */


/** mimas::Label::Label(const char *title=NULL)
 * include/mimas/Label.h:50
 */
static int Label_Label1(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Label * retval__;
    if (top__ < 1) {
      retval__ = new Label();
    } else {
      const char *title = dubL_checkstring(L, 1);
      retval__ = new Label(title);
    }
    lua_pushclass2<Label>(L, retval__, "mimas.Label");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Label.Label: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Label.Label: Unknown exception");
  }
  return lua_error(L);
}



/** mimas::Label::Label(const char *title, QWidget *parent)
 * include/mimas/Label.h:54
 */
static int Label_Label2(lua_State *L) {
  try {
    const char *title = dubL_checkstring(L, 1);
    QWidget *parent = *((QWidget **)dubL_checksdata(L, 2, "mimas.QWidget"));
    Label * retval__ = new Label(title, parent);
    lua_pushclass2<Label>(L, retval__, "mimas.Label");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Label.Label: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Label.Label: Unknown exception");
  }
  return lua_error(L);
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
  } else if (top__ < 1) {
    return Label_Label1(L);
  } else {
    // use any to raise errors
    return Label_Label1(L);
  }
}


/* ============================ Destructor       ====================== */

static int Label_destructor(lua_State *L) {
  Label **userdata = (Label**)dubL_checksdata_n(L, 1, "mimas.Label");

  
  // custom destructor
  if (*userdata) (*userdata)->luaDestroy();
  
  *userdata = NULL;
  return 0;
}




// test if class is deleted
static int Label_deleted(lua_State *L) {
  Label **userdata = (Label**)dubL_checksdata_n(L, 1, "mimas.Label");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Label__tostring(lua_State *L) {
  Label **userdata = (Label**)dubL_checksdata_n(L, 1, "mimas.Label");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.Label: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.Label: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** QString mimas::Label::cssClass() const 
 * include/mimas/Label.h:62
 */
static int Label_cssClass(lua_State *L) {
  try {
    Label *self__ = *((Label**)dubL_checksdata(L, 1, "mimas.Label"));
    if (!self__) throw dub::Exception("Using deleted mimas.Label in cssClass");
    QString  retval__ = self__->cssClass();
    lua_pushclass<QString>(L, retval__, "mimas.QString");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Label.cssClass: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Label.cssClass: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::Label::height()
 * include/mimas/Label.h:107
 */
static int Label_height(lua_State *L) {
  try {
    Label *self__ = *((Label**)dubL_checksdata(L, 1, "mimas.Label"));
    if (!self__) throw dub::Exception("Using deleted mimas.Label in height");
    int  retval__ = self__->height();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Label.height: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Label.height: Unknown exception");
  }
  return lua_error(L);
}



/** float mimas::Label::hue()
 * include/mimas/Label.h:124
 */
static int Label_hue(lua_State *L) {
  try {
    Label *self__ = *((Label**)dubL_checksdata(L, 1, "mimas.Label"));
    if (!self__) throw dub::Exception("Using deleted mimas.Label in hue");
    float  retval__ = self__->hue();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Label.hue: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Label.hue: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Label::move(int x, int y)
 * include/mimas/Label.h:87
 */
static int Label_move(lua_State *L) {
  try {
    Label *self__ = *((Label**)dubL_checksdata(L, 1, "mimas.Label"));
    if (!self__) throw dub::Exception("Using deleted mimas.Label in move");
    int x = dubL_checkint(L, 2);
    int y = dubL_checkint(L, 3);
    self__->move(x, y);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Label.move: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Label.move: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::Label::name(lua_State *L)
 * include/mimas/Label.h:76
 */
static int Label_name(lua_State *L) {
  try {
    Label *self__ = *((Label**)dubL_checksdata(L, 1, "mimas.Label"));
    if (!self__) throw dub::Exception("Using deleted mimas.Label in name");
    
    LuaStackSize  retval__ = self__->name(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Label.name: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Label.name: Unknown exception");
  }
  return lua_error(L);
}



/** QObject* mimas::Label::object()
 * include/mimas/Label.h:70
 */
static int Label_object(lua_State *L) {
  try {
    Label *self__ = *((Label**)dubL_checksdata(L, 1, "mimas.Label"));
    if (!self__) throw dub::Exception("Using deleted mimas.Label in object");
    QObject * retval__ = self__->object();
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Label.object: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Label.object: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Label::resize(int w, int h)
 * include/mimas/Label.h:91
 */
static int Label_resize(lua_State *L) {
  try {
    Label *self__ = *((Label**)dubL_checksdata(L, 1, "mimas.Label"));
    if (!self__) throw dub::Exception("Using deleted mimas.Label in resize");
    int w = dubL_checkint(L, 2);
    int h = dubL_checkint(L, 3);
    self__->resize(w, h);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Label.resize: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Label.resize: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Label::setHue(float hue)
 * include/mimas/Label.h:119
 */
static int Label_setHue(lua_State *L) {
  try {
    Label *self__ = *((Label**)dubL_checksdata(L, 1, "mimas.Label"));
    if (!self__) throw dub::Exception("Using deleted mimas.Label in setHue");
    float hue = dubL_checknumber(L, 2);
    self__->setHue(hue);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Label.setHue: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Label.setHue: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Label::setName(const char *name)
 * include/mimas/Label.h:83
 */
static int Label_setName(lua_State *L) {
  try {
    Label *self__ = *((Label**)dubL_checksdata(L, 1, "mimas.Label"));
    if (!self__) throw dub::Exception("Using deleted mimas.Label in setName");
    const char *name = dubL_checkstring(L, 2);
    self__->setName(name);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Label.setName: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Label.setName: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Label::setStyle(const char *text)
 * include/mimas/Label.h:111
 */
static int Label_setStyle(lua_State *L) {
  try {
    Label *self__ = *((Label**)dubL_checksdata(L, 1, "mimas.Label"));
    if (!self__) throw dub::Exception("Using deleted mimas.Label in setStyle");
    const char *text = dubL_checkstring(L, 2);
    self__->setStyle(text);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Label.setStyle: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Label.setStyle: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Label::setStyleSheet(const char *text)
 * include/mimas/Label.h:115
 */
static int Label_setStyleSheet(lua_State *L) {
  try {
    Label *self__ = *((Label**)dubL_checksdata(L, 1, "mimas.Label"));
    if (!self__) throw dub::Exception("Using deleted mimas.Label in setStyleSheet");
    const char *text = dubL_checkstring(L, 2);
    self__->setStyleSheet(text);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Label.setStyleSheet: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Label.setStyleSheet: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Label::setText(const char *text)
 * include/mimas/Label.h:130
 */
static int Label_setText(lua_State *L) {
  try {
    Label *self__ = *((Label**)dubL_checksdata(L, 1, "mimas.Label"));
    if (!self__) throw dub::Exception("Using deleted mimas.Label in setText");
    const char *text = dubL_checkstring(L, 2);
    self__->setText(text);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Label.setText: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Label.setText: Unknown exception");
  }
  return lua_error(L);
}



/** QWidget* mimas::Label::widget()
 * include/mimas/Label.h:66
 */
static int Label_widget(lua_State *L) {
  try {
    Label *self__ = *((Label**)dubL_checksdata(L, 1, "mimas.Label"));
    if (!self__) throw dub::Exception("Using deleted mimas.Label in widget");
    QWidget * retval__ = self__->widget();
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Label.widget: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Label.widget: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::Label::width()
 * include/mimas/Label.h:103
 */
static int Label_width(lua_State *L) {
  try {
    Label *self__ = *((Label**)dubL_checksdata(L, 1, "mimas.Label"));
    if (!self__) throw dub::Exception("Using deleted mimas.Label in width");
    int  retval__ = self__->width();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Label.width: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Label.width: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::Label::x()
 * include/mimas/Label.h:95
 */
static int Label_x(lua_State *L) {
  try {
    Label *self__ = *((Label**)dubL_checksdata(L, 1, "mimas.Label"));
    if (!self__) throw dub::Exception("Using deleted mimas.Label in x");
    int  retval__ = self__->x();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Label.x: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Label.x: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::Label::y()
 * include/mimas/Label.h:99
 */
static int Label_y(lua_State *L) {
  try {
    Label *self__ = *((Label**)dubL_checksdata(L, 1, "mimas.Label"));
    if (!self__) throw dub::Exception("Using deleted mimas.Label in y");
    int  retval__ = self__->y();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "mimas.Label.y: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "mimas.Label.y: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Label_member_methods[] = {
  {"cssClass"          , Label_cssClass},
  {"height"            , Label_height},
  {"hue"               , Label_hue},
  {"move"              , Label_move},
  {"name"              , Label_name},
  {"object"            , Label_object},
  {"resize"            , Label_resize},
  {"setHue"            , Label_setHue},
  {"setName"           , Label_setName},
  {"setStyle"          , Label_setStyle},
  {"setStyleSheet"     , Label_setStyleSheet},
  {"setText"           , Label_setText},
  {"widget"            , Label_widget},
  {"width"             , Label_width},
  {"x"                 , Label_x},
  {"y"                 , Label_y},
  {"__tostring"        , Label__tostring},
  {"__gc"              , Label_destructor},
  {"deleted"           , Label_deleted},
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
  // save meta-table in mimas.Label_
  register_mt(L, "mimas", "Label");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Label_namespace_methods);


	return 1;
}
