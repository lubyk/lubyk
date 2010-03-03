#include "opencv/cv.h"

#include "lua_cpp_helper.h"


using namespace cv;


/* ============================ Constructors     ====================== */


/** cv::Point2i::Point2i()
 * include/opencv/cxcore.hpp:295
 */
static int Point_Point1(lua_State *L) {
  Point * retval__ = new Point();
  lua_pushclass<Point>(L, retval__, "cv.Point");
  return 1;
}


/** cv::Point2i::Point2i(_Tp _x, _Tp _y)
 * include/opencv/cxcore.hpp:296
 */
static int Point_Point2(lua_State *L) {
  int _x = luaL_checkint(L, 1);
  int _y = luaL_checkint(L, 2);
  Point * retval__ = new Point(_x, _y);
  lua_pushclass<Point>(L, retval__, "cv.Point");
  return 1;
}


/** cv::Point2i::Point2i(const Point2i &pt)
 * include/opencv/cxcore.hpp:297
 */
static int Point_Point3(lua_State *L) {
  const Point2i *pt = *((const Point2i **)luaL_checkudata(L, 1, "cv.Point2i"));
  Point * retval__ = new Point(*pt);
  lua_pushclass<Point>(L, retval__, "cv.Point");
  return 1;
}


/** cv::Point2i::Point2i(const CvPoint &pt)
 * include/opencv/cxcore.hpp:298
 */
static int Point_Point4(lua_State *L) {
  const CvPoint *pt = *((const CvPoint **)luaL_checkudata(L, 1, "cv.CvPoint"));
  Point * retval__ = new Point(*pt);
  lua_pushclass<Point>(L, retval__, "cv.Point");
  return 1;
}


/** cv::Point2i::Point2i(const CvPoint2D32f &pt)
 * include/opencv/cxcore.hpp:299
 */
static int Point_Point5(lua_State *L) {
  const CvPoint2D32f *pt = *((const CvPoint2D32f **)luaL_checkudata(L, 1, "cv.CvPoint2D32f"));
  Point * retval__ = new Point(*pt);
  lua_pushclass<Point>(L, retval__, "cv.Point");
  return 1;
}



/** Overloaded function chooser for Point(...) */
static int Point_Point(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.CvPoint")) {
    return Point_Point4(L);
  } else if (type__ == LUA_TNONE) {
    return Point_Point1(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Point2i")) {
    return Point_Point3(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.CvPoint2D32f")) {
    return Point_Point5(L);
  } else if (type__ == LUA_TNUMBER) {
    return Point_Point2(L);
  } else {
    // use any to raise errors
    return Point_Point2(L);
  }
}

/* ============================ Destructor       ====================== */

static int Point_destructor(lua_State *L) {
  Point **userdata = (Point**)luaL_checkudata(L, 1, "cv.Point");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Point__tostring(lua_State *L) {
  Point **userdata = (Point**)luaL_checkudata(L, 1, "cv.Point");
  Point *pt = *userdata;
  lua_pushfstring(L, "<cv.Point:%p %dx%d>", pt, pt->x, pt->y);
  return 1;
}

/* ============================ Member Methods   ====================== */


/** double cv::Point2i::ddot(const Point2i &pt) const
 * include/opencv/cxcore.hpp:309
 */
static int Point_ddot(lua_State *L) {
  Point *self__ = *((Point**)luaL_checkudata(L, 1, "cv.Point"));
  lua_remove(L, 1);
  const Point2i *pt = *((const Point2i **)luaL_checkudata(L, 1, "cv.Point2i"));
  double  retval__ = self__->ddot(*pt);
  lua_pushnumber(L, retval__);
  return 1;
}


/** _Tp cv::Point2i::dot(const Point2i &pt) const
 * include/opencv/cxcore.hpp:308
 */
static int Point_dot(lua_State *L) {
  Point *self__ = *((Point**)luaL_checkudata(L, 1, "cv.Point"));
  lua_remove(L, 1);
  const Point2i *pt = *((const Point2i **)luaL_checkudata(L, 1, "cv.Point2i"));
  int  retval__ = self__->dot(*pt);
  lua_pushnumber(L, retval__);
  return 1;
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Point_member_methods[] = {
  {"ddot"              , Point_ddot},
  {"dot"               , Point_dot},
  {"__tostring"        , Point__tostring},
  {"__gc"              , Point_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Point_namespace_methods[] = {
  {"Point"             , Point_Point},
  {"Point2i"           , Point_Point},
  {NULL, NULL},
};



void luaopen_cv_Point(lua_State *L) {
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "cv.Point"); // "dub.Matrix"

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Point_member_methods);

  // register class methods in a global table like "dub"
  luaL_register(L, "cv", Point_namespace_methods);


}
