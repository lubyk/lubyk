#include "mimas/Pen.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Pen::Pen()
 * include/mimas/Pen.h:45
 */
static int Pen_Pen(lua_State *L) {
  try {
    Pen * retval__ = new Pen();
    lua_pushclass<Pen>(L, retval__, "mimas.Pen");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Pen.Pen: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Pen.Pen: Unknown exception");
  }
}


/* ============================ Destructor       ====================== */

static int Pen_destructor(lua_State *L) {
  Pen **userdata = (Pen**)luaL_checkudata(L, 1, "mimas.Pen");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Pen__tostring(lua_State *L) {
  Pen **userdata = (Pen**)luaL_checkudata(L, 1, "mimas.Pen");
  
  
  lua_pushfstring(L, "<mimas.Pen: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::Pen::setColor(float r, float g, float b, float a)
 * include/mimas/Pen.h:59
 */
static int Pen_setColor(lua_State *L) {
  try {
    Pen *self__ = *((Pen**)luaL_checkudata(L, 1, "mimas.Pen"));
    float r = luaL_checknumber(L, 2);
    float g = luaL_checknumber(L, 3);
    float b = luaL_checknumber(L, 4);
    float a = luaL_checknumber(L, 5);
    self__->setColor(r, g, b, a);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Pen.setColor: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Pen.setColor: Unknown exception");
  }
}


/** void mimas::Pen::setHsva(float h, float s, float v, float a)
 * include/mimas/Pen.h:63
 */
static int Pen_setHsva(lua_State *L) {
  try {
    Pen *self__ = *((Pen**)luaL_checkudata(L, 1, "mimas.Pen"));
    float h = luaL_checknumber(L, 2);
    float s = luaL_checknumber(L, 3);
    float v = luaL_checknumber(L, 4);
    float a = luaL_checknumber(L, 5);
    self__->setHsva(h, s, v, a);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Pen.setHsva: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Pen.setHsva: Unknown exception");
  }
}


/** void mimas::Pen::setWidth(float w)
 * include/mimas/Pen.h:55
 */
static int Pen_setWidth(lua_State *L) {
  try {
    Pen *self__ = *((Pen**)luaL_checkudata(L, 1, "mimas.Pen"));
    float w = luaL_checknumber(L, 2);
    self__->setWidth(w);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Pen.setWidth: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Pen.setWidth: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Pen_member_methods[] = {
  {"setColor"          , Pen_setColor},
  {"setHsva"           , Pen_setHsva},
  {"setWidth"          , Pen_setWidth},
  {"__tostring"        , Pen__tostring},
  {"__gc"              , Pen_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Pen_namespace_methods[] = {
  {"Pen"               , Pen_Pen},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Pen(lua_State *L) {
#else
extern "C" int luaopen_mimas_Pen(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Pen");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Pen_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Pen_namespace_methods);


	return 1;
}
