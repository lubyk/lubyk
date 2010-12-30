#include "mimas/Slider.h"

#include "lua_cpp_helper.h"


using namespace mimas;


/* ============================ Constructors     ====================== */

/** mimas::Slider::Slider(rubyk::Worker *worker, int orientation=(int) Qt::Horizontal, QWidget *parent=0)
 * include/mimas/Slider.h:54
 */
static int Slider_Slider(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Slider * retval__;
    rubyk::Worker *worker = *((rubyk::Worker **)luaL_checkudata(L, 1, "rubyk.Worker"));
    if (top__ < 2) {
      retval__ = new Slider(worker);
    } else {
      int orientation = luaL_checkint(L, 2);
      if (top__ < 3) {
        retval__ = new Slider(worker, orientation);
      } else {
        QWidget *parent = *((QWidget **)luaL_checkudata(L, 3, "mimas.QWidget"));
        retval__ = new Slider(worker, orientation, parent);
      }
    }
    lua_pushclass<Slider>(L, retval__, "mimas.Slider");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.Slider: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.Slider: Unknown exception");
    lua_error(L);
    return 0;
  }
}

/* ============================ Destructor       ====================== */

static int Slider_destructor(lua_State *L) {
  Slider **userdata = (Slider**)luaL_checkudata(L, 1, "mimas.Slider");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Slider__tostring(lua_State *L) {
  Slider **userdata = (Slider**)luaL_checkudata(L, 1, "mimas.Slider");
  
  lua_pushfstring(L, "<mimas.Slider: %p>", *userdata);
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** QObject* mimas::Slider::object()
 * include/mimas/Slider.h:68
 */
static int Slider_object(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    lua_remove(L, 1);
    QObject * retval__ = self__->object();
    lua_pushclass<QObject>(L, retval__, "mimas.QObject");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.object: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.object: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void mimas::Slider::setValue(int value)
 * include/mimas/Slider.h:58
 */
static int Slider_setValue(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    lua_remove(L, 1);
    int value = luaL_checkint(L, 1);
    self__->setValue(value);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.setValue: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.setValue: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** QWidget* mimas::Slider::widget()
 * include/mimas/Slider.h:64
 */
static int Slider_widget(lua_State *L) {
  try {
    Slider *self__ = *((Slider**)luaL_checkudata(L, 1, "mimas.Slider"));
    lua_remove(L, 1);
    QWidget * retval__ = self__->widget();
    lua_pushclass<QWidget>(L, retval__, "mimas.QWidget");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("mimas.Slider.widget: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "mimas.Slider.widget: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Slider_member_methods[] = {
  {"object"            , Slider_object},
  {"setValue"          , Slider_setValue},
  {"widget"            , Slider_widget},
  {"__tostring"        , Slider__tostring},
  {"__gc"              , Slider_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Slider_namespace_methods[] = {
  {"Slider"            , Slider_Slider},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_Slider_core(lua_State *L) {
#else
extern "C" int luaopen_mimas_Slider_core(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "mimas.Slider");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Slider_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "mimas", Slider_namespace_methods);


	return 1;
}
