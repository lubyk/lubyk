#include "mimas/ListView.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::ListView::ListView(lubyk::Worker *worker)
 * include/mimas/ListView.h:61
 */
static int ListView_ListView(lua_State *L) {
  try {
    lubyk::Worker *worker = *((lubyk::Worker **)dubL_checksdata(L, 1, "lubyk.Worker"));
    ListView * retval__ = new ListView(worker);
    return retval__->luaInit(L, retval__, "mimas.ListView");
  } catch (std::exception &e) {
    lua_pushfstring(L, "ListView: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "ListView: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int ListView_destructor(lua_State *L) {
  ListView **userdata = (ListView**)dubL_checksdata_n(L, 1, "mimas.ListView");

  
  // custom destructor
  ListView *self = *userdata;
  if (self) self->luaDestroy();
  
  *userdata = NULL;
  return 0;
}



// test if class is deleted
static int ListView_deleted(lua_State *L) {
  ListView **userdata = (ListView**)dubL_checksdata_n(L, 1, "mimas.ListView");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int ListView__tostring(lua_State *L) {
  ListView **userdata = (ListView**)dubL_checksdata_n(L, 1, "mimas.ListView");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.ListView: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.ListView: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** bool mimas::ListView::close()
 * include/mimas/ListView.h:183
 */
static int ListView_close(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in close");
    bool  retval__ = self->close();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "close: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "close: Unknown exception");
  }
  return lua_error(L);
}



/** QString mimas::ListView::cssClass() const 
 * include/mimas/ListView.h:77
 */
static int ListView_cssClass(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in cssClass");
    QString  retval__ = self->cssClass();
    lua_pushclass<QString>(L, retval__, "mimas.QString");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "cssClass: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "cssClass: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::ListView::globalMove(float x, float y)
 * include/mimas/ListView.h:211
 */
static int ListView_globalMove(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in globalMove");
    float x = dubL_checknumber(L, 2);
    float y = dubL_checknumber(L, 3);
    self->globalMove(x, y);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "globalMove: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "globalMove: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::ListView::globalPosition(lua_State *L)
 * include/mimas/ListView.h:202
 */
static int ListView_globalPosition(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in globalPosition");
    
    LuaStackSize  retval__ = self->globalPosition(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "globalPosition: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "globalPosition: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::ListView::height()
 * include/mimas/ListView.h:122
 */
static int ListView_height(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in height");
    int  retval__ = self->height();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "height: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "height: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::ListView::hide()
 * include/mimas/ListView.h:195
 */
static int ListView_hide(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in hide");
    self->hide();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "hide: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "hide: Unknown exception");
  }
  return lua_error(L);
}



/** float mimas::ListView::hue()
 * include/mimas/ListView.h:139
 */
static int ListView_hue(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in hue");
    float  retval__ = self->hue();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "hue: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "hue: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::ListView::indexAt(float x, float y, lua_State *L)
 * include/mimas/ListView.h:233
 */
static int ListView_indexAt(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in indexAt");
    float x = dubL_checknumber(L, 2);
    float y = dubL_checknumber(L, 3);
    
    LuaStackSize  retval__ = self->indexAt(x, y, L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "indexAt: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "indexAt: Unknown exception");
  }
  return lua_error(L);
}



/** bool mimas::ListView::isVisible() const 
 * include/mimas/ListView.h:187
 */
static int ListView_isVisible(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in isVisible");
    bool  retval__ = self->isVisible();
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "isVisible: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "isVisible: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::ListView::lower()
 * include/mimas/ListView.h:217
 */
static int ListView_lower(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in lower");
    self->lower();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "lower: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "lower: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::ListView::move(int x, int y)
 * include/mimas/ListView.h:102
 */
static int ListView_move(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in move");
    int x = dubL_checkint(L, 2);
    int y = dubL_checkint(L, 3);
    self->move(x, y);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "move: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "move: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::ListView::name(lua_State *L)
 * include/mimas/ListView.h:91
 */
static int ListView_name(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in name");
    
    LuaStackSize  retval__ = self->name(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "name: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "name: Unknown exception");
  }
  return lua_error(L);
}



/** QObject* mimas::ListView::object()
 * include/mimas/ListView.h:85
 */
static int ListView_object(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in object");
    QObject * retval__ = self->object();
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "object: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "object: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::ListView::raise()
 * include/mimas/ListView.h:223
 */
static int ListView_raise(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in raise");
    self->raise();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "raise: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "raise: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::ListView::resize(int w, int h)
 * include/mimas/ListView.h:106
 */
static int ListView_resize(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in resize");
    int w = dubL_checkint(L, 2);
    int h = dubL_checkint(L, 3);
    self->resize(w, h);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "resize: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "resize: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::ListView::selectRow(int row)
 * include/mimas/ListView.h:245
 */
static int ListView_selectRow(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in selectRow");
    int row = dubL_checkint(L, 2);
    self->selectRow(row);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "selectRow: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "selectRow: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::ListView::setHue(float hue)
 * include/mimas/ListView.h:134
 */
static int ListView_setHue(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in setHue");
    float hue = dubL_checknumber(L, 2);
    self->setHue(hue);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setHue: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setHue: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::ListView::setMinimumSize(float w, float h)
 * include/mimas/ListView.h:171
 */
static int ListView_setMinimumSize(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in setMinimumSize");
    float w = dubL_checknumber(L, 2);
    float h = dubL_checknumber(L, 3);
    self->setMinimumSize(w, h);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setMinimumSize: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setMinimumSize: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::ListView::setModel(DataSource *model)
 * include/mimas/ListView.h:253
 */
static int ListView_setModel(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in setModel");
    DataSource *model = *((DataSource **)dubL_checksdata(L, 2, "mimas.DataSource"));
    self->setModel(model);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setModel: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setModel: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::ListView::setMouseTracking(bool enable)
 * include/mimas/ListView.h:177
 */
static int ListView_setMouseTracking(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in setMouseTracking");
    bool enable = lua_toboolean(L, 2);
    self->setMouseTracking(enable);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setMouseTracking: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setMouseTracking: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::ListView::setName(const char *name)
 * include/mimas/ListView.h:98
 */
static int ListView_setName(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in setName");
    const char *name = dubL_checkstring(L, 2);
    self->setName(name);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setName: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setName: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::ListView::setSizeHint(float w, float h)
 * include/mimas/ListView.h:158
 */
static int ListView_setSizeHint(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in setSizeHint");
    float w = dubL_checknumber(L, 2);
    float h = dubL_checknumber(L, 3);
    self->setSizeHint(w, h);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setSizeHint: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setSizeHint: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::ListView::setSizePolicy(int horizontal, int vertical)
 * include/mimas/ListView.h:165
 */
static int ListView_setSizePolicy(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in setSizePolicy");
    int horizontal = dubL_checkint(L, 2);
    int vertical = dubL_checkint(L, 3);
    self->setSizePolicy(horizontal, vertical);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setSizePolicy: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setSizePolicy: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::ListView::setStyle(const char *text)
 * include/mimas/ListView.h:126
 */
static int ListView_setStyle(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in setStyle");
    const char *text = dubL_checkstring(L, 2);
    self->setStyle(text);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setStyle: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setStyle: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::ListView::setStyleSheet(const char *text)
 * include/mimas/ListView.h:130
 */
static int ListView_setStyleSheet(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in setStyleSheet");
    const char *text = dubL_checkstring(L, 2);
    self->setStyleSheet(text);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setStyleSheet: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setStyleSheet: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::ListView::show()
 * include/mimas/ListView.h:191
 */
static int ListView_show(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in show");
    self->show();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "show: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "show: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::ListView::textSize(const char *text, lua_State *L)
 * include/mimas/ListView.h:149
 */
static int ListView_textSize(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in textSize");
    const char *text = dubL_checkstring(L, 2);
    
    LuaStackSize  retval__ = self->textSize(text, L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "textSize: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "textSize: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::ListView::update()
 * include/mimas/ListView.h:143
 */
static int ListView_update(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in update");
    self->update();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "update: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "update: Unknown exception");
  }
  return lua_error(L);
}



/** QWidget* mimas::ListView::widget()
 * include/mimas/ListView.h:81
 */
static int ListView_widget(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in widget");
    QWidget * retval__ = self->widget();
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "widget: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "widget: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::ListView::width()
 * include/mimas/ListView.h:118
 */
static int ListView_width(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in width");
    int  retval__ = self->width();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "width: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "width: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::ListView::x()
 * include/mimas/ListView.h:110
 */
static int ListView_x(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in x");
    int  retval__ = self->x();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "x: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "x: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::ListView::y()
 * include/mimas/ListView.h:114
 */
static int ListView_y(lua_State *L) {
  try {
    ListView *self = *((ListView**)dubL_checksdata(L, 1, "mimas.ListView"));
    if (!self) throw dub::Exception("Using deleted mimas.ListView in y");
    int  retval__ = self->y();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "y: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "y: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg ListView_member_methods[] = {
  {"close"             , ListView_close},
  {"cssClass"          , ListView_cssClass},
  {"globalMove"        , ListView_globalMove},
  {"globalPosition"    , ListView_globalPosition},
  {"height"            , ListView_height},
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
  {"selectRow"         , ListView_selectRow},
  {"setHue"            , ListView_setHue},
  {"setMinimumSize"    , ListView_setMinimumSize},
  {"setModel"          , ListView_setModel},
  {"setMouseTracking"  , ListView_setMouseTracking},
  {"setName"           , ListView_setName},
  {"setSizeHint"       , ListView_setSizeHint},
  {"setSizePolicy"     , ListView_setSizePolicy},
  {"setStyle"          , ListView_setStyle},
  {"setStyleSheet"     , ListView_setStyleSheet},
  {"show"              , ListView_show},
  {"textSize"          , ListView_textSize},
  {"update"            , ListView_update},
  {"widget"            , ListView_widget},
  {"width"             , ListView_width},
  {"x"                 , ListView_x},
  {"y"                 , ListView_y},
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
