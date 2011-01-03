#include "mimas/HBoxLayout.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */


/** mimas::HBoxLayout::HBoxLayout(QWidget *parent)
 * include/mimas/HBoxLayout.h:48
 */
static int HBoxLayout_HBoxLayout1(lua_State *L) {
  try {
    QWidget *parent = *((QWidget **)luaL_checkudata(L, 1, "mimas.QWidget"));
    HBoxLayout * retval__ = new HBoxLayout(parent);
    lua_pushclass<HBoxLayout>(L, retval__, "mimas.HBoxLayout");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.HBoxLayout.HBoxLayout: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.HBoxLayout.HBoxLayout: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** mimas::HBoxLayout::HBoxLayout()
 * include/mimas/HBoxLayout.h:50
 */
static int HBoxLayout_HBoxLayout2(lua_State *L) {
  try {
    HBoxLayout * retval__ = new HBoxLayout();
    lua_pushclass<HBoxLayout>(L, retval__, "mimas.HBoxLayout");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.HBoxLayout.HBoxLayout: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.HBoxLayout.HBoxLayout: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for HBoxLayout(...) */
static int HBoxLayout_HBoxLayout(lua_State *L) {
  int type__ = lua_type(L, 2);
  int top__  = lua_gettop(L);
  if (top__ < 2) {
    return HBoxLayout_HBoxLayout2(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 2, "mimas.QWidget")) {
    return HBoxLayout_HBoxLayout1(L);
  } else {
    // use any to raise errors
    return HBoxLayout_HBoxLayout1(L);
  }
}

/* ============================ Destructor       ====================== */

static int HBoxLayout_destructor(lua_State *L) {
  HBoxLayout **userdata = (HBoxLayout**)luaL_checkudata(L, 1, "mimas.HBoxLayout");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int HBoxLayout__tostring(lua_State *L) {
  HBoxLayout **userdata = (HBoxLayout**)luaL_checkudata(L, 1, "mimas.HBoxLayout");
  
  lua_pushfstring(L, "<mimas.HBoxLayout: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::HBoxLayout::addLayout(QLayout *layout)
 * include/mimas/HBoxLayout.h:58
 */
static int HBoxLayout_addLayout(lua_State *L) {
  try {
    HBoxLayout *self__ = *((HBoxLayout**)luaL_checkudata(L, 1, "mimas.HBoxLayout"));
    QLayout *layout = *((QLayout **)luaL_checkudata(L, 2, "mimas.QLayout"));
    self__->addLayout(layout);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.HBoxLayout.addLayout: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.HBoxLayout.addLayout: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::HBoxLayout::addWidget(QWidget *widget)
 * include/mimas/HBoxLayout.h:54
 */
static int HBoxLayout_addWidget(lua_State *L) {
  try {
    HBoxLayout *self__ = *((HBoxLayout**)luaL_checkudata(L, 1, "mimas.HBoxLayout"));
    QWidget *widget = *((QWidget **)luaL_checkudata(L, 2, "mimas.QWidget"));
    self__->addWidget(widget);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.HBoxLayout.addWidget: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.HBoxLayout.addWidget: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** QLayout* mimas::HBoxLayout::layout()
 * include/mimas/HBoxLayout.h:62
 */
static int HBoxLayout_layout(lua_State *L) {
  try {
    HBoxLayout *self__ = *((HBoxLayout**)luaL_checkudata(L, 1, "mimas.HBoxLayout"));
    QLayout * retval__ = self__->layout();
    lua_pushclass<QLayout>(L, retval__, "mimas.QLayout");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.HBoxLayout.layout: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.HBoxLayout.layout: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg HBoxLayout_member_methods[] = {
  {"addLayout"         , HBoxLayout_addLayout},
  {"addWidget"         , HBoxLayout_addWidget},
  {"layout"            , HBoxLayout_layout},
  {"__tostring"        , HBoxLayout__tostring},
  {"__gc"              , HBoxLayout_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg HBoxLayout_namespace_methods[] = {
  {"HBoxLayout"        , HBoxLayout_HBoxLayout},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_HBoxLayout_core(lua_State *L) {
#else
extern "C" int luaopen_mimas_HBoxLayout_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.HBoxLayout");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, HBoxLayout_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "mimas", HBoxLayout_namespace_methods);


	return 1;
}
