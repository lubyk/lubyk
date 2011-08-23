#include "midi/Out.h"

#include "lua_cpp_helper.h"


using namespace midi;


/* ============================ Constructors     ====================== */

/** midi::Out::Out(lubyk::Worker *worker)
 * include/midi/Out.h:51
 */
static int Out_Out(lua_State *L) {
  try {
    lubyk::Worker *worker = *((lubyk::Worker **)dubL_checksdata(L, 1, "lubyk.Worker"));
    Out * retval__ = new Out(worker);
    lua_pushclass<Out>(L, retval__, "midi.Out");
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "midi.Out.Out: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "midi.Out.Out: Unknown exception");
  }
  return lua_error(L);
}



/* ============================ Destructor       ====================== */

static int Out_destructor(lua_State *L) {
  Out **userdata = (Out**)dubL_checksdata_n(L, 1, "midi.Out");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Out__tostring(lua_State *L) {
  Out **userdata = (Out**)dubL_checksdata_n(L, 1, "midi.Out");
  
  
  lua_pushfstring(L, "<midi.Out: %p %s (%f)>", *userdata, (*userdata)->port_name(), (*userdata)->port());
  
  return 1;
}

/* ============================ Member Methods   ====================== */



/** void midi::Out::open_port(int port, lua_State *L)
 * include/midi/Out.h:69
 */
static int Out_open_port1(lua_State *L) {
  try {
    Out *self__ = *((Out**)dubL_checksdata(L, 1, "midi.Out"));
    int port = dubL_checkint(L, 2);
    
    self__->open_port(port, L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "midi.Out.open_port: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "midi.Out.open_port: Unknown exception");
  }
  return lua_error(L);
}



/** void midi::Out::open_port(const char *port_name, lua_State *L)
 * include/midi/Out.h:87
 */
static int Out_open_port2(lua_State *L) {
  try {
    Out *self__ = *((Out**)dubL_checksdata(L, 1, "midi.Out"));
    const char *port_name = dubL_checkstring(L, 2);
    
    self__->open_port(port_name, L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "midi.Out.open_port: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "midi.Out.open_port: Unknown exception");
  }
  return lua_error(L);
}




/** Overloaded function chooser for open_port(...) */
static int Out_open_port(lua_State *L) {
  int type__ = lua_type(L, 2);
  if (type__ == LUA_TSTRING) {
    return Out_open_port2(L);
  } else if (type__ == LUA_TNUMBER) {
    return Out_open_port1(L);
  } else {
    // use any to raise errors
    return Out_open_port1(L);
  }
}


/** int midi::Out::port() const 
 * include/midi/Out.h:61
 */
static int Out_port(lua_State *L) {
  try {
    Out *self__ = *((Out**)dubL_checksdata(L, 1, "midi.Out"));
    int  retval__ = self__->port();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "midi.Out.port: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "midi.Out.port: Unknown exception");
  }
  return lua_error(L);
}



/** const char* midi::Out::port_name() const 
 * include/midi/Out.h:65
 */
static int Out_port_name(lua_State *L) {
  try {
    Out *self__ = *((Out**)dubL_checksdata(L, 1, "midi.Out"));
    const char * retval__ = self__->port_name();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "midi.Out.port_name: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "midi.Out.port_name: Unknown exception");
  }
  return lua_error(L);
}



/** void midi::Out::send(int a, int b, int c)
 * include/midi/Out.h:107
 */
static int Out_send(lua_State *L) {
  try {
    Out *self__ = *((Out**)dubL_checksdata(L, 1, "midi.Out"));
    int a = dubL_checkint(L, 2);
    int b = dubL_checkint(L, 3);
    int c = dubL_checkint(L, 4);
    self__->send(a, b, c);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "midi.Out.send: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "midi.Out.send: Unknown exception");
  }
  return lua_error(L);
}



/** void midi::Out::virtual_port(const char *port_name, lua_State *L)
 * include/midi/Out.h:102
 */
static int Out_virtual_port(lua_State *L) {
  try {
    Out *self__ = *((Out**)dubL_checksdata(L, 1, "midi.Out"));
    const char *port_name = dubL_checkstring(L, 2);
    
    self__->virtual_port(port_name, L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "midi.Out.virtual_port: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "midi.Out.virtual_port: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Out_member_methods[] = {
  {"open_port"         , Out_open_port},
  {"port"              , Out_port},
  {"port_name"         , Out_port_name},
  {"send"              , Out_send},
  {"virtual_port"      , Out_virtual_port},
  {"__tostring"        , Out__tostring},
  {"__gc"              , Out_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Out_namespace_methods[] = {
  {"Out"               , Out_Out},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_midi_Out_core(lua_State *L) {
#else
extern "C" int luaopen_midi_Out_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "midi.Out");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Out_member_methods);
  // save meta-table in midi.Out_
  register_mt(L, "midi", "Out");

  // register class methods in a global namespace table
  luaL_register(L, "midi", Out_namespace_methods);


	return 1;
}
