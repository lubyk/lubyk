#include "draw.h"

#include "lua_cpp_helper.h"

using namespace gl;



/** void gl::DrawPixels(const cv::Mat &mat)
 * lubyk/support/draw.h:131
 */
static int gl_DrawPixels(lua_State *L) {
  try {
    const cv::Mat *mat = *((const cv::Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    DrawPixels(*mat);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("gl.DrawPixels: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "gl.DrawPixels: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** void gl::TexImage(const cv::Mat &mat)
 * lubyk/support/draw.h:136
 */
static int gl_TexImage(lua_State *L) {
  try {
    const cv::Mat *mat = *((const cv::Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    TexImage(*mat);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("gl.TexImage: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "gl.TexImage: Unknown exception");
    lua_error(L);
    return 0;
  }
}



// Register namespace


static const struct luaL_Reg gl_functions[] = {
  {"DrawPixels"                    , gl_DrawPixels},
  {"TexImage"                      , gl_TexImage},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_gl(lua_State *L) {
#else
extern "C" int luaopen_gl(lua_State *L) {
#endif
  // register functions
  luaL_register(L, "gl", gl_functions);

  return 0;
}
