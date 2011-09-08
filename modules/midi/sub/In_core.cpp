#include "midi/In.h"

#include "lua_cpp_helper.h"





using namespace midi;


/* ============================ Constructors     ====================== */

/** midi::In::In()
 * include/midi/In.h:51
 */
static int In_In(lua_State *L) {
  try {
    In * retval__ = new In();
    // The class inherits from 'LuaCallback', use lua_init instead of pushclass.
    return retval__->luaInit(L, retval__, "midi.In");
  } catch (std::exception &e) {
    lua_pushfstring(L, "In: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "In: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int In_destructor(lua_State *L) {
  In **userdata = (In**)dubL_checksdata_n(L, 1, "midi.In");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int In__tostring(lua_State *L) {
  In **userdata = (In**)dubL_checksdata_n(L, 1, "midi.In");
  
  
  lua_pushfstring(L, "<midi.In: %p %s (%f)>", *userdata, (*userdata)->port_name(), (*userdata)->port());
  
  return 1;
}

/* ============================ Member Methods   ====================== */



/** void midi::In::open_port(int port, lua_State *L)
 * include/midi/In.h:73
 */
static int In_open_port1(lua_State *L) {
  try {
    In *self = *((In**)dubL_checksdata(L, 1, "midi.In"));
    int port = dubL_checkint(L, 2);
    
    self->open_port(port, L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "open_port: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "open_port: Unknown exception");
  }
  return lua_error(L);
}



/** void midi::In::open_port(const char *port_name, lua_State *L)
 * include/midi/In.h:91
 */
static int In_open_port2(lua_State *L) {
  try {
    In *self = *((In**)dubL_checksdata(L, 1, "midi.In"));
    const char *port_name = dubL_checkstring(L, 2);
    
    self->open_port(port_name, L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "open_port: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "open_port: Unknown exception");
  }
  return lua_error(L);
}




/** Overloaded function chooser for open_port(...) */
static int In_open_port(lua_State *L) {
  int type__ = lua_type(L, 2);
  if (type__ == LUA_TSTRING) {
    return In_open_port2(L);
  } else if (type__ == LUA_TNUMBER) {
    return In_open_port1(L);
  } else {
    // use any to raise errors
    return In_open_port1(L);
  }
}


/** int midi::In::port() const 
 * include/midi/In.h:65
 */
static int In_port(lua_State *L) {
  try {
    In *self = *((In**)dubL_checksdata(L, 1, "midi.In"));
    int  retval__ = self->port();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "port: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "port: Unknown exception");
  }
  return lua_error(L);
}



/** const char* midi::In::port_name() const 
 * include/midi/In.h:69
 */
static int In_port_name(lua_State *L) {
  try {
    In *self = *((In**)dubL_checksdata(L, 1, "midi.In"));
    const char * retval__ = self->port_name();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "port_name: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "port_name: Unknown exception");
  }
  return lua_error(L);
}



/** void midi::In::virtual_port(const char *port_name, lua_State *L)
 * include/midi/In.h:106
 */
static int In_virtual_port(lua_State *L) {
  try {
    In *self = *((In**)dubL_checksdata(L, 1, "midi.In"));
    const char *port_name = dubL_checkstring(L, 2);
    
    self->virtual_port(port_name, L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "virtual_port: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "virtual_port: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg In_member_methods[] = {
  {"open_port"         , In_open_port},
  {"port"              , In_port},
  {"port_name"         , In_port_name},
  {"virtual_port"      , In_virtual_port},
  {"__tostring"        , In__tostring},
  {"__gc"              , In_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg In_namespace_methods[] = {
  {"In"                , In_In},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_midi_In_core(lua_State *L) {
#else
extern "C" int luaopen_midi_In_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "midi.In");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, In_member_methods);
  // save meta-table in midi.In_
  register_mt(L, "midi", "In");

  // register class methods in a global namespace table
  luaL_register(L, "midi", In_namespace_methods);


	return 1;
}
