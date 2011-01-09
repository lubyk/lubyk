#include "lua_cpp_helper.h"

int luaload_cv(lua_State *L);
int luaload_cv_Mat(lua_State *L);
int luaload_cv_Size(lua_State *L);
int luaload_cv_Point(lua_State *L);
int luaload_cv_Scalar(lua_State *L);
int luaload_cv_Scalar(lua_State *L);
//int luaload_cv_additions(lua_State *L);

extern "C" int luaopen_cv_core(lua_State *L) {

  luaload_cv(L);
  luaload_cv_Mat(L);
  luaload_cv_Size(L);
  luaload_cv_Point(L);
  luaload_cv_Scalar(L);
  //luaload_cv_additions(L);

  return 0;
}