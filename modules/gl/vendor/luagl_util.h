#ifndef __LUAGL_UTIL_H__
#define __LUAGL_UTIL_H__


typedef struct luaglConst  
{
  const char *str;
  unsigned int value;
} luaglConst;

void luagl_initconst(lua_State *L, const luaglConst *gl_const);
unsigned int luagl_get_enum(lua_State *L, int index, const luaglConst* gl_const);
unsigned int luagl_get_gl_enum(lua_State *L, int index);
int luagl_checkboolean (lua_State *L, int narg);
void* luagl_checkuserdata (lua_State *L, int narg);

#define LUAGL_ENUM_ERROR (unsigned int)-2

/* returns an parray with given type and size */
#define LUAGL_NEW_ARRAY(_type, _size) ( (_type *)malloc((_size) * sizeof(_type)) )

/* frees the space for the given parray, must be called together with LUAGL_NEW_ARRAY */
#define LUAGL_DELETE_ARRAY(_parray) { if(_parray) {free(_parray);} }

/* Returns an array in a lua table. */
void luagl_push_arrayuc(lua_State *L, unsigned char* parray, int size);
void luagl_push_arrayb(lua_State *L, unsigned char* parray, int size);  /* boolean */
void luagl_push_arrayui(lua_State *L, unsigned int* parray, int size);
void luagl_push_arrayi(lua_State *L, int* parray, int size);
void luagl_push_arrayf(lua_State *L, float* parray, int size);
void luagl_push_arrayd(lua_State *L, double* parray, int size);

/* Gets an parray from a lua table, store it in 'parray' and returns the no. of elems of the parray
   index refers to where the table is in stack. */
int luagl_get_arrayuc(lua_State *L, int index, unsigned char **parray);
int luagl_get_arrayb(lua_State *L, int index, unsigned char **parray);  /* boolean */
int luagl_get_arrayi(lua_State *L, int index, int **parray);
int luagl_get_arrayui(lua_State *L, int index, unsigned int **parray);
int luagl_get_arrayd(lua_State *L, int index, double **parray);
int luagl_get_arrayf(lua_State *L, int index, float **parray);

int luagl_get_array2uc(lua_State *L, int index, unsigned char **parray, int *size);
int luagl_get_array2d(lua_State *L, int index, double **parray, int *size);
int luagl_get_array2f(lua_State *L, int index, float **parray, int *size);

int luagl_str2mask(const char *str);
const char *luagl_mask2str(int mask);

#endif
