#include "mimas/HBoxLayout.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::HBoxLayout::HBoxLayout(Widget *widget)
 * include/mimas/HBoxLayout.h:49
 */
static int HBoxLayout_HBoxLayout(lua_State *L) {
  try {
    Widget *widget = *((Widget **)luaL_checkudata(L, 1, "mimas.Widget"));
    HBoxLayout * retval__ = new HBoxLayout(widget);
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


/** void mimas::HBoxLayout::addWidget(PushButton *widget)
 * include/mimas/HBoxLayout.h:53
 */
static int HBoxLayout_addWidget(lua_State *L) {
  try {
    HBoxLayout *self__ = *((HBoxLayout**)luaL_checkudata(L, 1, "mimas.HBoxLayout"));
    lua_remove(L, 1);
    PushButton *widget = *((PushButton **)luaL_checkudata(L, 1, "mimas.PushButton"));
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




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg HBoxLayout_member_methods[] = {
  {"addWidget"         , HBoxLayout_addWidget},
  {"__tostring"        , HBoxLayout__tostring},
  {"__gc"              , HBoxLayout_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg HBoxLayout_namespace_methods[] = {
  {"HBoxLayout"        , HBoxLayout_HBoxLayout},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_HBoxLayout(lua_State *L) {
#else
extern "C" int luaopen_mimas_HBoxLayout(lua_State *L) {
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
