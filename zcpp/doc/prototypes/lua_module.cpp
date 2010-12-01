/** compile and test with

g++ -O2 -fno-common -c -o test.o -I../../include/rubyk/lua lua_module.cpp
gcc -bundle -undefined dynamic_lookup -o test.so test.o -lstdc++
lua
> require 'test'
> print(test.hello(5))

*/

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

/** void rk::makeTexture(const cv::Mat &mat, int tex)
 * rubyk/support/draw.h:54
 */
static int test_hello(lua_State *L) {
  int x = luaL_checkint(L, 1);
  lua_pushnumber(L, x+7);
  return 1;
}



// Register namespace


static const struct luaL_Reg test_functions[] = {
  {"hello", test_hello},
  {NULL, NULL},
};


extern "C" int luaopen_test(lua_State *L) {
  // register functions
  luaL_register(L, "test", test_functions);
  return 0;
}
