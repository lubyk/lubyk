#include "mimas/Label.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Label::Label(const char *title=NULL)
 * include/mimas/Label.h:46
 */
static int Label_Label(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Label * retval__;
    if (top__ < 1) {
      retval__ = new Label();
    } else {
      const char *title = luaL_checkstring(L, 1);
      retval__ = new Label(title);
    }
    lua_pushclass<Label>(L, retval__, "mimas.Label");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Label.Label: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Label.Label: Unknown exception");
    lua_error(L);
    return 0;
  }
}

/* ============================ Destructor       ====================== */

static int Label_destructor(lua_State *L) {
  Label **userdata = (Label**)luaL_checkudata(L, 1, "mimas.Label");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Label__tostring(lua_State *L) {
  Label **userdata = (Label**)luaL_checkudata(L, 1, "mimas.Label");
  
  lua_pushfstring(L, "<mimas.Label: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Label_member_methods[] = {
  {"__tostring"        , Label__tostring},
  {"__gc"              , Label_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Label_namespace_methods[] = {
  {"Label"             , Label_Label},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Label(lua_State *L) {
#else
extern "C" int luaopen_mimas_Label(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Label");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Label_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Label_namespace_methods);


	return 1;
}
