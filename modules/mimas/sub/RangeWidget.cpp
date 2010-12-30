#include "mimas/RangeWidget.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::RangeWidget::RangeWidget(Widget *widget)
 * include/mimas/RangeWidget.h:42
 */
static int RangeWidget_RangeWidget(lua_State *L) {
  try {
    Widget *widget = *((Widget **)luaL_checkudata(L, 1, "mimas.Widget"));
    RangeWidget * retval__ = new RangeWidget(widget);
    lua_pushclass<RangeWidget>(L, retval__, "mimas.RangeWidget");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.RangeWidget.RangeWidget: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.RangeWidget.RangeWidget: Unknown exception");
    lua_error(L);
    return 0;
  }
}

/* ============================ Destructor       ====================== */

static int RangeWidget_destructor(lua_State *L) {
  RangeWidget **userdata = (RangeWidget**)luaL_checkudata(L, 1, "mimas.RangeWidget");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int RangeWidget__tostring(lua_State *L) {
  RangeWidget **userdata = (RangeWidget**)luaL_checkudata(L, 1, "mimas.RangeWidget");
  
  lua_pushfstring(L, "<mimas.RangeWidget: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** bool mimas::RangeWidget::isDragged() const 
 * include/mimas/RangeWidget.h:78
 */
static int RangeWidget_isDragged(lua_State *L) {
  try {
    RangeWidget *self__ = *((RangeWidget**)luaL_checkudata(L, 1, "mimas.RangeWidget"));
    lua_remove(L, 1);
    bool  retval__ = self__->isDragged();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.RangeWidget.isDragged: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.RangeWidget.isDragged: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** int mimas::RangeWidget::position(double range)
 * include/mimas/RangeWidget.h:52
 */
static int RangeWidget_position(lua_State *L) {
  try {
    RangeWidget *self__ = *((RangeWidget**)luaL_checkudata(L, 1, "mimas.RangeWidget"));
    lua_remove(L, 1);
    double range = luaL_checknumber(L, 1);
    int  retval__ = self__->position(range);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.RangeWidget.position: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.RangeWidget.position: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** int mimas::RangeWidget::remotePosition(double range)
 * include/mimas/RangeWidget.h:59
 */
static int RangeWidget_remotePosition(lua_State *L) {
  try {
    RangeWidget *self__ = *((RangeWidget**)luaL_checkudata(L, 1, "mimas.RangeWidget"));
    lua_remove(L, 1);
    double range = luaL_checknumber(L, 1);
    int  retval__ = self__->remotePosition(range);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.RangeWidget.remotePosition: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.RangeWidget.remotePosition: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::RangeWidget::setDragged(bool dragged)
 * include/mimas/RangeWidget.h:82
 */
static int RangeWidget_setDragged(lua_State *L) {
  try {
    RangeWidget *self__ = *((RangeWidget**)luaL_checkudata(L, 1, "mimas.RangeWidget"));
    lua_remove(L, 1);
    bool dragged = lua_toboolean(L, 1);
    self__->setDragged(dragged);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.RangeWidget.setDragged: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.RangeWidget.setDragged: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::RangeWidget::setPosition(double position, double range)
 * include/mimas/RangeWidget.h:67
 */
static int RangeWidget_setPosition(lua_State *L) {
  try {
    RangeWidget *self__ = *((RangeWidget**)luaL_checkudata(L, 1, "mimas.RangeWidget"));
    lua_remove(L, 1);
    double position = luaL_checknumber(L, 1);
    double range = luaL_checknumber(L, 2);
    self__->setPosition(position, range);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.RangeWidget.setPosition: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.RangeWidget.setPosition: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::RangeWidget::setRange(double min, double max)
 * include/mimas/RangeWidget.h:46
 */
static int RangeWidget_setRange(lua_State *L) {
  try {
    RangeWidget *self__ = *((RangeWidget**)luaL_checkudata(L, 1, "mimas.RangeWidget"));
    lua_remove(L, 1);
    double min = luaL_checknumber(L, 1);
    double max = luaL_checknumber(L, 2);
    self__->setRange(min, max);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.RangeWidget.setRange: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.RangeWidget.setRange: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg RangeWidget_member_methods[] = {
  {"isDragged"         , RangeWidget_isDragged},
  {"position"          , RangeWidget_position},
  {"remotePosition"    , RangeWidget_remotePosition},
  {"setDragged"        , RangeWidget_setDragged},
  {"setPosition"       , RangeWidget_setPosition},
  {"setRange"          , RangeWidget_setRange},
  {"__tostring"        , RangeWidget__tostring},
  {"__gc"              , RangeWidget_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg RangeWidget_namespace_methods[] = {
  {"RangeWidget"       , RangeWidget_RangeWidget},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_RangeWidget(lua_State *L) {
#else
extern "C" int luaopen_mimas_RangeWidget(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.RangeWidget");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, RangeWidget_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "mimas", RangeWidget_namespace_methods);


	return 1;
}
