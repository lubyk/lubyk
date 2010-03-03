#include "opencv/cv.h"

#include "lua_dub_helper.h"


using namespace cv;


/* ============================ Constructors     ====================== */


/** cv::Size2i::Size2i()
 * include/opencv/cxcore.hpp:348
 */
static int Size2i_Size2i1(lua_State *L) {
  Size2i * retval__    = new Size2i();
  lua_pushclass<Size2i>(L, retval__, "cv.Size");
  return 1;
}


/** cv::Size2i::Size2i(_Tp _width, _Tp _height)
 * include/opencv/cxcore.hpp:349
 */
static int Size2i_Size2i2(lua_State *L) {
  int _width           = luaL_checkint   (L, 1);
  int _height          = luaL_checkint   (L, 2);
  Size2i * retval__    = new Size2i(_width, _height);
  lua_pushclass<Size2i>(L, retval__, "cv.Size");
  return 1;
}


/** cv::Size2i::Size2i(const Size2i &sz)
 * include/opencv/cxcore.hpp:350
 */
static int Size2i_Size2i3(lua_State *L) {
  const Size2i *sz     = *((const Size2i **)luaL_checkudata(L, 1, "cv.Size"));
  Size2i * retval__    = new Size2i(*sz);
  lua_pushclass<Size2i>(L, retval__, "cv.Size");
  return 1;
}


/** cv::Size2i::Size2i(const CvSize &sz)
 * include/opencv/cxcore.hpp:351
 */
static int Size2i_Size2i4(lua_State *L) {
  const CvSize *sz     = *((const CvSize **)luaL_checkudata(L, 1, "cv.CvSize"));
  Size2i * retval__    = new Size2i(*sz);
  lua_pushclass<Size2i>(L, retval__, "cv.Size");
  return 1;
}


/** cv::Size2i::Size2i(const CvSize2D32f &sz)
 * include/opencv/cxcore.hpp:352
 */
static int Size2i_Size2i5(lua_State *L) {
  const CvSize2D32f *sz = *((const CvSize2D32f **)luaL_checkudata(L, 1, "cv.CvSize2D32f"));
  Size2i * retval__    = new Size2i(*sz);
  lua_pushclass<Size2i>(L, retval__, "cv.Size");
  return 1;
}


/** cv::Size2i::Size2i(const Point_< _Tp > &pt)
 * include/opencv/cxcore.hpp:353
 */
static int Size2i_Size2i6(lua_State *L) {
  const Point_<int> *pt     = *((const Point_<int> **)luaL_checkudata(L, 1, "cv.Point2i"));
  Size2i * retval__    = new Size2i(*pt);
  lua_pushclass<Size2i>(L, retval__, "cv.Size");
  return 1;
}



/** Overloaded function chooser for Size2i(...) */
static int Size2i_Size2i(lua_State *L) {
  int type__ = lua_type(L, 1);
  if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Point_")) {
    return Size2i_Size2i6(L);
  } else if (type__ == LUA_TNONE) {
    return Size2i_Size2i1(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.CvSize")) {
    return Size2i_Size2i4(L);
  } else if (type__ == LUA_TNUMBER) {
    return Size2i_Size2i2(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.CvSize2D32f")) {
    return Size2i_Size2i5(L);
  } else if (type__ == LUA_TUSERDATA && is_userdata(L, 1, "cv.Size")) {
    return Size2i_Size2i3(L);
  } else {
    // use any to raise errors
    return Size2i_Size2i3(L);
  }
}

/* ============================ Destructor       ====================== */

static int Size2i_destructor(lua_State *L) {
  Size2i **userdata = (Size2i**)luaL_checkudata(L, 1, "cv.Size");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int Size2i__tostring(lua_State *L) {
  Size2i **userdata = (Size2i**)luaL_checkudata(L, 1, "cv.Size");
  lua_pushfstring(L, "cv.Size: %p", *userdata);
  return 1;
}

/* ============================ Member Methods   ====================== */


/** _Tp cv::Size2i::area() const
 * include/opencv/cxcore.hpp:355
 */
static int Size2i_area(lua_State *L) {
  Size2i *self__       = *((Size2i**)luaL_checkudata(L, 1, "cv.Size"));
  lua_remove(L, 1);
  int  retval__        = self__->area();
  lua_pushnumber(L, retval__);
  return 1;
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg Size2i_member_methods[] = {
  {"area"              , Size2i_area},
  {"__tostring"        , Size2i__tostring},
  {"__gc"              , Size2i_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg Size2i_namespace_methods[] = {
  {"Size2i"            , Size2i_Size2i},
  {"Size"              , Size2i_Size2i},
  {NULL, NULL},
};



void luaopen_cv_Size2i(lua_State *L) {
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "cv.Size"); // "dub.Matrix"

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Size2i_member_methods);

  // register class methods in a global table like "dub"
  luaL_register(L, "cv", Size2i_namespace_methods);


}
