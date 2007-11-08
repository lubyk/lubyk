#include "class.h"
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

class Lua : public Node
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
    mScriptModTime = 0;
    mReloadEvery   = 0;
    mNextReload    = 0;
    mScriptDead    = true;
    mFunctionName = std::string("rubyk_").append(mName);
    
    /* Load Lua libraries */
    luaL_openlibs(mLua);
    
    std::string str;
    if (p.get(&str, "script") || (p.get(&str, "lua"))) {
      eval_script(str);
    }
    
    if (p.get(&str, "load")) {
      mScriptFile = str;
      mReloadEvery = p.val("reload", 1);
      load_script_from_file(true);
    }
    return true;
  }
  
  // load script from file.
  void load(const Params& p)
  {
    
    if (!p.get(&mScriptFile) && mScriptFile == "") {
      *mOutput << "Please give a filename.\n" << std::endl;
      return;
    }
    
    mReloadEvery = p.val("reload", mReloadEvery);
    if (!p.get(&mReloadEvery, "reload") && !mReloadEvery)
      mReloadEvery = 5;
    
    load_script_from_file(true);
  }
  
  void reload_script()
  {
    if ( !mReloadEvery || (mNextReload > mServer->mCurrentTime) ) {
      return;
    }
    
    mNextReload = mServer->mCurrentTime + (mReloadEvery * ONE_SECOND);
     
    load_script_from_file(false);
  }
  
  void load_script_from_file(bool isNewFile)
  {
    int status;
    struct stat info;
    
    if (stat(mScriptFile.c_str(), &info)) {
      *mOutput << "Could not stat '" << mScriptFile << "'." << std::endl;
      return;
    }
    
    if (!isNewFile && info.st_mtime == mScriptModTime) {
      // file did not change, skip
      return;
    }
    
    mScriptModTime = info.st_mtime;
    
    std::ifstream in(mScriptFile.c_str() ,std::ios::in);
      std::ostringstream oss;
      oss << in.rdbuf();
    in.close();
    eval_script(oss.str());
  }
  
  // set script
  void set(const Params& p)
  {
    std::string script;
    if (!p.get(&script)) {
      *mOutput << mName << ": No script found !\n";
      return;
    }
    eval_script(script);
  }
  
  void eval_script(const std::string& pScript) {
    int status;
    
    /* wrap the script into a function called 'mName_lua() */
    mScript = std::string("function ").append(mFunctionName).append("(sig)\n").append(pScript).append("\nend");
    
    /* compile script (as long as we maintain 1 context per object, we could release the mutex for long compilations since they are done inside the 'command' space) */
    status = luaL_loadbuffer(mLua, mScript.c_str(), mScript.size(), "script" ); // the last parameter is just used for debugging and error reporting
    if (status) {
      *mOutput << mName << ": Compilation error !\n";
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
    mScriptDead = false; // ok, we can receive process signals (again).
  }
  
  void script(const Params& p)
  {
    if (mScript == "" && mReloadEvery) reload_script();
    *mOutput << mScript << std::endl;
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
    sig.set(i);
  }
  
private:
  bool        mScriptDead;
  std::string mScript;       /**< The function definition. */
  std::string mFunctionName; /**< The name of the function in lua. */
  std::string mScriptFile;
  time_t      mScriptModTime;
  lua_State * mLua;
  int         mInput;
  int         mReloadEvery;
  time_t      mNextReload;
};

extern "C" void init()
{
  CLASS (Lua)
  INLET (Lua, input)
  OUTLET(Lua, output)
  METHOD(Lua, set)
  METHOD(Lua, load)
  METHOD(Lua, script)
}
