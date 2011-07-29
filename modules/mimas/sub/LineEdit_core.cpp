#include "mimas/LineEdit.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::LineEdit::LineEdit(lubyk::Worker *worker, const char *content=NULL, QWidget *parent=NULL)
 * include/mimas/LineEdit.h:61
 */
static int LineEdit_LineEdit(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    LineEdit * retval__;
    lubyk::Worker *worker = *((lubyk::Worker **)luaL_checkudata(L, 1, "lubyk.Worker"));
    if (top__ < 2) {
      retval__ = new LineEdit(worker);
    } else {
      const char *content = luaL_checkstring(L, 2);
      if (top__ < 3) {
        retval__ = new LineEdit(worker, content);
      } else {
        QWidget *parent = *((QWidget **)luaL_checkudata(L, 3, "mimas.QWidget"));
        retval__ = new LineEdit(worker, content, parent);
      }
    }
    lua_pushclass2<LineEdit>(L, retval__, "mimas.LineEdit");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.LineEdit.LineEdit: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.LineEdit.LineEdit: Unknown exception");
  }
}


/* ============================ Destructor       ====================== */

static int LineEdit_destructor(lua_State *L) {
  LineEdit **userdata = (LineEdit**)luaL_checkudata(L, 1, "mimas.LineEdit");

  
  // custom destructor
  if (*userdata) (*userdata)->dub_destroy();
  
  *userdata = NULL;
  return 0;
}




// test if class is deleted
static int LineEdit_deleted(lua_State *L) {
  LineEdit **userdata = (LineEdit**)luaL_checkudata(L, 1, "mimas.LineEdit");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int LineEdit__tostring(lua_State *L) {
  LineEdit **userdata = (LineEdit**)luaL_checkudata(L, 1, "mimas.LineEdit");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.LineEdit: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.LineEdit: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::LineEdit::__newindex(lua_State *L)
 * include/mimas/LineEdit.h:145
 */
static int LineEdit___newindex(lua_State *L) {
  try {
    LineEdit *self__ = *((LineEdit**)luaL_checkudata(L, 1, "mimas.LineEdit"));
    if (!self__) return luaL_error(L, "Using deleted mimas.LineEdit in __newindex");
    
    self__->__newindex(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.LineEdit.__newindex: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.LineEdit.__newindex: Unknown exception");
  }
}


/** QString mimas::LineEdit::cssClass() const 
 * include/mimas/LineEdit.h:73
 */
static int LineEdit_cssClass(lua_State *L) {
  try {
    LineEdit *self__ = *((LineEdit**)luaL_checkudata(L, 1, "mimas.LineEdit"));
    if (!self__) return luaL_error(L, "Using deleted mimas.LineEdit in cssClass");
    QString  retval__ = self__->cssClass();
    lua_pushclass<QString>(L, retval__, "mimas.QString");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.LineEdit.cssClass: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.LineEdit.cssClass: Unknown exception");
  }
}


/** float mimas::LineEdit::hue()
 * include/mimas/LineEdit.h:115
 */
static int LineEdit_hue(lua_State *L) {
  try {
    LineEdit *self__ = *((LineEdit**)luaL_checkudata(L, 1, "mimas.LineEdit"));
    if (!self__) return luaL_error(L, "Using deleted mimas.LineEdit in hue");
    float  retval__ = self__->hue();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.LineEdit.hue: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.LineEdit.hue: Unknown exception");
  }
}


/** void mimas::LineEdit::move(int x, int y)
 * include/mimas/LineEdit.h:98
 */
static int LineEdit_move(lua_State *L) {
  try {
    LineEdit *self__ = *((LineEdit**)luaL_checkudata(L, 1, "mimas.LineEdit"));
    if (!self__) return luaL_error(L, "Using deleted mimas.LineEdit in move");
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    self__->move(x, y);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.LineEdit.move: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.LineEdit.move: Unknown exception");
  }
}


/** LuaStackSize mimas::LineEdit::name(lua_State *L)
 * include/mimas/LineEdit.h:87
 */
static int LineEdit_name(lua_State *L) {
  try {
    LineEdit *self__ = *((LineEdit**)luaL_checkudata(L, 1, "mimas.LineEdit"));
    if (!self__) return luaL_error(L, "Using deleted mimas.LineEdit in name");
    
    LuaStackSize  retval__ = self__->name(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.LineEdit.name: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.LineEdit.name: Unknown exception");
  }
}


/** QObject* mimas::LineEdit::object()
 * include/mimas/LineEdit.h:81
 */
static int LineEdit_object(lua_State *L) {
  try {
    LineEdit *self__ = *((LineEdit**)luaL_checkudata(L, 1, "mimas.LineEdit"));
    if (!self__) return luaL_error(L, "Using deleted mimas.LineEdit in object");
    QObject * retval__ = self__->object();
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.LineEdit.object: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.LineEdit.object: Unknown exception");
  }
}


/** void mimas::LineEdit::resize(int w, int h)
 * include/mimas/LineEdit.h:102
 */
static int LineEdit_resize(lua_State *L) {
  try {
    LineEdit *self__ = *((LineEdit**)luaL_checkudata(L, 1, "mimas.LineEdit"));
    if (!self__) return luaL_error(L, "Using deleted mimas.LineEdit in resize");
    int w = luaL_checkint(L, 2);
    int h = luaL_checkint(L, 3);
    self__->resize(w, h);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.LineEdit.resize: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.LineEdit.resize: Unknown exception");
  }
}


/** void mimas::LineEdit::selectAll()
 * include/mimas/LineEdit.h:134
 */
static int LineEdit_selectAll(lua_State *L) {
  try {
    LineEdit *self__ = *((LineEdit**)luaL_checkudata(L, 1, "mimas.LineEdit"));
    if (!self__) return luaL_error(L, "Using deleted mimas.LineEdit in selectAll");
    self__->selectAll();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.LineEdit.selectAll: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.LineEdit.selectAll: Unknown exception");
  }
}


/** void mimas::LineEdit::setFocus()
 * include/mimas/LineEdit.h:138
 */
static int LineEdit_setFocus(lua_State *L) {
  try {
    LineEdit *self__ = *((LineEdit**)luaL_checkudata(L, 1, "mimas.LineEdit"));
    if (!self__) return luaL_error(L, "Using deleted mimas.LineEdit in setFocus");
    self__->setFocus();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.LineEdit.setFocus: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.LineEdit.setFocus: Unknown exception");
  }
}


/** void mimas::LineEdit::setHue(float hue)
 * include/mimas/LineEdit.h:110
 */
static int LineEdit_setHue(lua_State *L) {
  try {
    LineEdit *self__ = *((LineEdit**)luaL_checkudata(L, 1, "mimas.LineEdit"));
    if (!self__) return luaL_error(L, "Using deleted mimas.LineEdit in setHue");
    float hue = luaL_checknumber(L, 2);
    self__->setHue(hue);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.LineEdit.setHue: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.LineEdit.setHue: Unknown exception");
  }
}


/** void mimas::LineEdit::setName(const char *name)
 * include/mimas/LineEdit.h:94
 */
static int LineEdit_setName(lua_State *L) {
  try {
    LineEdit *self__ = *((LineEdit**)luaL_checkudata(L, 1, "mimas.LineEdit"));
    if (!self__) return luaL_error(L, "Using deleted mimas.LineEdit in setName");
    const char *name = luaL_checkstring(L, 2);
    self__->setName(name);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.LineEdit.setName: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.LineEdit.setName: Unknown exception");
  }
}


/** void mimas::LineEdit::setSelection(int start, int length)
 * include/mimas/LineEdit.h:130
 */
static int LineEdit_setSelection(lua_State *L) {
  try {
    LineEdit *self__ = *((LineEdit**)luaL_checkudata(L, 1, "mimas.LineEdit"));
    if (!self__) return luaL_error(L, "Using deleted mimas.LineEdit in setSelection");
    int start = luaL_checkint(L, 2);
    int length = luaL_checkint(L, 3);
    self__->setSelection(start, length);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.LineEdit.setSelection: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.LineEdit.setSelection: Unknown exception");
  }
}


/** void mimas::LineEdit::setStyle(const char *text)
 * include/mimas/LineEdit.h:106
 */
static int LineEdit_setStyle(lua_State *L) {
  try {
    LineEdit *self__ = *((LineEdit**)luaL_checkudata(L, 1, "mimas.LineEdit"));
    if (!self__) return luaL_error(L, "Using deleted mimas.LineEdit in setStyle");
    const char *text = luaL_checkstring(L, 2);
    self__->setStyle(text);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.LineEdit.setStyle: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.LineEdit.setStyle: Unknown exception");
  }
}


/** void mimas::LineEdit::setText(const char *text)
 * include/mimas/LineEdit.h:121
 */
static int LineEdit_setText(lua_State *L) {
  try {
    LineEdit *self__ = *((LineEdit**)luaL_checkudata(L, 1, "mimas.LineEdit"));
    if (!self__) return luaL_error(L, "Using deleted mimas.LineEdit in setText");
    const char *text = luaL_checkstring(L, 2);
    self__->setText(text);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.LineEdit.setText: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.LineEdit.setText: Unknown exception");
  }
}


/** const char* mimas::LineEdit::text() const 
 * include/mimas/LineEdit.h:126
 */
static int LineEdit_text(lua_State *L) {
  try {
    LineEdit *self__ = *((LineEdit**)luaL_checkudata(L, 1, "mimas.LineEdit"));
    if (!self__) return luaL_error(L, "Using deleted mimas.LineEdit in text");
    const char * retval__ = self__->text();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.LineEdit.text: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.LineEdit.text: Unknown exception");
  }
}


/** QWidget* mimas::LineEdit::widget()
 * include/mimas/LineEdit.h:77
 */
static int LineEdit_widget(lua_State *L) {
  try {
    LineEdit *self__ = *((LineEdit**)luaL_checkudata(L, 1, "mimas.LineEdit"));
    if (!self__) return luaL_error(L, "Using deleted mimas.LineEdit in widget");
    QWidget * retval__ = self__->widget();
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.LineEdit.widget: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.LineEdit.widget: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg LineEdit_member_methods[] = {
  {"__newindex"        , LineEdit___newindex},
  {"cssClass"          , LineEdit_cssClass},
  {"hue"               , LineEdit_hue},
  {"move"              , LineEdit_move},
  {"name"              , LineEdit_name},
  {"object"            , LineEdit_object},
  {"resize"            , LineEdit_resize},
  {"selectAll"         , LineEdit_selectAll},
  {"setFocus"          , LineEdit_setFocus},
  {"setHue"            , LineEdit_setHue},
  {"setName"           , LineEdit_setName},
  {"setSelection"      , LineEdit_setSelection},
  {"setStyle"          , LineEdit_setStyle},
  {"setText"           , LineEdit_setText},
  {"text"              , LineEdit_text},
  {"widget"            , LineEdit_widget},
  {"__tostring"        , LineEdit__tostring},
  {"__gc"              , LineEdit_destructor},
  {"deleted"           , LineEdit_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg LineEdit_namespace_methods[] = {
  {"LineEdit"          , LineEdit_LineEdit},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_LineEdit_core(lua_State *L) {
#else
extern "C" int luaopen_mimas_LineEdit_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.LineEdit");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, LineEdit_member_methods);
  // save meta-table in mimas.LineEdit_
  register_mt(L, "mimas", "LineEdit");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", LineEdit_namespace_methods);


	return 1;
}
