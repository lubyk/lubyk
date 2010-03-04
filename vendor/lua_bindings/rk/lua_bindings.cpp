#include "draw.h"

#include "lua_cpp_helper.h"

using namespace rk;



/** void rk::draw_matrix(const cv::Mat &mat, float start_x, float start_y, float end_x, float end_y, float alpha=1.0, float z_mul=0, bool strips=false)
 * rubyk/support/draw.h:47
 */
static int rk_draw_matrix(lua_State *L) {
  int top__ = lua_gettop(L);
  const cv::Mat *mat = *((const cv::Mat **)luaL_checkudata(L, 1, "cv.Mat"));
  float start_x = luaL_checknumber(L, 2);
  float start_y = luaL_checknumber(L, 3);
  float end_x = luaL_checknumber(L, 4);
  float end_y = luaL_checknumber(L, 5);
  if (top__ < 6) {
    draw_matrix(*mat, start_x, start_y, end_x, end_y);
  } else {
    float alpha = luaL_checknumber(L, 6);
    if (top__ < 7) {
      draw_matrix(*mat, start_x, start_y, end_x, end_y, alpha);
    } else {
      float z_mul = luaL_checknumber(L, 7);
      if (top__ < 8) {
        draw_matrix(*mat, start_x, start_y, end_x, end_y, alpha, z_mul);
      } else {
        bool strips = lua_toboolean(L, 8);
        draw_matrix(*mat, start_x, start_y, end_x, end_y, alpha, z_mul, strips);
      }
    }
  }
  return 0;
}



// Register namespace


static const struct luaL_Reg rk_functions[] = {
  {"draw_matrix"                   , rk_draw_matrix},
  {NULL, NULL},
};



void luaopen_rk(lua_State *L) {
  // register functions
  luaL_register(L, "rk", rk_functions);

}
