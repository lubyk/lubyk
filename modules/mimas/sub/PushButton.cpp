#include "mimas/PushButton.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::PushButton::PushButton(const char *title=NULL)
 * include/mimas/PushButton.h:48
 */
static int PushButton_PushButton(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    PushButton * retval__;
    if (top__ < 1) {
      retval__ = new PushButton();
    } else {
      const char *title = luaL_checkstring(L, 1);
      retval__ = new PushButton(title);
    }
    lua_pushclass<PushButton>(L, retval__, "mimas.PushButton");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.PushButton.PushButton: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.PushButton.PushButton: Unknown exception");
    lua_error(L);
    return 0;
  }
}

/* ============================ Destructor       ====================== */

static int PushButton_destructor(lua_State *L) {
  PushButton **userdata = (PushButton**)luaL_checkudata(L, 1, "mimas.PushButton");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int PushButton__tostring(lua_State *L) {
  PushButton **userdata = (PushButton**)luaL_checkudata(L, 1, "mimas.PushButton");
  
  lua_pushfstring(L, "<mimas.PushButton: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** QObject* mimas::PushButton::object()
 * include/mimas/PushButton.h:56
 */
static int PushButton_object(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    lua_remove(L, 1);
    QObject * retval__ = self__->object();
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.PushButton.object: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.PushButton.object: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** QWidget* mimas::PushButton::widget()
 * include/mimas/PushButton.h:52
 */
static int PushButton_widget(lua_State *L) {
  try {
    PushButton *self__ = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    lua_remove(L, 1);
    QWidget * retval__ = self__->widget();
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.PushButton.widget: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.PushButton.widget: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg PushButton_member_methods[] = {
  {"object"            , PushButton_object},
  {"widget"            , PushButton_widget},
  {"__tostring"        , PushButton__tostring},
  {"__gc"              , PushButton_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg PushButton_namespace_methods[] = {
  {"PushButton"        , PushButton_PushButton},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_PushButton(lua_State *L) {
#else
extern "C" int luaopen_mimas_PushButton(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.PushButton");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, PushButton_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "mimas", PushButton_namespace_methods);


	return 1;
}
