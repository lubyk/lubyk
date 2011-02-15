#include "mimas/VBoxLayout.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */


/** mimas::VBoxLayout::VBoxLayout(QWidget *parent)
 * include/mimas/VBoxLayout.h:49
 */
static int VBoxLayout_VBoxLayout1(lua_State *L) {
  try {
    QWidget *parent = *((QWidget **)luaL_checkudata(L, 1, "mimas.QWidget"));
    VBoxLayout * retval__ = new VBoxLayout(parent);
    lua_pushclass2<VBoxLayout>(L, retval__, "mimas.VBoxLayout");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.VBoxLayout.VBoxLayout: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.VBoxLayout.VBoxLayout: Unknown exception");
  }
}


/** mimas::VBoxLayout::VBoxLayout()
 * include/mimas/VBoxLayout.h:51
 */
static int VBoxLayout_VBoxLayout2(lua_State *L) {
  try {
    VBoxLayout * retval__ = new VBoxLayout();
    lua_pushclass2<VBoxLayout>(L, retval__, "mimas.VBoxLayout");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.VBoxLayout.VBoxLayout: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.VBoxLayout.VBoxLayout: Unknown exception");
  }
}



/** Overloaded function chooser for VBoxLayout(...) */
static int VBoxLayout_VBoxLayout(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (top__ < 1) {
    return VBoxLayout_VBoxLayout2(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "mimas.QWidget")) {
    return VBoxLayout_VBoxLayout1(L);
  } else {
    // use any to raise errors
    return VBoxLayout_VBoxLayout1(L);
  }
}


/* ============================ Destructor       ====================== */

static int VBoxLayout_destructor(lua_State *L) {
  VBoxLayout **userdata = (VBoxLayout**)luaL_checkudata(L, 1, "mimas.VBoxLayout");

  
  // custom destructor
  if (*userdata) (*userdata)->dub_destroy();
  
  *userdata = NULL;
  return 0;
}




// test if class is deleted
static int VBoxLayout_deleted(lua_State *L) {
  VBoxLayout **userdata = (VBoxLayout**)luaL_checkudata(L, 1, "mimas.VBoxLayout");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int VBoxLayout__tostring(lua_State *L) {
  VBoxLayout **userdata = (VBoxLayout**)luaL_checkudata(L, 1, "mimas.VBoxLayout");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.VBoxLayout: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.VBoxLayout: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::VBoxLayout::addLayout(QLayout *layout)
 * include/mimas/VBoxLayout.h:61
 */
static int VBoxLayout_addLayout(lua_State *L) {
  try {
    VBoxLayout *self__ = *((VBoxLayout**)luaL_checkudata(L, 1, "mimas.VBoxLayout"));
    if (!self__) return luaL_error(L, "Using deleted mimas.VBoxLayout in addLayout");
    QLayout *layout = *((QLayout **)luaL_checkudata(L, 2, "mimas.QLayout"));
    self__->addLayout(layout);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.VBoxLayout.addLayout: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.VBoxLayout.addLayout: Unknown exception");
  }
}


/** void mimas::VBoxLayout::addWidget(QWidget *widget)
 * include/mimas/VBoxLayout.h:57
 */
static int VBoxLayout_addWidget(lua_State *L) {
  try {
    VBoxLayout *self__ = *((VBoxLayout**)luaL_checkudata(L, 1, "mimas.VBoxLayout"));
    if (!self__) return luaL_error(L, "Using deleted mimas.VBoxLayout in addWidget");
    QWidget *widget = *((QWidget **)luaL_checkudata(L, 2, "mimas.QWidget"));
    self__->addWidget(widget);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.VBoxLayout.addWidget: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.VBoxLayout.addWidget: Unknown exception");
  }
}


/** QLayout* mimas::VBoxLayout::layout()
 * include/mimas/VBoxLayout.h:65
 */
static int VBoxLayout_layout(lua_State *L) {
  try {
    VBoxLayout *self__ = *((VBoxLayout**)luaL_checkudata(L, 1, "mimas.VBoxLayout"));
    if (!self__) return luaL_error(L, "Using deleted mimas.VBoxLayout in layout");
    QLayout * retval__ = self__->layout();
    lua_pushclass<QLayout>(L, retval__, "mimas.QLayout");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.VBoxLayout.layout: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.VBoxLayout.layout: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg VBoxLayout_member_methods[] = {
  {"addLayout"         , VBoxLayout_addLayout},
  {"addWidget"         , VBoxLayout_addWidget},
  {"layout"            , VBoxLayout_layout},
  {"__tostring"        , VBoxLayout__tostring},
  {"__gc"              , VBoxLayout_destructor},
  {"deleted"           , VBoxLayout_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg VBoxLayout_namespace_methods[] = {
  {"VBoxLayout"        , VBoxLayout_VBoxLayout},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_VBoxLayout_core(lua_State *L) {
#else
extern "C" int luaopen_mimas_VBoxLayout_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.VBoxLayout");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, VBoxLayout_member_methods);
  // save meta-table in mimas.VBoxLayout_
  register_mt(L, "mimas", "VBoxLayout");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", VBoxLayout_namespace_methods);


	return 1;
}
