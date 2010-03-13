
#include "draw.h"

#include "lua_cpp_helper.h"

using namespace cv;



/** float cv::rk_mat_at(const cv::Mat &m, uint x, uint y, uint c=0)
 * rubyk/support/draw.h:78
 */
static int cv_rk_mat_at(lua_State *L) {
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
}



// Register namespace


static const struct luaL_Reg Mat_additions_functions[] = {
  {"at"                     , cv_rk_mat_at},
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
