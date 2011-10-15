#include "midi/In.h"

#include "lua_cpp_helper.h"





using namespace midi;


/* ============================ Constructors     ====================== */

/** midi::In::In()
 * include/midi/In.h:56
 */
static int In_In(lua_State *L) {
  try {
    In * retval__ = new In();
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

  
  // custom destructor
  In *self = *userdata;
  if (self) self->luaDestroy();
  
  *userdata = NULL;
  return 0;
}



// test if class is deleted
static int In_deleted(lua_State *L) {
  In **userdata = (In**)dubL_checksdata_n(L, 1, "midi.In");
  lua_pushboolean(L, *userdata == NULL);
  return 1;
}

/* ============================ tostring         ====================== */

static int In__tostring(lua_State *L) {
  In **userdata = (In**)dubL_checksdata_n(L, 1, "midi.In");
  
  if (!*userdata) {
    lua_pushstring(L, "<midi.In: NULL>");
    return 1;
  }
  
  
  lua_pushfstring(L, "<midi.In: %p %s (%f)>", *userdata, (*userdata)->portName(), (*userdata)->port());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** int lubyk::FifoMethods::fd()
 * lubyk/bind/Fifo.h:8
 */
static int FifoMethods_fd(lua_State *L) {
  try {
    In *self = *((In**)dubL_checksdata(L, 1, "midi.In"));
    if (!self) throw dub::Exception("Using deleted midi.In in fd");
    int  retval__ = self->fd();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "fd: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "fd: Unknown exception");
  }
  return lua_error(L);
}




/** void midi::In::openPort(int port, lua_State *L)
 * include/midi/In.h:74
 */
static int In_openPort1(lua_State *L) {
  try {
    In *self = *((In**)dubL_checksdata(L, 1, "midi.In"));
    if (!self) throw dub::Exception("Using deleted midi.In in openPort");
    int port = dubL_checkint(L, 2);
    
    self->openPort(port, L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "openPort: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "openPort: Unknown exception");
  }
  return lua_error(L);
}



/** void midi::In::openPort(const char *port_name, lua_State *L)
 * include/midi/In.h:92
 */
static int In_openPort2(lua_State *L) {
  try {
    In *self = *((In**)dubL_checksdata(L, 1, "midi.In"));
    if (!self) throw dub::Exception("Using deleted midi.In in openPort");
    const char *port_name = dubL_checkstring(L, 2);
    
    self->openPort(port_name, L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "openPort: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "openPort: Unknown exception");
  }
  return lua_error(L);
}




/** Overloaded function chooser for openPort(...) */
static int In_openPort(lua_State *L) {
  int type__ = lua_type(L, 2);
  if (type__ == LUA_TSTRING) {
    return In_openPort2(L);
  } else if (type__ == LUA_TNUMBER) {
    return In_openPort1(L);
  } else {
    // use any to raise errors
    return In_openPort1(L);
  }
}


/** LuaStackSize lubyk::FifoMethods::pop(lua_State *L)
 * lubyk/bind/Fifo.h:9
 */
static int FifoMethods_pop(lua_State *L) {
  try {
    In *self = *((In**)dubL_checksdata(L, 1, "midi.In"));
    if (!self) throw dub::Exception("Using deleted midi.In in pop");
    
    LuaStackSize  retval__ = self->pop(L);
    return retval__;
  } catch (std::exception &e) {
    lua_pushfstring(L, "pop: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "pop: Unknown exception");
  }
  return lua_error(L);
}



/** int midi::In::port() const 
 * include/midi/In.h:66
 */
static int In_port(lua_State *L) {
  try {
    In *self = *((In**)dubL_checksdata(L, 1, "midi.In"));
    if (!self) throw dub::Exception("Using deleted midi.In in port");
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



/** const char* midi::In::portName() const 
 * include/midi/In.h:70
 */
static int In_portName(lua_State *L) {
  try {
    In *self = *((In**)dubL_checksdata(L, 1, "midi.In"));
    if (!self) throw dub::Exception("Using deleted midi.In in portName");
    const char * retval__ = self->portName();
    lua_pushstring(L, retval__);
    return 1;
  } catch (std::exception &e) {
    lua_pushfstring(L, "portName: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "portName: Unknown exception");
  }
  return lua_error(L);
}



/** void midi::In::virtualPort(const char *port_name, lua_State *L)
 * include/midi/In.h:107
 */
static int In_virtualPort(lua_State *L) {
  try {
    In *self = *((In**)dubL_checksdata(L, 1, "midi.In"));
    if (!self) throw dub::Exception("Using deleted midi.In in virtualPort");
    const char *port_name = dubL_checkstring(L, 2);
    
    self->virtualPort(port_name, L);
    return 0;
  } catch (std::exception &e) {
    lua_pushfstring(L, "virtualPort: %s", e.what());
  } catch (...) {
    lua_pushfstring(L, "virtualPort: Unknown exception");
  }
  return lua_error(L);
}





/* ============================ Lua Registration ====================== */

static const struct luaL_Reg In_member_methods[] = {
  {"fd"                , FifoMethods_fd},
  {"openPort"          , In_openPort},
  {"pop"               , FifoMethods_pop},
  {"port"              , In_port},
  {"portName"          , In_portName},
  {"virtualPort"       , In_virtualPort},
  {"__tostring"        , In__tostring},
  {"__gc"              , In_destructor},
  {"deleted"           , In_deleted},
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
