#include "draw.h"

#include "lua_cpp_helper.h"

using namespace glu;



/** void glu::Build2DMipmaps(const cv::Mat &mat)
 * rubyk/support/draw.h:120
 */
static int glu_Build2DMipmaps(lua_State *L) {
  try {
    const cv::Mat *mat = *((const cv::Mat **)luaL_checkudata(L, 1, "cv.Mat"));
    Build2DMipmaps(*mat);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("glu.Build2DMipmaps: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "glu.Build2DMipmaps: Unknown exception");
    lua_error(L);
    return 0;
  }
}



// Register namespace


static const struct luaL_Reg glu_functions[] = {
  {"Build2DMipmaps"                , glu_Build2DMipmaps},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_glu(lua_State *L) {
#else
extern "C" int luaopen_glu(lua_State *L) {
#endif
  // register functions
  luaL_register(L, "glu", glu_functions);

  return 0;
}
