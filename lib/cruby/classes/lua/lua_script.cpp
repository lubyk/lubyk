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


bool LuaScript::init_lua (const Params& p)
{
  mLua = NULL;
  return init_script(p,"lua", false);
}


void LuaScript::call_lua(const char * pFunctionName, Signal& sig, float f)
{
  int status,i;
  
  reload_script();
  
  if (mScriptDead) return;
  
  lua_getfield(mLua, LUA_GLOBALSINDEX, pFunctionName); /* function to be called */
  lua_pushnumber(mLua, f); /* first argument */
  
  /* Run the function. */
  status = lua_pcall(mLua, 1, 1, 0); // 1 arg, 1 result, no error function
  if (status) {
    *mOutput << "Script execution failed !\n";
    *mOutput << lua_tostring(mLua, -1) << std::endl;
  	
    mScriptDead = true;
    return;
  }
  sig_from_lua(&sig, lua_gettop(mLua));
}


void LuaScript::eval_script(const std::string& pScript) 
{
  int status;
  
  mScript = pScript;
  
  if (!mLua) {
    /* Our own lua context. We might decide to share the context between lua objects some day. */
    mLua = lua_open();
    /* Load Lua libraries */
    luaL_openlibs(mLua);
    /* push 'this' into the global field 'rubyk_this' */
    lua_pushlightuserdata(mLua, (void*)this);
    lua_setglobal(mLua, "rubyk_this");
    
    /* register methods for lua */
    std::vector<std::string>::const_iterator it,end;
    const Hash<std::string, method_for_lua_t> * method_for_lua = mClass->methodsForLua();
    end   = method_for_lua->end();
    
    for(it = method_for_lua->begin(); it < end; it++) {
      method_for_lua_t method;
      if (method_for_lua->get(&method, *it)) {
        lua_pushcfunction(mLua, method);
        lua_setglobal(mLua, (*it).c_str());
      }
    }
  }
  
  /* compile script (as long as we maintain 1 context per object, we could release the mutex for long compilations since they are done inside the 'command' space) */
  status = luaL_loadbuffer(mLua, mScript.c_str(), mScript.size(), "script" ); // the last parameter is just used for debugging and error reporting
  if (status) {
    *mOutput << mName << ": Compilation error !\n";
    *mOutput << lua_tostring(mLua, -1) << std::endl;
    mScriptDead = true; // stop processing events until next reload
    return;
  }
  
  /* Run the script to create the functions. */
  status = lua_pcall(mLua, 0, 0, 0); // 0 arg, 1 result, no error function
  if (status) {
    *mOutput << mName << ": Function creation failed !\n";
    *mOutput << lua_tostring(mLua, -1) << std::endl;
  	
    mScriptDead = true;
    return;
  }
  mScriptDead = false; // ok, we can receive and process signals (again).
}


Node * LuaScript::get_node_from_lua(lua_State * L)
{
  lua_getglobal(L, "rubyk_this");
  Node * node = (Node*)lua_touserdata(L,lua_gettop(L));
  lua_pop(L,1);
  return node;
}

void LuaScript::sig_from_lua(Signal * sig, int index)
{
  sig_from_lua(sig, index, mLuaReturn, LUA_RETURN_BUFFER_SIZE);
}

void LuaScript::sig_from_lua(Signal * sig, int index, double * pBuffer, int pBufSize)
{
  int i  = 1;
  int sz = 0;
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
    if (!pBuffer) {
      fprintf(stderr, "Cannot return lua table without buffer to write values !\n");
      sig->set_nil();
      return;
    }
    while(true) {
      lua_pushinteger(mLua, i);
      lua_gettable(mLua, index);
      if(!lua_isnumber(mLua, -1))
        break;
      if (i > pBufSize) {
        fprintf(stderr, "Buffer too small (%i) to write all values returned by lua table !\n", pBufSize);
        break;
      }
      double f = lua_tonumber(mLua, -1);
      pBuffer[i-1] = f;
      sz++;
      lua_pop(mLua,1);
      i++;
    }    
    lua_pop(mLua,1);
    sig->type = DoubleArraySignal;
    sig->doubles.value = pBuffer;
    sig->doubles.size  = sz;
    break;
  default:
    fprintf(stderr, "Unsupported lua return value '%s'.\n", lua_typename(mLua, index));
    sig->set_nil();
    lua_pop(mLua, 1);
  }
}