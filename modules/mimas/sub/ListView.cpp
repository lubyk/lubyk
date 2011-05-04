#include "mimas/ListView.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::ListView::ListView(lubyk::Worker *worker)
 * include/mimas/ListView.h:76
 */
static int ListView_ListView(lua_State *L) {
  try {
    lubyk::Worker *worker = *((lubyk::Worker **)luaL_checkudata(L, 1, "lubyk.Worker"));
    ListView * retval__ = new ListView(worker);
    lua_pushclass2<ListView>(L, retval__, "mimas.ListView");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.ListView: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.ListView: Unknown exception");
  }
}


/* ============================ Destructor       ====================== */

static int ListView_destructor(lua_State *L) {
  ListView **userdata = (ListView**)luaL_checkudata(L, 1, "mimas.ListView");

  
  // custom destructor
  if (*userdata) (*userdata)->dub_destroy();
  
  *userdata = NULL;
  return 0;
}




// test if class is deleted
static int ListView_deleted(lua_State *L) {
  ListView **userdata = (ListView**)luaL_checkudata(L, 1, "mimas.ListView");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int ListView__tostring(lua_State *L) {
  ListView **userdata = (ListView**)luaL_checkudata(L, 1, "mimas.ListView");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.ListView: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.ListView: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::ListView::__newindex(lua_State *L)
 * include/mimas/ListView.h:261
 */
static int ListView___newindex(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in __newindex");
    
    self__->__newindex(L);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.__newindex: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.__newindex: Unknown exception");
  }
}


/** bool mimas::ListView::close()
 * include/mimas/ListView.h:185
 */
static int ListView_close(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in close");
    bool  retval__ = self__->close();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.close: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.close: Unknown exception");
  }
}


/** QString mimas::ListView::cssClass() const 
 * include/mimas/ListView.h:93
 */
static int ListView_cssClass(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in cssClass");
    QString  retval__ = self__->cssClass();
    lua_pushclass<QString>(L, retval__, "mimas.QString");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.cssClass: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.cssClass: Unknown exception");
  }
}


/** void mimas::ListView::globalMove(float x, float y)
 * include/mimas/ListView.h:216
 */
static int ListView_globalMove(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in globalMove");
    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    self__->globalMove(x, y);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.globalMove: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.globalMove: Unknown exception");
  }
}


/** LuaStackSize mimas::ListView::globalPosition(lua_State *L)
 * include/mimas/ListView.h:207
 */
static int ListView_globalPosition(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in globalPosition");
    
    LuaStackSize  retval__ = self__->globalPosition(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.globalPosition: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.globalPosition: Unknown exception");
  }
}


/** void mimas::ListView::hide()
 * include/mimas/ListView.h:199
 */
static int ListView_hide(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in hide");
    self__->hide();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.hide: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.hide: Unknown exception");
  }
}


/** float mimas::ListView::hue()
 * include/mimas/ListView.h:137
 */
static int ListView_hue(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in hue");
    float  retval__ = self__->hue();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.hue: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.hue: Unknown exception");
  }
}


/** LuaStackSize mimas::ListView::indexAt(float x, float y, lua_State *L)
 * include/mimas/ListView.h:239
 */
static int ListView_indexAt(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in indexAt");
    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    
    LuaStackSize  retval__ = self__->indexAt(x, y, L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.indexAt: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.indexAt: Unknown exception");
  }
}


/** bool mimas::ListView::isVisible() const 
 * include/mimas/ListView.h:190
 */
static int ListView_isVisible(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in isVisible");
    bool  retval__ = self__->isVisible();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.isVisible: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.isVisible: Unknown exception");
  }
}


/** void mimas::ListView::lower()
 * include/mimas/ListView.h:223
 */
static int ListView_lower(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in lower");
    self__->lower();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.lower: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.lower: Unknown exception");
  }
}


/** void mimas::ListView::move(int x, int y)
 * include/mimas/ListView.h:118
 */
static int ListView_move(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in move");
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    self__->move(x, y);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.move: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.move: Unknown exception");
  }
}


/** LuaStackSize mimas::ListView::name(lua_State *L)
 * include/mimas/ListView.h:107
 */
static int ListView_name(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in name");
    
    LuaStackSize  retval__ = self__->name(L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.name: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.name: Unknown exception");
  }
}


/** QObject* mimas::ListView::object()
 * include/mimas/ListView.h:101
 */
static int ListView_object(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in object");
    QObject * retval__ = self__->object();
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.object: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.object: Unknown exception");
  }
}


/** void mimas::ListView::raise()
 * include/mimas/ListView.h:229
 */
static int ListView_raise(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in raise");
    self__->raise();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.raise: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.raise: Unknown exception");
  }
}


/** void mimas::ListView::resize(int w, int h)
 * include/mimas/ListView.h:123
 */
static int ListView_resize(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in resize");
    int w = luaL_checkint(L, 2);
    int h = luaL_checkint(L, 3);
    self__->resize(w, h);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.resize: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.resize: Unknown exception");
  }
}


/** void mimas::ListView::setHue(float hue)
 * include/mimas/ListView.h:132
 */
static int ListView_setHue(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in setHue");
    float hue = luaL_checknumber(L, 2);
    self__->setHue(hue);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.setHue: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.setHue: Unknown exception");
  }
}


/** void mimas::ListView::setMinimumSize(float w, float h)
 * include/mimas/ListView.h:172
 */
static int ListView_setMinimumSize(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in setMinimumSize");
    float w = luaL_checknumber(L, 2);
    float h = luaL_checknumber(L, 3);
    self__->setMinimumSize(w, h);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.setMinimumSize: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.setMinimumSize: Unknown exception");
  }
}


/** void mimas::ListView::setMouseTracking(bool enable)
 * include/mimas/ListView.h:179
 */
static int ListView_setMouseTracking(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in setMouseTracking");
    bool enable = lua_toboolean(L, 2);
    self__->setMouseTracking(enable);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.setMouseTracking: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.setMouseTracking: Unknown exception");
  }
}


/** void mimas::ListView::setName(const char *name)
 * include/mimas/ListView.h:114
 */
static int ListView_setName(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in setName");
    const char *name = luaL_checkstring(L, 2);
    self__->setName(name);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.setName: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.setName: Unknown exception");
  }
}


/** void mimas::ListView::setSizeHint(float w, float h)
 * include/mimas/ListView.h:157
 */
static int ListView_setSizeHint(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in setSizeHint");
    float w = luaL_checknumber(L, 2);
    float h = luaL_checknumber(L, 3);
    self__->setSizeHint(w, h);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.setSizeHint: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.setSizeHint: Unknown exception");
  }
}


/** void mimas::ListView::setSizePolicy(int horizontal, int vertical)
 * include/mimas/ListView.h:165
 */
static int ListView_setSizePolicy(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in setSizePolicy");
    int horizontal = luaL_checkint(L, 2);
    int vertical = luaL_checkint(L, 3);
    self__->setSizePolicy(horizontal, vertical);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.setSizePolicy: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.setSizePolicy: Unknown exception");
  }
}


/** void mimas::ListView::setStyle(const char *text)
 * include/mimas/ListView.h:128
 */
static int ListView_setStyle(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in setStyle");
    const char *text = luaL_checkstring(L, 2);
    self__->setStyle(text);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.setStyle: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.setStyle: Unknown exception");
  }
}


/** void mimas::ListView::show()
 * include/mimas/ListView.h:194
 */
static int ListView_show(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in show");
    self__->show();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.show: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.show: Unknown exception");
  }
}


/** LuaStackSize mimas::ListView::textSize(const char *text, lua_State *L)
 * include/mimas/ListView.h:148
 */
static int ListView_textSize(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in textSize");
    const char *text = luaL_checkstring(L, 2);
    
    LuaStackSize  retval__ = self__->textSize(text, L);
    return retval__;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.textSize: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.textSize: Unknown exception");
  }
}


/** void mimas::ListView::update()
 * include/mimas/ListView.h:141
 */
static int ListView_update(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in update");
    self__->update();
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.update: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.update: Unknown exception");
  }
}


/** QWidget* mimas::ListView::widget()
 * include/mimas/ListView.h:97
 */
static int ListView_widget(lua_State *L) {
  try {
    ListView *self__ = *((ListView**)luaL_checkudata(L, 1, "mimas.ListView"));
    if (!self__) return luaL_error(L, "Using deleted mimas.ListView in widget");
    QWidget * retval__ = self__->widget();
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.ListView.widget: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.ListView.widget: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg ListView_member_methods[] = {
  {"__newindex"        , ListView___newindex},
  {"close"             , ListView_close},
  {"cssClass"          , ListView_cssClass},
  {"globalMove"        , ListView_globalMove},
  {"globalPosition"    , ListView_globalPosition},
  {"hide"              , ListView_hide},
  {"hue"               , ListView_hue},
  {"indexAt"           , ListView_indexAt},
  {"isVisible"         , ListView_isVisible},
  {"lower"             , ListView_lower},
  {"move"              , ListView_move},
  {"name"              , ListView_name},
  {"object"            , ListView_object},
  {"raise"             , ListView_raise},
  {"resize"            , ListView_resize},
  {"setHue"            , ListView_setHue},
  {"setMinimumSize"    , ListView_setMinimumSize},
  {"setMouseTracking"  , ListView_setMouseTracking},
  {"setName"           , ListView_setName},
  {"setSizeHint"       , ListView_setSizeHint},
  {"setSizePolicy"     , ListView_setSizePolicy},
  {"setStyle"          , ListView_setStyle},
  {"show"              , ListView_show},
  {"textSize"          , ListView_textSize},
  {"update"            , ListView_update},
  {"widget"            , ListView_widget},
  {"__tostring"        , ListView__tostring},
  {"__gc"              , ListView_destructor},
  {"deleted"           , ListView_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg ListView_namespace_methods[] = {
  {"ListView"          , ListView_ListView},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_ListView(lua_State *L) {
#else
extern "C" int luaopen_mimas_ListView(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.ListView");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, ListView_member_methods);
  // save meta-table in mimas.ListView_
  register_mt(L, "mimas", "ListView");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", ListView_namespace_methods);


	return 1;
}