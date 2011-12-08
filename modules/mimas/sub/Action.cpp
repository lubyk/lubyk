#include "mimas/Action.h"

#include "lua_cpp_helper.h"





using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Action::Action(const char *name, QObject *parent=0)
 * include/mimas/Action.h:50
 */
static int Action_Action(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Action * retval__;
    const char *name = dubL_checkstring(L, 1);
    if (top__ < 2) {
      retval__ = new Action(name);
    } else {
      QObject *parent = *((QObject **)dubL_checksdata(L, 2, "mimas.QObject"));
      retval__ = new Action(name, parent);
    }
    return retval__->luaInit(L, retval__, "mimas.Action");
  } catch (std::exception &e) {
    lua_pushfstring(L, "Action: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "Action: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Action_destructor(lua_State *L) {
  Action **userdata = (Action**)dubL_checksdata_n(L, 1, "mimas.Action");

  
  // custom destructor
  Action *self = *userdata;
  if (self) self->luaDestroy();
  
  *userdata = NULL;
  return 0;
}



// test if class is deleted
static int Action_deleted(lua_State *L) {
  Action **userdata = (Action**)dubL_checksdata_n(L, 1, "mimas.Action");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int Action__tostring(lua_State *L) {
  Action **userdata = (Action**)dubL_checksdata_n(L, 1, "mimas.Action");
  
  if (!*userdata) {
    lua_pushstring(L, "<mimas.Action: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<mimas.Action: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::QObject::name()
 * mimas/bind/QObject.h:12
 */
static int QObject_name(lua_State *L) {
  try {
    Action *self = *((Action**)dubL_checksdata(L, 1, "mimas.Action"));
    if (!self) throw dub::Exception("Using deleted mimas.Action in name");
    lua_pushstring(L, self->objectName().toUtf8().data());
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "name: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "name: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QObject::object()
 * mimas/bind/QObject.h:11
 */
static int QObject_object(lua_State *L) {
  try {
    Action *self = *((Action**)dubL_checksdata(L, 1, "mimas.Action"));
    if (!self) throw dub::Exception("Using deleted mimas.Action in object");
    QObject * retval__ = self;
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "object: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "object: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Action::setMenuRole(int role)
 * include/mimas/Action.h:65
 */
static int Action_setMenuRole(lua_State *L) {
  try {
    Action *self = *((Action**)dubL_checksdata(L, 1, "mimas.Action"));
    if (!self) throw dub::Exception("Using deleted mimas.Action in setMenuRole");
    int role = dubL_checkint(L, 2);
    self->setMenuRole(role);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setMenuRole: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setMenuRole: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QObject::setName(const char *name)
 * mimas/bind/QObject.h:13
 */
static int QObject_setName(lua_State *L) {
  try {
    Action *self = *((Action**)dubL_checksdata(L, 1, "mimas.Action"));
    if (!self) throw dub::Exception("Using deleted mimas.Action in setName");
    const char *name = dubL_checkstring(L, 2);
    self->setObjectName(QString(name));
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setName: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setName: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::Action::setShortcut(const char *sequence)
 * include/mimas/Action.h:59
 */
static int Action_setShortcut(lua_State *L) {
  try {
    Action *self = *((Action**)dubL_checksdata(L, 1, "mimas.Action"));
    if (!self) throw dub::Exception("Using deleted mimas.Action in setShortcut");
    const char *sequence = dubL_checkstring(L, 2);
    self->setShortcut(sequence);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setShortcut: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setShortcut: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QAction::setText(const char *text)
 * mimas/bind/QAction.h:14
 */
static int QAction_setText(lua_State *L) {
  try {
    Action *self = *((Action**)dubL_checksdata(L, 1, "mimas.Action"));
    if (!self) throw dub::Exception("Using deleted mimas.Action in setText");
    const char *text = dubL_checkstring(L, 2);
    self->setText(text);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "setText: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "setText: Unknown exception");
  }
  return lua_error(L);
}



/** void mimas::QAction::text()
 * mimas/bind/QAction.h:13
 */
static int QAction_text(lua_State *L) {
  try {
    Action *self = *((Action**)dubL_checksdata(L, 1, "mimas.Action"));
    if (!self) throw dub::Exception("Using deleted mimas.Action in text");
    lua_pushstring(L, self->text().toUtf8().data());
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "text: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "text: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Action_member_methods[] = {
  {"name"              , QObject_name},
  {"object"            , QObject_object},
  {"setMenuRole"       , Action_setMenuRole},
  {"setName"           , QObject_setName},
  {"setShortcut"       , Action_setShortcut},
  {"setText"           , QAction_setText},
  {"text"              , QAction_text},
  {"__tostring"        , Action__tostring},
  {"__gc"              , Action_destructor},
  {"deleted"           , Action_deleted},
  {NULL, NULL},
};

static const struct luaL_Reg Action_namespace_methods[] = {
  {"Action"            , Action_Action},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Action(lua_State *L) {
#else
extern "C" int luaopen_mimas_Action(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Action");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Action_member_methods);
  // save meta-table in mimas.Action_
  register_mt(L, "mimas", "Action");

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Action_namespace_methods);


	return 1;
}
