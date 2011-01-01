#include "mimas/VBoxLayout.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::VBoxLayout::VBoxLayout(QWidget *widget)
 * include/mimas/VBoxLayout.h:47
 */
static int VBoxLayout_VBoxLayout(lua_State *L) {
  try {
    QWidget *widget = *((QWidget **)luaL_checkudata(L, 1, "mimas.QWidget"));
    VBoxLayout * retval__ = new VBoxLayout(widget);
    lua_pushclass<VBoxLayout>(L, retval__, "mimas.VBoxLayout");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.VBoxLayout.VBoxLayout: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.VBoxLayout.VBoxLayout: Unknown exception");
    lua_error(L);
    return 0;
  }
}

/* ============================ Destructor       ====================== */

static int VBoxLayout_destructor(lua_State *L) {
  VBoxLayout **userdata = (VBoxLayout**)luaL_checkudata(L, 1, "mimas.VBoxLayout");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int VBoxLayout__tostring(lua_State *L) {
  VBoxLayout **userdata = (VBoxLayout**)luaL_checkudata(L, 1, "mimas.VBoxLayout");
  
  lua_pushfstring(L, "<mimas.VBoxLayout: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::VBoxLayout::addLayout(QLayout *layout)
 * include/mimas/VBoxLayout.h:55
 */
static int VBoxLayout_addLayout(lua_State *L) {
  try {
    VBoxLayout *self__ = *((VBoxLayout**)luaL_checkudata(L, 1, "mimas.VBoxLayout"));
    QLayout *layout = *((QLayout **)luaL_checkudata(L, 2, "mimas.QLayout"));
    self__->addLayout(layout);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.VBoxLayout.addLayout: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.VBoxLayout.addLayout: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::VBoxLayout::addWidget(QWidget *widget)
 * include/mimas/VBoxLayout.h:51
 */
static int VBoxLayout_addWidget(lua_State *L) {
  try {
    VBoxLayout *self__ = *((VBoxLayout**)luaL_checkudata(L, 1, "mimas.VBoxLayout"));
    QWidget *widget = *((QWidget **)luaL_checkudata(L, 2, "mimas.QWidget"));
    self__->addWidget(widget);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.VBoxLayout.addWidget: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.VBoxLayout.addWidget: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** QLayout* mimas::VBoxLayout::layout()
 * include/mimas/VBoxLayout.h:59
 */
static int VBoxLayout_layout(lua_State *L) {
  try {
    VBoxLayout *self__ = *((VBoxLayout**)luaL_checkudata(L, 1, "mimas.VBoxLayout"));
    QLayout * retval__ = self__->layout();
    lua_pushclass<QLayout>(L, retval__, "mimas.QLayout");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.VBoxLayout.layout: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.VBoxLayout.layout: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg VBoxLayout_member_methods[] = {
  {"addLayout"         , VBoxLayout_addLayout},
  {"addWidget"         , VBoxLayout_addWidget},
  {"layout"            , VBoxLayout_layout},
  {"__tostring"        , VBoxLayout__tostring},
  {"__gc"              , VBoxLayout_destructor},
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

  // register class methods in a global namespace table
  luaL_register(L, "mimas", VBoxLayout_namespace_methods);


	return 1;
}
