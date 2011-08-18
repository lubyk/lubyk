#include "cv/cv.h"

#include "lua_cpp_helper.h"


using namespace cv;


/* ============================ Constructors     ====================== */


/** cv::Point2i< _Tp >::Point2i()
 * include/opencv/cxcore.hpp:295
 */
static int Point_Point1(lua_State *L) {
  try {
    Point * retval__ = new Point();
    lua_pushclass<Point>(L, retval__, "cv.Point");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Point.Point: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Point.Point: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Point.Point: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** cv::Point2i< _Tp >::Point2i(_Tp _x, _Tp _y)
 * include/opencv/cxcore.hpp:296
 */
static int Point_Point2(lua_State *L) {
  try {
    int _x = dubL_checkint(L, 1);
    int _y = dubL_checkint(L, 2);
    Point * retval__ = new Point(_x, _y);
    lua_pushclass<Point>(L, retval__, "cv.Point");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Point.Point: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Point.Point: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Point.Point: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** cv::Point2i< _Tp >::Point2i(const Point2i &pt)
 * include/opencv/cxcore.hpp:297
 */
static int Point_Point3(lua_State *L) {
  try {
    const Point2i *pt = *((const Point2i **)dubL_checkudata(L, 1, "cv.Point2i"));
    Point * retval__ = new Point(*pt);
    lua_pushclass<Point>(L, retval__, "cv.Point");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Point.Point: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Point.Point: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Point.Point: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** cv::Point2i< _Tp >::Point2i(const CvPoint &pt)
 * include/opencv/cxcore.hpp:298
 */
static int Point_Point4(lua_State *L) {
  try {
    const CvPoint *pt = *((const CvPoint **)dubL_checkudata(L, 1, "cv.CvPoint"));
    Point * retval__ = new Point(*pt);
    lua_pushclass<Point>(L, retval__, "cv.Point");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Point.Point: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Point.Point: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Point.Point: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** cv::Point2i< _Tp >::Point2i(const CvPoint2D32f &pt)
 * include/opencv/cxcore.hpp:299
 */
static int Point_Point5(lua_State *L) {
  try {
    const CvPoint2D32f *pt = *((const CvPoint2D32f **)dubL_checkudata(L, 1, "cv.CvPoint2D32f"));
    Point * retval__ = new Point(*pt);
    lua_pushclass<Point>(L, retval__, "cv.Point");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Point.Point: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Point.Point: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Point.Point: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** cv::Point2i< _Tp >::Point2i(const Size_< _Tp > &sz)
 * include/opencv/cxcore.hpp:300
 */
static int Point_Point6(lua_State *L) {
  try {
    const Size *sz = *((const Size **)dubL_checkudata(L, 1, "cv.Size"));
    Point * retval__ = new Point(*sz);
    lua_pushclass<Point>(L, retval__, "cv.Point");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Point.Point: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Point.Point: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Point.Point: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** cv::Point2i< _Tp >::Point2i(const Vec< _Tp, 2 > &v)
 * include/opencv/cxcore.hpp:301
 */
static int Point_Point7(lua_State *L) {
  try {
    const Vec2i *v = *((const Vec2i **)dubL_checkudata(L, 1, "cv.Vec2i"));
    Point * retval__ = new Point(*v);
    lua_pushclass<Point>(L, retval__, "cv.Point");
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Point.Point: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Point.Point: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Point.Point: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for Point(...) */
static int Point_Point(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.CvPoint")) {
    return Point_Point4(L);
  } else if (type__ == LUA_TNUMBER) {
    return Point_Point2(L);
  } else if (top__ < 1) {
    return Point_Point1(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Vec2i")) {
    return Point_Point7(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Point2i")) {
    return Point_Point3(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Size")) {
    return Point_Point6(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.CvPoint2D32f")) {
    return Point_Point5(L);
  } else {
    // use any to raise errors
    return Point_Point5(L);
  }
}

/* ============================ Destructor       ====================== */

static int Point_destructor(lua_State *L) {
  Point **userdata = (Point**)dubL_checkudata(L, 1, "cv.Point");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Point__tostring(lua_State *L) {
  Point **userdata = (Point**)dubL_checkudata(L, 1, "cv.Point");

  lua_pushfstring(L, "<cv.Point: %p>", *userdata);

  return 1;
}

/* ============================ Member Methods   ====================== */


/** double cv::Point2i< _Tp >::ddot(const Point2i &pt) const
 * include/opencv/cxcore.hpp:309
 */
static int Point_ddot(lua_State *L) {
  try {
    Point *self__ = *((Point**)dubL_checkudata(L, 1, "cv.Point"));
    lua_remove(L, 1);
    const Point2i *pt = *((const Point2i **)dubL_checkudata(L, 1, "cv.Point2i"));
    double  retval__ = self__->ddot(*pt);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Point.ddot: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Point.ddot: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Point.ddot: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** _Tp cv::Point2i< _Tp >::dot(const Point2i &pt) const
 * include/opencv/cxcore.hpp:308
 */
static int Point_dot(lua_State *L) {
  try {
    Point *self__ = *((Point**)dubL_checkudata(L, 1, "cv.Point"));
    lua_remove(L, 1);
    const Point2i *pt = *((const Point2i **)dubL_checkudata(L, 1, "cv.Point2i"));
    int  retval__ = self__->dot(*pt);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Point.dot: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Point.dot: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Point.dot: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** bool cv::Point2i< _Tp >::inside(const Rect_< _Tp > &r) const
 * include/opencv/cxcore.hpp:310
 */
static int Point_inside(lua_State *L) {
  try {
    Point *self__ = *((Point**)dubL_checkudata(L, 1, "cv.Point"));
    lua_remove(L, 1);
    const Rect *r = *((const Rect **)dubL_checkudata(L, 1, "cv.Rect"));
    bool  retval__ = self__->inside(*r);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (cv::Exception &e) {
    std::string *s = new std::string("cv.Point.inside: failed (");
    s->append(e.err);
    s->append(")");
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Point.inside: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Point.inside: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Point_member_methods[] = {
  {"ddot"              , Point_ddot},
  {"dot"               , Point_dot},
  {"inside"            , Point_inside},
  {"__tostring"        , Point__tostring},
  {"__gc"              , Point_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Point_namespace_methods[] = {
  {"Point"             , Point_Point},
  {"Point2i"           , Point_Point},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_cv_Point(lua_State *L) {
#else
extern "C" int luaopen_cv_Point(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "cv.Point");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Point_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "cv", Point_namespace_methods);


}
