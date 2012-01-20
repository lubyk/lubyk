#include "mimas/VBoxLayout.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::VBoxLayout::VBoxLayout(QWidget *parent=NULL)
 * include/mimas/VBoxLayout.h:52
 */
static int VBoxLayout_VBoxLayout(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    VBoxLayout * retval__;
    if (top__ < 1) {
      retval__ = new VBoxLayout();
    } else {
      QWidget *parent = *((QWidget **)dubL_checksdata(L, 1, "mimas.QWidget"));
      retval__ = new VBoxLayout(parent);
    }
    return retval__->luaInit(L, retval__, "mimas.VBoxLayout");
  } catch (std::exception &e) {
    lua_pushfstring(L, "VBoxLayout: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "VBoxLayout: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int VBoxLayout_destructor(lua_State *L) {
  VBoxLayout **userdata = (VBoxLayout**)dubL_checksdata_n(L, 1, "mimas.VBoxLayout");

  
  // custom destructor
  VBoxLayout *self = *userdata;
  if (self) self->luaDestroy();
  
  *userdata = NULL;
  return 0;
}



// test if class is deleted
static int VBoxLayout_deleted(lua_State *L) {
  VBoxLayout **userdata = (VBoxLayout**)dubL_checksdata_n(L, 1, "mimas.VBoxLayout");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int VBoxLayout__tostring(lua_State *L) {
  VBoxLayout **userdata = (VBoxLayout**)dubL_checksdata_n(L, 1, "mimas.VBoxLayout");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.VBoxLayout: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.VBoxLayout: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::VBoxLayout::addLayout(QLayout *layout, int stretch=0)
 * include/mimas/VBoxLayout.h:72
 */
static int VBoxLayout_addLayout(lua_State *L) {
  try {
    VBoxLayout *self = *((VBoxLayout**)dubL_checksdata(L, 1, "mimas.VBoxLayout"));
    if (!self) throw dub::Exception("Using deleted mimas.VBoxLayout in addLayout");
    int top__ = lua_gettop(L);
    QLayout *layout = *((QLayout **)dubL_checksdata(L, 2, "mimas.QLayout"));
    if (top__ < 3) {
      self->addLayout(layout);
    } else {
      int stretch = dubL_checkint(L, 3);
      self->addLayout(layout, stretch);
    }
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "addLayout: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "addLayout: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::VBoxLayout::addSpacing(int size)
 * include/mimas/VBoxLayout.h:100
 */
static int VBoxLayout_addSpacing(lua_State *L) {
  try {
    VBoxLayout *self = *((VBoxLayout**)dubL_checksdata(L, 1, "mimas.VBoxLayout"));
    if (!self) throw dub::Exception("Using deleted mimas.VBoxLayout in addSpacing");
    int size = dubL_checkint(L, 2);
    self->addSpacing(size);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "addSpacing: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "addSpacing: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::VBoxLayout::addStretch(int stretch=1)
 * include/mimas/VBoxLayout.h:94
 */
static int VBoxLayout_addStretch(lua_State *L) {
  try {
    VBoxLayout *self = *((VBoxLayout**)dubL_checksdata(L, 1, "mimas.VBoxLayout"));
    if (!self) throw dub::Exception("Using deleted mimas.VBoxLayout in addStretch");
    int top__ = lua_gettop(L);
    if (top__ < 2) {
      self->addStretch();
    } else {
      int stretch = dubL_checkint(L, 2);
      self->addStretch(stretch);
    }
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "addStretch: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "addStretch: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::VBoxLayout::addWidget(QWidget *widget, int stretch=0, int alignment=0)
 * include/mimas/VBoxLayout.h:60
 */
static int VBoxLayout_addWidget(lua_State *L) {
  try {
    VBoxLayout *self = *((VBoxLayout**)dubL_checksdata(L, 1, "mimas.VBoxLayout"));
    if (!self) throw dub::Exception("Using deleted mimas.VBoxLayout in addWidget");
    int top__ = lua_gettop(L);
    QWidget *widget = *((QWidget **)dubL_checksdata(L, 2, "mimas.QWidget"));
    if (top__ < 3) {
      self->addWidget(widget);
    } else {
      int stretch = dubL_checkint(L, 3);
      if (top__ < 4) {
        self->addWidget(widget, stretch);
      } else {
        int alignment = dubL_checkint(L, 4);
        self->addWidget(widget, stretch, alignment);
      }
    }
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "addWidget: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "addWidget: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::VBoxLayout::insertLayout(int pos, QLayout *layout, int stretch=0)
 * include/mimas/VBoxLayout.h:76
 */
static int VBoxLayout_insertLayout(lua_State *L) {
  try {
    VBoxLayout *self = *((VBoxLayout**)dubL_checksdata(L, 1, "mimas.VBoxLayout"));
    if (!self) throw dub::Exception("Using deleted mimas.VBoxLayout in insertLayout");
    int top__ = lua_gettop(L);
    int pos = dubL_checkint(L, 2);
    QLayout *layout = *((QLayout **)dubL_checksdata(L, 3, "mimas.QLayout"));
    if (top__ < 4) {
      self->insertLayout(pos, layout);
    } else {
      int stretch = dubL_checkint(L, 4);
      self->insertLayout(pos, layout, stretch);
    }
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "insertLayout: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "insertLayout: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::VBoxLayout::insertWidget(int pos, QWidget *widget, int stretch=0, int alignment=0)
 * include/mimas/VBoxLayout.h:64
 */
static int VBoxLayout_insertWidget(lua_State *L) {
  try {
    VBoxLayout *self = *((VBoxLayout**)dubL_checksdata(L, 1, "mimas.VBoxLayout"));
    if (!self) throw dub::Exception("Using deleted mimas.VBoxLayout in insertWidget");
    int top__ = lua_gettop(L);
    int pos = dubL_checkint(L, 2);
    QWidget *widget = *((QWidget **)dubL_checksdata(L, 3, "mimas.QWidget"));
    if (top__ < 4) {
      self->insertWidget(pos, widget);
    } else {
      int stretch = dubL_checkint(L, 4);
      if (top__ < 5) {
        self->insertWidget(pos, widget, stretch);
      } else {
        int alignment = dubL_checkint(L, 5);
        self->insertWidget(pos, widget, stretch, alignment);
      }
    }
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "insertWidget: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "insertWidget: Unknown exception");
  }
  return lua_error(L);
}



/** QLayout* mimas::VBoxLayout::layout()
 * include/mimas/VBoxLayout.h:121
 */
static int VBoxLayout_layout(lua_State *L) {
  try {
    VBoxLayout *self = *((VBoxLayout**)dubL_checksdata(L, 1, "mimas.VBoxLayout"));
    if (!self) throw dub::Exception("Using deleted mimas.VBoxLayout in layout");
    QLayout * retval__ = self->layout();
    lua_pushclass<QLayout>(L, retval__, "mimas.QLayout");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "layout: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "layout: Unknown exception");
  }
  return lua_error(L);
}



/** LuaStackSize mimas::VBoxLayout::minimumSize(lua_State *L) const 
 * include/mimas/VBoxLayout.h:114
 */
static int VBoxLayout_minimumSize(lua_State *L) {
  try {
    VBoxLayout *self = *((VBoxLayout**)dubL_checksdata(L, 1, "mimas.VBoxLayout"));
    if (!self) throw dub::Exception("Using deleted mimas.VBoxLayout in minimumSize");
    
    LuaStackSize  retval__ = self->minimumSize(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "minimumSize: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "minimumSize: Unknown exception");
  }
  return lua_error(L);
}



/** bool mimas::VBoxLayout::setAlignment(QWidget *w, int alignment)
 * include/mimas/VBoxLayout.h:88
 */
static int VBoxLayout_setAlignment(lua_State *L) {
  try {
    VBoxLayout *self = *((VBoxLayout**)dubL_checksdata(L, 1, "mimas.VBoxLayout"));
    if (!self) throw dub::Exception("Using deleted mimas.VBoxLayout in setAlignment");
    QWidget *w = *((QWidget **)dubL_checksdata(L, 2, "mimas.QWidget"));
    int alignment = dubL_checkint(L, 3);
    bool  retval__ = self->setAlignment(w, alignment);
    lua_pushboolean(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setAlignment: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setAlignment: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::VBoxLayout::setContentsMargins(int left, int top, int right, int bottom)
 * include/mimas/VBoxLayout.h:110
 */
static int VBoxLayout_setContentsMargins(lua_State *L) {
  try {
    VBoxLayout *self = *((VBoxLayout**)dubL_checksdata(L, 1, "mimas.VBoxLayout"));
    if (!self) throw dub::Exception("Using deleted mimas.VBoxLayout in setContentsMargins");
    int left = dubL_checkint(L, 2);
    int top = dubL_checkint(L, 3);
    int right = dubL_checkint(L, 4);
    int bottom = dubL_checkint(L, 5);
    self->setContentsMargins(left, top, right, bottom);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setContentsMargins: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setContentsMargins: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::VBoxLayout::setSpacing(int space)
 * include/mimas/VBoxLayout.h:106
 */
static int VBoxLayout_setSpacing(lua_State *L) {
  try {
    VBoxLayout *self = *((VBoxLayout**)dubL_checksdata(L, 1, "mimas.VBoxLayout"));
    if (!self) throw dub::Exception("Using deleted mimas.VBoxLayout in setSpacing");
    int space = dubL_checkint(L, 2);
    self->setSpacing(space);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setSpacing: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setSpacing: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg VBoxLayout_member_methods[] = {
  {"addLayout"         , VBoxLayout_addLayout},
  {"addSpacing"        , VBoxLayout_addSpacing},
  {"addStretch"        , VBoxLayout_addStretch},
  {"addWidget"         , VBoxLayout_addWidget},
  {"insertLayout"      , VBoxLayout_insertLayout},
  {"insertWidget"      , VBoxLayout_insertWidget},
  {"layout"            , VBoxLayout_layout},
  {"minimumSize"       , VBoxLayout_minimumSize},
  {"setAlignment"      , VBoxLayout_setAlignment},
  {"setContentsMargins", VBoxLayout_setContentsMargins},
  {"setSpacing"        , VBoxLayout_setSpacing},
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
