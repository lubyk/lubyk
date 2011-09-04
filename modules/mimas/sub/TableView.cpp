#include "mimas/TableView.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::TableView::TableView(lubyk::Worker *worker)
 * include/mimas/TableView.h:60
 */
static int TableView_TableView(lua_State *L) {
  try {
    lubyk::Worker *worker = *((lubyk::Worker **)dubL_checksdata(L, 1, "lubyk.Worker"));
    TableView * retval__ = new TableView(worker);
    // The class inherits from 'LuaCallback', use lua_init instead of pushclass.
    return retval__->luaInit(L, retval__, "mimas.TableView");
  } catch (std::exception &e) {
    lua_pushfstring(L, "TableView: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "TableView: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int TableView_destructor(lua_State *L) {
  TableView **userdata = (TableView**)dubL_checksdata_n(L, 1, "mimas.TableView");

  
  // custom destructor
  if (*userdata) (*userdata)->luaDestroy();
  
  *userdata = NULL;
  return 0;
}




// test if class is deleted
static int TableView_deleted(lua_State *L) {
  TableView **userdata = (TableView**)dubL_checksdata_n(L, 1, "mimas.TableView");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int TableView__tostring(lua_State *L) {
  TableView **userdata = (TableView**)dubL_checksdata_n(L, 1, "mimas.TableView");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.TableView: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.TableView: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** bool mimas::TableView::close()
 * include/mimas/TableView.h:217
 */
static int TableView_close(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in close");
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



/** QString mimas::TableView::cssClass() const 
 * include/mimas/TableView.h:109
 */
static int TableView_cssClass(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in cssClass");
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



/** void mimas::TableView::globalMove(float x, float y)
 * include/mimas/TableView.h:245
 */
static int TableView_globalMove(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in globalMove");
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



/** LuaStackSize mimas::TableView::globalPosition(lua_State *L)
 * include/mimas/TableView.h:236
 */
static int TableView_globalPosition(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in globalPosition");
    
    LuaStackSize  retval__ = self->globalPosition(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "globalPosition: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "globalPosition: Unknown exception");
  }
  return lua_error(L);
}



/** int mimas::TableView::height()
 * include/mimas/TableView.h:154
 */
static int TableView_height(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in height");
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



/** void mimas::TableView::hide()
 * include/mimas/TableView.h:229
 */
static int TableView_hide(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in hide");
    self->hide();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "hide: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "hide: Unknown exception");
  }
  return lua_error(L);
}



/** float mimas::TableView::hue()
 * include/mimas/TableView.h:171
 */
static int TableView_hue(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in hue");
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



/** bool mimas::TableView::isVisible() const 
 * include/mimas/TableView.h:221
 */
static int TableView_isVisible(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in isVisible");
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



/** void mimas::TableView::lower()
 * include/mimas/TableView.h:251
 */
static int TableView_lower(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in lower");
    self->lower();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "lower: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "lower: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::TableView::move(int x, int y)
 * include/mimas/TableView.h:134
 */
static int TableView_move(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in move");
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



/** LuaStackSize mimas::TableView::name(lua_State *L)
 * include/mimas/TableView.h:123
 */
static int TableView_name(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in name");
    
    LuaStackSize  retval__ = self->name(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "name: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "name: Unknown exception");
  }
  return lua_error(L);
}



/** QObject* mimas::TableView::object()
 * include/mimas/TableView.h:117
 */
static int TableView_object(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in object");
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



/** void mimas::TableView::raise()
 * include/mimas/TableView.h:257
 */
static int TableView_raise(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in raise");
    self->raise();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "raise: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "raise: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::TableView::resize(int w, int h)
 * include/mimas/TableView.h:138
 */
static int TableView_resize(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in resize");
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



/** virtual void mimas::TableView::selectColumn(int row)
 * include/mimas/TableView.h:103
 */
static int TableView_selectColumn(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in selectColumn");
    int row = dubL_checkint(L, 2);
    self->selectColumn(row);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "selectColumn: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "selectColumn: Unknown exception");
  }
  return lua_error(L);
}



/** virtual void mimas::TableView::selectRow(int row)
 * include/mimas/TableView.h:99
 */
static int TableView_selectRow(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in selectRow");
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



/** void mimas::TableView::setAlternatingRowColors(bool should_enable)
 * include/mimas/TableView.h:95
 */
static int TableView_setAlternatingRowColors(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in setAlternatingRowColors");
    bool should_enable = lua_toboolean(L, 2);
    self->setAlternatingRowColors(should_enable);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setAlternatingRowColors: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setAlternatingRowColors: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::TableView::setGridStyle(int style)
 * include/mimas/TableView.h:89
 */
static int TableView_setGridStyle(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in setGridStyle");
    int style = dubL_checkint(L, 2);
    self->setGridStyle(style);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setGridStyle: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setGridStyle: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::TableView::setHue(float hue)
 * include/mimas/TableView.h:166
 */
static int TableView_setHue(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in setHue");
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



/** void mimas::TableView::setMinimumSize(float w, float h)
 * include/mimas/TableView.h:203
 */
static int TableView_setMinimumSize(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in setMinimumSize");
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



/** void mimas::TableView::setModel(DataSource *model)
 * include/mimas/TableView.h:263
 */
static int TableView_setModel(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in setModel");
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



/** void mimas::TableView::setMouseTracking(bool enable)
 * include/mimas/TableView.h:209
 */
static int TableView_setMouseTracking(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in setMouseTracking");
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



/** void mimas::TableView::setName(const char *name)
 * include/mimas/TableView.h:130
 */
static int TableView_setName(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in setName");
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



/** void mimas::TableView::setSizeHint(float w, float h)
 * include/mimas/TableView.h:190
 */
static int TableView_setSizeHint(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in setSizeHint");
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



/** void mimas::TableView::setSizePolicy(int horizontal, int vertical)
 * include/mimas/TableView.h:197
 */
static int TableView_setSizePolicy(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in setSizePolicy");
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



/** void mimas::TableView::setStyle(const char *text)
 * include/mimas/TableView.h:158
 */
static int TableView_setStyle(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in setStyle");
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



/** void mimas::TableView::setStyleSheet(const char *text)
 * include/mimas/TableView.h:162
 */
static int TableView_setStyleSheet(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in setStyleSheet");
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



/** void mimas::TableView::setVisibleHeaders(int orientation, bool visible)
 * include/mimas/TableView.h:72
 */
static int TableView_setVisibleHeaders(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in setVisibleHeaders");
    int orientation = dubL_checkint(L, 2);
    bool visible = lua_toboolean(L, 3);
    self->setVisibleHeaders(orientation, visible);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setVisibleHeaders: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setVisibleHeaders: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::TableView::show()
 * include/mimas/TableView.h:225
 */
static int TableView_show(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in show");
    self->show();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "show: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "show: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::TableView::textSize(const char *text, lua_State *L)
 * include/mimas/TableView.h:181
 */
static int TableView_textSize(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in textSize");
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



/** void mimas::TableView::update()
 * include/mimas/TableView.h:175
 */
static int TableView_update(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in update");
    self->update();
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "update: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "update: Unknown exception");
  }
  return lua_error(L);
}



/** QWidget* mimas::TableView::widget()
 * include/mimas/TableView.h:113
 */
static int TableView_widget(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in widget");
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



/** int mimas::TableView::width()
 * include/mimas/TableView.h:150
 */
static int TableView_width(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in width");
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



/** int mimas::TableView::x()
 * include/mimas/TableView.h:142
 */
static int TableView_x(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in x");
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



/** int mimas::TableView::y()
 * include/mimas/TableView.h:146
 */
static int TableView_y(lua_State *L) {
  try {
    TableView *self = *((TableView**)dubL_checksdata(L, 1, "mimas.TableView"));
    if (!self) throw dub::Exception("Using deleted mimas.TableView in y");
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

static const struct luaL_Reg TableView_member_methods[] = {
  {"close"             , TableView_close},
  {"cssClass"          , TableView_cssClass},
  {"globalMove"        , TableView_globalMove},
  {"globalPosition"    , TableView_globalPosition},
  {"height"            , TableView_height},
  {"hide"              , TableView_hide},
  {"hue"               , TableView_hue},
  {"isVisible"         , TableView_isVisible},
  {"lower"             , TableView_lower},
  {"move"              , TableView_move},
  {"name"              , TableView_name},
  {"object"            , TableView_object},
  {"raise"             , TableView_raise},
  {"resize"            , TableView_resize},
  {"selectColumn"      , TableView_selectColumn},
  {"selectRow"         , TableView_selectRow},
  {"setAlternatingRowColors", TableView_setAlternatingRowColors},
  {"setGridStyle"      , TableView_setGridStyle},
  {"setHue"            , TableView_setHue},
  {"setMinimumSize"    , TableView_setMinimumSize},
  {"setModel"          , TableView_setModel},
  {"setMouseTracking"  , TableView_setMouseTracking},
  {"setName"           , TableView_setName},
  {"setSizeHint"       , TableView_setSizeHint},
  {"setSizePolicy"     , TableView_setSizePolicy},
  {"setStyle"          , TableView_setStyle},
  {"setStyleSheet"     , TableView_setStyleSheet},
  {"setVisibleHeaders" , TableView_setVisibleHeaders},
  {"show"              , TableView_show},
  {"textSize"          , TableView_textSize},
  {"update"            , TableView_update},
  {"widget"            , TableView_widget},
  {"width"             , TableView_width},
  {"x"                 , TableView_x},
  {"y"                 , TableView_y},
  {"__tostring"        , TableView__tostring},
  {"__gc"              , TableView_destructor},
  {"deleted"           , TableView_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg TableView_namespace_methods[] = {
  {"TableView"         , TableView_TableView},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_TableView(lua_State *L) {
#else
extern "C" int luaopen_mimas_TableView(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.TableView");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, TableView_member_methods);
  // save meta-table in mimas.TableView_
  register_mt(L, "mimas", "TableView");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", TableView_namespace_methods);


	return 1;
}
