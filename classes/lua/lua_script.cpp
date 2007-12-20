#include "lua_script.h"

extern "C" {
// we compiled as C code
// FIXME: include lua lib compilation in the makefile as C++ code.
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

LuaScript::~LuaScript()
{
  if (mLua) lua_close(mLua);
}


bool LuaScript::set_lua (const Params& p)
{
  return set_script(p);
}

void LuaScript::call_lua(const char * pFunctionName)
{
  int status;
  Signal sig;
  
  reload_script();
  
  if (!mScriptOK) return;
  
  lua_getglobal(mLua, pFunctionName); /* function to be called */
  
  /* Run the function. */
  status = lua_pcall(mLua, 0, 1, 0); // 1 arg, 1 result, no error function
  if (status) {
    *mOutput << mName << "(error): ";
    *mOutput << lua_tostring(mLua, -1) << std::endl;
  	
    mScriptOK = false;
    return;
  }
  sig_from_lua(&sig);
  if (sig.type) send(sig);
}


bool LuaScript::eval_script(const std::string& pScript) 
{
  mScript = pScript;
  
  return eval_lua_script(mScript);
}

bool LuaScript::eval_lua_script(const std::string& pScript)
{
  int status;
  if (!mLua) {
    /* Our own lua context. We might decide to share the context between lua objects some day. */
    mLua = lua_open();
    /* Load Lua libraries */
    luaL_openlibs(mLua);
    /* push 'this' into the global field 'rubyk_this' */
    lua_pushlightuserdata(mLua, (void*)this);
    lua_setglobal(mLua, "rubyk_this");
    
    /* register methods */
    register_lua_methods();
    
    /* register matrix */
    register_lua_Matrix();
  }
  
  /* compile script (as long as we maintain 1 context per object, we could release the mutex for long compilations since they are done inside the 'command' space) */
  status = luaL_loadbuffer(mLua, pScript.c_str(), pScript.size(), "script" ); // the last parameter is just used for debugging and error reporting
  if (status) {
    *mOutput << mName << ": Compilation error !\n";
    *mOutput << lua_tostring(mLua, -1) << std::endl;
    return false;
  }
  
  /* Run the script to create the functions. */
  status = lua_pcall(mLua, 0, 0, 0); // 0 arg, 1 result, no error function
  if (status) {
    *mOutput << mName << ": Function creation failed !\n";
    *mOutput << lua_tostring(mLua, -1) << std::endl;
  	return false;
  }
  return true; // ok, we can receive and process signals (again).
}

Node * LuaScript::get_node_from_lua(lua_State * L)
{
  lua_getglobal(L, "rubyk_this");
  Node * node = (Node*)lua_touserdata(L,lua_gettop(L));
  
  if (!node) printf("Lua error: 'rubyk_this' not set.\n");
  lua_pop(L,1);
  return node;
}

bool LuaScript::sig_from_lua(Signal * sig, int index)
{
  return sig_from_lua(sig, index, mLuaReturn);
}

bool LuaScript::double_from_lua(double * d)
{
  int index = lua_gettop(mLua);
  if (!lua_isnumber(mLua, index)) {
    *mOutput << mName << ": wrong value type to get double (" << lua_typename(mLua, index) << " at " << index << ").\n";
    return false;
  }
  *d = lua_tonumber(mLua,index);
  lua_pop(mLua,1);
  return true;
}

bool LuaScript::sig_from_lua(Signal * sig, int pIndex, Matrix& pMat)
{
  int index = pIndex < 0 ? lua_gettop(mLua) + pIndex + 1 : pIndex;
  int i  = 1;
  /* LUA_TNIL, LUA_TNUMBER, LUA_TBOOLEAN, LUA_TSTRING, LUA_TTABLE, LUA_TFUNCTION, LUA_TUSERDATA, LUA_TTHREAD, and LUA_TLIGHTUSERDATA.
  */
  switch ( lua_type(mLua, index) ) {
  case LUA_TNONE:
    sig->set_bang();
    break;
  case LUA_TNIL:
    sig->set_nil();
    lua_pop(mLua, 1);
    break;
  case LUA_TNUMBER:
    sig->set(lua_tonumber(mLua, index));
    lua_pop(mLua, 1);
    break;
  case LUA_TBOOLEAN:
    sig->set(lua_toboolean(mLua, index));
    lua_pop(mLua, 1);
    break;
  case LUA_TTABLE:
    pMat.set_sizes(1,0);
    while(true) {
      lua_pushinteger(mLua, i);
      lua_gettable(mLua, index);
      if(!lua_isnumber(mLua, -1)) {
        lua_pop(mLua,1);
        break;
      }
      double d = lua_tonumber(mLua, -1);
      pMat.append(d);
      lua_pop(mLua,1);
      i++;
    }    
    lua_pop(mLua,1);
    sig->set(pMat);
    break;
  default:
    *mOutput << mName << ": wrong value type to build signal (" << lua_typename(mLua, lua_type(mLua, index)) << " at " << index << ").\n";
    lua_pop(mLua, 1);
    return false;
  }
  return true;
}


bool LuaScript::matrix_from_lua (lua_State *L, Matrix ** pMat, int pIndex)
{
  Matrix ** mat_ptr, * mat;
  mat_ptr = (Matrix **)luaL_checkudata(L, pIndex, LUA_MATRIX_NAME);
  if (mat_ptr == NULL) {
    std::cout << "lua: wrong value type to get matrix (" << lua_typename(L, lua_type(L, pIndex)) << " at " << pIndex << ").\n";
    return false;
  } else {
    mat = *mat_ptr;
    if (!mat) {
      std::cout << "lua: null matrix from lua.\n";
      return false;
    }
    *pMat = mat;
    return true;
  }
}

void LuaScript::lua_pushmatrix (const Matrix& pMat)
{
  Matrix ** mat_ptr = (Matrix **)lua_newuserdata(mLua, sizeof(Matrix *));
  *mat_ptr = const_cast<Matrix*>(&pMat); // we are carefull not to modify the matrix (matrix from)
  luaL_getmetatable(mLua, LUA_MATRIX_NAME);
  lua_setmetatable(mLua, -2);
}

int LuaScript::Matrix_get (lua_State *L)
{
  Matrix * mat;
  if (!matrix_from_lua(L, &mat, 1)) return 0;
  int index = luaL_checkint(L, 2) - 1;
  if (index < 0 || index >= (int)mat->size()) return 0;
  lua_pushnumber(L, mat->data[index]);
  return 1;
}

int LuaScript::Matrix_gc (lua_State *L)
{
  // nothing to do for gc (const ref).
  return 0;
}

int LuaScript::Matrix_tostring (lua_State *L)
{ 
  Matrix * mat;
  if (!matrix_from_lua(L, &mat, 1)) return 0;
  
  std::ostringstream oss(std::ostringstream::out);
  oss << *mat;
  lua_pushfstring(L, "%s", oss.str().c_str());
  return 1;
}


const luaL_reg LuaScript::sMatrix_methods[] = {
  {"get",        Matrix_get},
  {"tostring",   Matrix_tostring},
  {0,0}
};

const luaL_reg LuaScript::sMatrix_meta[] = {
  {"__gc",       Matrix_gc},
  {"__tostring", Matrix_tostring},
  {0, 0}
};

void LuaScript::register_lua_methods()
{
  /* register methods for lua */
  std::vector<std::string>::const_iterator it,end;
  const Hash<std::string, method_for_lua_t> * method_for_lua = mClass->methodsForLua();
  end   = method_for_lua->end();
  
  /* register send method */
  lua_pushcfunction(mLua, &send_for_lua);
  lua_setglobal(mLua, "send");
  
  for(it = method_for_lua->begin(); it < end; it++) {
    method_for_lua_t method;
    if (method_for_lua->get(&method, *it)) {
      lua_pushcfunction(mLua, method);
      lua_setglobal(mLua, (*it).c_str());
    }
  }
}

void LuaScript::register_lua_Matrix()
{
  luaL_newmetatable(mLua, LUA_MATRIX_NAME);        /* create metatable for Matrix, add it to the Lua registry */
  luaL_openlib(mLua, LUA_MATRIX_NAME, sMatrix_methods, 0);  /* create methods table, add it to the globals */
  
  lua_pushstring(mLua, "__index");
  lua_pushstring(mLua, "get");
  lua_gettable(mLua, 2);  /* get Matrix.get */
  lua_settable(mLua, 1);  /* metatable.__index = Matrix.get */
  
  
  lua_pushstring(mLua, "__tostring");
  lua_pushstring(mLua, "tostring");
  lua_gettable(mLua, 2); /* get Matrix.tostring */
  lua_settable(mLua, 1); /* metatable.__tostring = Matrix.tostring */
  
  
  // uncomment when we have const / not const checking code.
  // lua_pushstring(L, "__newindex");
  // lua_pushstring(L, "set");
  // lua_gettable(L, 2); /* get array.set */
  // lua_settable(L, 1); /* metatable.__newindex = array.set */
  
  //  luaL_openlib(mLua, 0, sMatrix_meta, 0);  /* fill metatable */
  //  lua_pushliteral(mLua, "__index");
  //  lua_pushvalue(mLua, -3);               /* dup methods table*/
  //  lua_rawset(mLua, -3);                  /* metatable.__index = methods */
  //  lua_pushliteral(mLua, "__metatable");
  //  lua_pushvalue(mLua, -3);               /* dup methods table*/
  //  lua_rawset(mLua, -3);                  /* hide metatable:
  //                                         metatable.__metatable = methods */
  //  lua_pop(mLua, 1);                      /* drop metatable */
}