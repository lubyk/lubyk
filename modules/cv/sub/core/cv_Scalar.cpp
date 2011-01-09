#include "cv/cv.h"

#include "lua_cpp_helper.h"


using namespace cv;


/* ============================ Constructors     ====================== */


/** cv::Scalar< _Tp >::Scalar()
 * include/opencv/cxcore.hpp:427
 */
static int Scalar_Scalar1(lua_State *L) {
  try {
    Scalar * retval__ = new Scalar();
    lua_pushclass<Scalar>(L, retval__, "cv.Scalar");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Scalar.Scalar: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Scalar.Scalar: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Scalar.Scalar: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** cv::Scalar< _Tp >::Scalar(_Tp v0, _Tp v1, _Tp v2=0, _Tp v3=0)
 * include/opencv/cxcore.hpp:428
 */
static int Scalar_Scalar2(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    Scalar * retval__;
    double v0 = luaL_checknumber(L, 1);
    double v1 = luaL_checknumber(L, 2);
    if (top__ < 3) {
      retval__ = new Scalar(v0, v1);
    } else {
      double v2 = luaL_checknumber(L, 3);
      if (top__ < 4) {
        retval__ = new Scalar(v0, v1, v2);
      } else {
        double v3 = luaL_checknumber(L, 4);
        retval__ = new Scalar(v0, v1, v2, v3);
      }
    }
    lua_pushclass<Scalar>(L, retval__, "cv.Scalar");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Scalar.Scalar: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Scalar.Scalar: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Scalar.Scalar: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** cv::Scalar< _Tp >::Scalar(const CvScalar &s)
 * include/opencv/cxcore.hpp:429
 */
static int Scalar_Scalar3(lua_State *L) {
  try {
    const CvScalar *s = *((const CvScalar **)luaL_checkudata(L, 1, "cv.CvScalar"));
    Scalar * retval__ = new Scalar(*s);
    lua_pushclass<Scalar>(L, retval__, "cv.Scalar");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Scalar.Scalar: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Scalar.Scalar: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Scalar.Scalar: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** cv::Scalar< _Tp >::Scalar(_Tp v0)
 * include/opencv/cxcore.hpp:430
 */
static int Scalar_Scalar4(lua_State *L) {
  try {
    double v0 = luaL_checknumber(L, 1);
    Scalar * retval__ = new Scalar(v0);
    lua_pushclass<Scalar>(L, retval__, "cv.Scalar");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Scalar.Scalar: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Scalar.Scalar: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Scalar.Scalar: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for Scalar(...) */
static int Scalar_Scalar(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TNUMBER) {
    type__ = lua_type(L, 2);
    if (type__ == LUA_TNUMBER) {
      return Scalar_Scalar2(L);
    } else if (top__ < 2) {
      return Scalar_Scalar4(L);
    } else {
      // use any to raise errors
      return Scalar_Scalar4(L);
    }
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.CvScalar")) {
    return Scalar_Scalar3(L);
  } else if (top__ < 1) {
    return Scalar_Scalar1(L);
  } else {
    // use any to raise errors
    return Scalar_Scalar1(L);
  }
}

/* ============================ Destructor       ====================== */

static int Scalar_destructor(lua_State *L) {
  Scalar **userdata = (Scalar**)luaL_checkudata(L, 1, "cv.Scalar");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Scalar__tostring(lua_State *L) {
  Scalar **userdata = (Scalar**)luaL_checkudata(L, 1, "cv.Scalar");

  lua_pushfstring(L, "<cv.Scalar: %p>", *userdata);

  return 1;
}

/* ============================ Member Methods   ====================== */


/** static Scalar<_Tp> cv::Scalar< _Tp >::all(_Tp v0)
 * include/opencv/cxcore.hpp:431
 */
static int Scalar_all(lua_State *L) {
  try {
    double v0 = luaL_checknumber(L, 1);
    Scalar  retval__ = Scalar::all(v0);
    lua_pushclass<Scalar>(L, retval__, "cv.Scalar");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Scalar.all: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Scalar.all: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Scalar.all: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** Scalar<_Tp> cv::Scalar< _Tp >::mul(const Scalar< _Tp > &t, double scale=1) const
 * include/opencv/cxcore.hpp:436
 */
static int Scalar_mul(lua_State *L) {
  try {
    Scalar *self__ = *((Scalar**)luaL_checkudata(L, 1, "cv.Scalar"));
    lua_remove(L, 1);
    int top__ = lua_gettop(L);
    Scalar  retval__;
    const Scalar *t = *((const Scalar **)luaL_checkudata(L, 1, "cv.Scalar"));
    if (top__ < 2) {
      retval__ = self__->mul(*t);
    } else {
      double scale = luaL_checknumber(L, 2);
      retval__ = self__->mul(*t, scale);
    }
    lua_pushclass<Scalar>(L, retval__, "cv.Scalar");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Scalar.mul: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Scalar.mul: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Scalar.mul: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Scalar_member_methods[] = {
  {"mul"               , Scalar_mul},
  {"__tostring"        , Scalar__tostring},
  {"__gc"              , Scalar_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Scalar_namespace_methods[] = {
  {"Scalar"            , Scalar_Scalar},
  {"Scalar_all"        , Scalar_all},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_cv_Scalar(lua_State *L) {
#else
extern "C" int luaopen_cv_Scalar(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "cv.Scalar");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Scalar_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "cv", Scalar_namespace_methods);


}
