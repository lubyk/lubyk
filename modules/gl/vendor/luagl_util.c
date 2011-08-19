#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>

#include "luagl_util.h"


/* returns a bi-dimensional parray with given type and size */
#define LUAGL_NEW_ARRAY2(type, size1, size2) ( (type *)malloc((size1) * (size2) * sizeof(type)) )

#define LUAGL_INIT_ARRAY(parray, _type, size, conversionFunc)   \
{                                                               \
  int i;                                                        \
  for(i = 0; i < (size); i++) {                                 \
    lua_rawgeti(L, index, i+1);                                 \
    (parray)[i] = (_type)(conversionFunc)(L, -1);               \
    lua_remove(L, -1);                                          \
  }                                                             \
}

#define LUAGL_INIT_ARRAY2(parray, _type, size1, size2, conversionFunc)  \
{                                                               \
  int i, j;                                                     \
  for(i = 0; i < size1; i++)  {                                 \
    lua_rawgeti(L, index, i+1);                                 \
    if(!lua_istable(L, -1)) return -1;                          \
    for(j = 0; j < *size2; j++) {                               \
      lua_rawgeti(L, -1, j + 1);                                \
      (parray)[i*(*size2) + j] = (_type)(conversionFunc)(L, -1);  \
      lua_remove(L, -1);                                        \
    }                                                           \
    lua_remove(L, -1);                                          \
  }                                                             \
}

/* Gets an parray from a lua table, store it in 'parray' and returns the no. of elems of the parray
index refers to where the table is in stack. */
#define DEFINE_GET_ARRAY_FUNC(name, _type, conversionFunc) \
  int name(lua_State *L, int index, _type **parray)        \
{                                                          \
  int n;                                                   \
  luaL_checktype(L, index, LUA_TTABLE);                    \
  n = luaL_getn(L, index);                                 \
  *parray = LUAGL_NEW_ARRAY(_type, n);                     \
  LUAGL_INIT_ARRAY(*parray, _type, n, conversionFunc);     \
  return n;                                                \
}

#define DEFINE_GET_ARRAY2_FUNC(name, _type, conversionFunc)    \
  int name(lua_State *L, int index, _type **parray, int *size) \
{                                                              \
  int n;                                                       \
  luaL_checktype(L, index, LUA_TTABLE);                        \
  n = luaL_getn(L, index);                                     \
  lua_rawgeti(L, index, 1);                                    \
  if(!lua_istable(L, -1)) { lua_remove(L, -1); return -1; }    \
  *size = luaL_getn(L, -1);                                    \
  *parray = LUAGL_NEW_ARRAY2(_type, n, *size);                 \
  LUAGL_INIT_ARRAY2(*parray, _type, n, size, conversionFunc);  \
  lua_remove(L, -1);                                           \
  return n;                                                    \
}

DEFINE_GET_ARRAY_FUNC(luagl_get_arrayb, unsigned char, lua_toboolean)
DEFINE_GET_ARRAY_FUNC(luagl_get_arrayd, double, lua_tonumber)
DEFINE_GET_ARRAY_FUNC(luagl_get_arrayf, float, lua_tonumber)
DEFINE_GET_ARRAY_FUNC(luagl_get_arrayi, int, lua_tointeger)
DEFINE_GET_ARRAY_FUNC(luagl_get_arrayui, unsigned int, lua_tointeger)
DEFINE_GET_ARRAY_FUNC(luagl_get_arrayuc, unsigned char, lua_tointeger)

DEFINE_GET_ARRAY2_FUNC(luagl_get_array2uc, unsigned char, lua_tointeger)
DEFINE_GET_ARRAY2_FUNC(luagl_get_array2d, double, lua_tonumber)
DEFINE_GET_ARRAY2_FUNC(luagl_get_array2f, float, lua_tonumber)

#undef DEFINE_GET_ARRAY_FUNC
#undef DEFINE_GET_ARRAY2_FUNC

#define DEFINE_PUSH_ARRAY_FUNC(name, _type, pushFunc) \
  void name(lua_State *L, _type *parray, int size)    \
{                                                     \
  int i;                                              \
  lua_createtable(L, size, 0);                        \
  for(i = 0; i < size; i++)                           \
  {                                                   \
    lua_pushinteger(L, i+1);                          \
    pushFunc(L, (_type)parray[i]);                    \
    lua_settable(L, -3);                              \
  }                                                   \
}

DEFINE_PUSH_ARRAY_FUNC(luagl_push_arrayb, unsigned char, lua_pushboolean)
DEFINE_PUSH_ARRAY_FUNC(luagl_push_arrayi, int, lua_pushinteger)
DEFINE_PUSH_ARRAY_FUNC(luagl_push_arrayui, unsigned int, lua_pushinteger)
DEFINE_PUSH_ARRAY_FUNC(luagl_push_arrayf, float, lua_pushnumber)
DEFINE_PUSH_ARRAY_FUNC(luagl_push_arrayd, double, lua_pushnumber)
DEFINE_PUSH_ARRAY_FUNC(luagl_push_arrayuc, unsigned char, lua_pushinteger)
                                          
#undef DEFINE_PUSH_ARRAY_FUNC

int luagl_str2mask(const char *str)
{
  int i, j;
  int mask = 0;
  int size = (int)strlen(str);
  for(i = 0, j = 0; j < size; i++)
  {
    if(str[i] == '1')
    {
      mask |= (1 << (size-1-j));
      j++;
    }
    else if(str[i] == '0')
      j++;

  }
  return mask;
}

const char *luagl_mask2str(int mask)
{
  unsigned int i;
  static char str[17];
  for(i = 0; i < 16; i++)
  {
    if(mask & (1 << (15 - i)))
      str[i] = '1';
    else
      str[i] = '0';
  }
  str[i] = 0;
  return str;
}

static unsigned int luagl_find_enum(const luaglConst* gl_const, const char *str, int n)
{
  int i = 0;

  while(gl_const[i].str != 0)
  {
    if(strncmp(str, gl_const[i].str, n) == 0 && gl_const[i].str[n] == 0)
      return gl_const[i].value;
    i++;
  }
  return LUAGL_ENUM_ERROR;
}

unsigned int luagl_get_enum(lua_State *L, int index, const luaglConst* gl_const)
{
  if (lua_isnumber(L, index))
  {
    return (unsigned int)luaL_checkinteger(L, index);
  }
  else
  {
    unsigned int i;
    const char *str = luaL_checkstring(L, index);
    unsigned int len = (unsigned int)strlen(str);
    unsigned int temp = 0, ret = 0, found = 0;

    for(i = 0; i < len; i++)
    {
      if(str[i] == ',')
      {
        temp = luagl_find_enum(gl_const, str, i);
        if (temp != LUAGL_ENUM_ERROR)
        {
          ret |= temp;
          found = 1;
        }

        str += i+1;
        len -= i+1;
        i = 0;
      }
    }
    temp = luagl_find_enum(gl_const, str, len);

    if (temp == LUAGL_ENUM_ERROR)
    {
      if (!found)
      {
        luaL_argerror(L, index, "unknown or invalid enumeration");
        return LUAGL_ENUM_ERROR;
      }

      return ret;
    }

    return ret | temp;
  }
}

void luagl_initconst(lua_State *L, const luaglConst *gl_const)
{
  for (; gl_const->str; gl_const++) 
  {
    if (gl_const->value == LUAGL_ENUM_ERROR)
      luaL_error(L, "WARNING: INVALID CONSTANT=LUAGL_ENUM_ERROR");

    lua_pushstring(L, gl_const->str);
    lua_pushinteger(L, gl_const->value);
    lua_settable(L, -3);
  }
}
