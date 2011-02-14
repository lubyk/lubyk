#include "mimas/Color.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */


/** mimas::Color::Color()
 * include/mimas/Color.h:45
 */
static int Color_Color1(lua_State *L) {
  try {
    Color * retval__ = new Color();
    lua_pushclass<Color>(L, retval__, "mimas.Color");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Color.Color: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Color.Color: Unknown exception");
  }
}


/** mimas::Color::Color(float h, float s=1.0, float v=1.0, float a=1.0)
 * include/mimas/Color.h:47
 */
static int Color_Color2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Color * retval__;
    float h = luaL_checknumber(L, 1);
    if (top__ < 2) {
      retval__ = new Color(h);
    } else {
      float s = luaL_checknumber(L, 2);
      if (top__ < 3) {
        retval__ = new Color(h, s);
      } else {
        float v = luaL_checknumber(L, 3);
        if (top__ < 4) {
          retval__ = new Color(h, s, v);
        } else {
          float a = luaL_checknumber(L, 4);
          retval__ = new Color(h, s, v, a);
        }
      }
    }
    lua_pushclass<Color>(L, retval__, "mimas.Color");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Color.Color: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Color.Color: Unknown exception");
  }
}



/** Overloaded function chooser for Color(...) */
static int Color_Color(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TNUMBER) {
    return Color_Color2(L);
  } else if (top__ < 1) {
    return Color_Color1(L);
  } else {
    // use any to raise errors
    return Color_Color1(L);
  }
}


/* ============================ Destructor       ====================== */

static int Color_destructor(lua_State *L) {
  Color **userdata = (Color**)luaL_checkudata(L, 1, "mimas.Color");

  
  if (*userdata) delete *userdata;
  
  *userdata = NULL;
  return 0;
}



/* ============================ tostring         ====================== */

static int Color__tostring(lua_State *L) {
  Color **userdata = (Color**)luaL_checkudata(L, 1, "mimas.Color");
  
  
  lua_pushfstring(L, "<mimas.Color: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** void mimas::Color::setHsva(float h, float s, float v, float a)
 * include/mimas/Color.h:59
 */
static int Color_setHsva(lua_State *L) {
  try {
    Color *self__ = *((Color**)luaL_checkudata(L, 1, "mimas.Color"));
    float h = luaL_checknumber(L, 2);
    float s = luaL_checknumber(L, 3);
    float v = luaL_checknumber(L, 4);
    float a = luaL_checknumber(L, 5);
    self__->setHsva(h, s, v, a);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Color.setHsva: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Color.setHsva: Unknown exception");
  }
}


/** void mimas::Color::setRgba(float r, float g, float b, float a=1.0)
 * include/mimas/Color.h:55
 */
static int Color_setRgba(lua_State *L) {
  try {
    Color *self__ = *((Color**)luaL_checkudata(L, 1, "mimas.Color"));
    int top__ = lua_gettop(L);
    float r = luaL_checknumber(L, 2);
    float g = luaL_checknumber(L, 3);
    float b = luaL_checknumber(L, 4);
    if (top__ < 5) {
      self__->setRgba(r, g, b);
    } else {
      float a = luaL_checknumber(L, 5);
      self__->setRgba(r, g, b, a);
    }
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.Color.setRgba: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.Color.setRgba: Unknown exception");
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Color_member_methods[] = {
  {"setHsva"           , Color_setHsva},
  {"setRgba"           , Color_setRgba},
  {"__tostring"        , Color__tostring},
  {"__gc"              , Color_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Color_namespace_methods[] = {
  {"Color"             , Color_Color},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Color(lua_State *L) {
#else
extern "C" int luaopen_mimas_Color(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Color");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Color_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Color_namespace_methods);


	return 1;
}
