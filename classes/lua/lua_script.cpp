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