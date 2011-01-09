#if 0
#include "draw.h"

#include "lua_cpp_helper.h"

using namespace cv;


/** float cv::rk_mat_at(const cv::Mat &m, int x, int y, int c=0)
 * rubyk/support/draw.h:79
 */
static int cv_rk_mat_at(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    float  retval__;
    const cv::Mat *m = *((const cv::Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    int x = luaL_checkint(L, 2);
    int y = luaL_checkint(L, 3);
    if (top__ < 4) {
      retval__ = rk_mat_at(*m, x, y);
    } else {
      int c = luaL_checkint(L, 4);
      retval__ = rk_mat_at(*m, x, y, c);
    }
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.at: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.at: Unknown exception");
    lua_error(L);
    return 0;
  }
}

/** size_t cv::rk_mat_cols(const cv::Mat &m)
 * rubyk/support/draw.h:97
 */
static int cv_rk_mat_cols(lua_State *L) {
  try {
    const cv::Mat *m = *((const cv::Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    size_t  retval__ = rk_mat_cols(*m);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.cols: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.cols: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** size_t cv::rk_mat_height(const cv::Mat &m)
 * rubyk/support/draw.h:103
 */
static int cv_rk_mat_height(lua_State *L) {
  try {
    const cv::Mat *m = *((const cv::Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    size_t  retval__ = rk_mat_height(*m);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.height: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.height: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** size_t cv::rk_mat_rows(const cv::Mat &m)
 * rubyk/support/draw.h:109
 */
static int cv_rk_mat_rows(lua_State *L) {
  try {
    const cv::Mat *m = *((const cv::Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    size_t  retval__ = rk_mat_rows(*m);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.rows: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.rows: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** size_t cv::rk_mat_width(const cv::Mat &m)
 * rubyk/support/draw.h:91
 */
static int cv_rk_mat_width(lua_State *L) {
  try {
    const cv::Mat *m = *((const cv::Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    size_t  retval__ = rk_mat_width(*m);
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("cv.Mat.width: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "cv.Mat.width: Unknown exception");
    lua_error(L);
    return 0;
  }
}


// Register namespace


static const struct luaL_Reg Mat_additions_functions[] = {
  {"at"                     , cv_rk_mat_at},
  {"cols"                   , cv_rk_mat_cols},
  {"height"                 , cv_rk_mat_height},
  {"rows"                   , cv_rk_mat_rows},
  {"width"                  , cv_rk_mat_width},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_cv_additions(lua_State *L) {
#else
extern "C" int luaopen_cv_additions(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "cv.Mat");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, Mat_additions_functions);
}
#endif