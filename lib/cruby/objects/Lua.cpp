#include "script.h"

extern "C" {
// we compiled as C code
// FIXME: include lua lib compilation in the makefile as C++ code.
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include	<sys/types.h>
#include	<sys/stat.h> // stat for file modification time
#include  <fstream>    // file io

class Lua : public Script
{
public:
  ~Lua()
  {
    lua_close(mLua);
  }
  
  bool init (const Params& p)
  {
    /* Our own lua context. We might decide to share the context between lua objects some day. */
    mLua = lua_open();
    
    mInput         = 0;
    mFunctionName = std::string("rubyk_").append(mName);
    
    /* Load Lua libraries */
    luaL_openlibs(mLua);
    
    return init_script(p);
  }
  
  // inlet 1
  void input(const Signal& sig)
  { 
    
    /* For now we only deal with integers. We will need to find a way to
     * pass other types and let the script decide. */
    sig.get(&mInput);
  }
  
  // outlet 1
  void output(Signal& sig)
  {
    int status,i;
    
    reload_script();
    
    if (mScriptDead) return;
    
    lua_getfield(mLua, LUA_GLOBALSINDEX, mFunctionName.c_str()); /* function to be called */
    lua_pushinteger(mLua, mInput); /* first argument */
    
    /* Run the function. */
    status = lua_pcall(mLua, 1, 1, 0); // 0 arg, 1 result, no error function
    if (status) {
      *mOutput << "Script execution failed !\n";
      *mOutput << lua_tostring(mLua, -1) << std::endl;
    	
      mScriptDead = true;
      return;
    }
    
    /* Get returned value at stack top. */
    i = lua_tointeger(mLua, lua_gettop(mLua));
    
    /* Remove value from stack. */
    //lua_pop(mLua, 1);
    /* Send the value out. */
    if (i == -1)
      sig.set_nil();
    else
      sig.set(i);
  }
  
  
  void eval_script(const std::string& pScript) 
  {
    int status;
    /* wrap the script into a function called 'mName_lua() */
    mScript = std::string("function ").append(mFunctionName).append("(sig)\n").append(pScript).append("\nend");
    
    /* compile script (as long as we maintain 1 context per object, we could release the mutex for long compilations since they are done inside the 'command' space) */
    status = luaL_loadbuffer(mLua, mScript.c_str(), mScript.size(), "script" ); // the last parameter is just used for debugging and error reporting
    if (status) {
      *mOutput << mName << ": Compilation error !\n";
      *mOutput << lua_tostring(mLua, -1) << std::endl;
      mScriptDead = true; // stop processing events until next reload
      return;
    }
    
    /* Run the script to create the function. */
    status = lua_pcall(mLua, 0, 0, 0); // 0 arg, 1 result, no error function
    if (status) {
      *mOutput << mName << ": Function creation failed !\n";
      *mOutput << lua_tostring(mLua, -1) << std::endl;
    	
      mScriptDead = true;
      return;
    }
    mScriptDead = false; // ok, we can receive and process signals (again).
  }
  
private:  
  lua_State * mLua;
  std::string mFunctionName; /**< The name of the function in lua. */
  int         mInput;
};

extern "C" void init()
{
  CLASS (Lua)
  INLET (Lua, input)
  OUTLET(Lua, output)
  SUPER_METHOD(Lua, Script, set)
  SUPER_METHOD(Lua, Script, load)
  SUPER_METHOD(Lua, Script, script)
}
