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
    mFunctionName = std::string(mName).append("_lua");
    
    /* Load Lua libraries */
    luaL_openlibs(mLua);
    
    set(p);
    return true;
  }
  
  // load script from file.
  void load(const Params& p)
  {
    int status;
    struct stat info;
    
    if (!p.get(&mScriptFile) && mScriptFile == "") {
      *mOutput << "Please give a filename.\n" << std::endl;
      return;
    }
    
    if (stat(mScriptFile.c_str(), &info)) {
      *mOutput << "Could not stat '" << mScriptFile << "'." << std::endl;
      return;
    }
    
    if (info.st_mtime > mScriptModTime) {
      // file changed, reload content
      std::ifstream in(mScriptFile.c_str() ,std::ios::in);
        std::ostringstream oss;
        oss << in.rdbuf();
      in.close();
      update_script(oss.str());
    }
  }
  
  // set script
  void set(const Params& p)
  {
    std::string script;
    if (!p.get(&script)) {
      *mOutput << "No script found !\n";
      return;
    }
    update_script(script);
  }
  
  void update_script(const std::string& pScript) {
    int status;
    /* wrap the script into a function called 'mName_lua() */
    mScript = std::string("function ").append(mFunctionName).append("(sig)\n").append(pScript).append("\nend\n");
    
    /* compile script (as long as we maintain 1 context per object, we could release the mutex for long compilations since they are done inside the 'command' space) */
    status = luaL_loadbuffer(mLua, mScript.c_str(), mScript.size(), "script" ); // the last parameter is just used for debugging and error reporting
    if (status) {
      *mOutput << "Compilation error !\n";
      mIsOK = false; // stop receiving events
      return;
    } else {
      mIsOK = true;
    }
    
    /* Run the script to create the function. */
    status = lua_pcall(mLua, 0, 0, 0); // 0 arg, 1 result, no error function
    if (status) {
      *mOutput << "Function creation failed !\n";
      *mOutput << lua_tostring(mLua, -1) << std::endl;
    	
      mIsOK = false;
      return;
    }
  }
  
  void script(const Params& p)
  {
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
    
    lua_getfield(mLua, LUA_GLOBALSINDEX, mFunctionName.c_str()); /* function to be called */
    lua_pushinteger(mLua, mInput); /* first argument */
    
    /* Run the function. */
    status = lua_pcall(mLua, 1, 1, 0); // 0 arg, 1 result, no error function
    if (status) {
      *mOutput << "Script execution failed !\n";
      *mOutput << lua_tostring(mLua, -1) << std::endl;
    	
      mIsOK = false;
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
  std::string mScript;       /**< The function definition. */
  std::string mFunctionName; /**< The name of the function in lua. */
  std::string mScriptFile;
  time_t      mScriptModTime;
  lua_State * mLua;
  int         mInput;
};

extern "C" void init()
{
  Class * klass = Class::declare<Lua>("Lua");
  klass->add_inlet<Lua,&Lua::input>("input");
  klass->add_outlet<Lua,&Lua::output>("output");
  klass->add_method<Lua,&Lua::set>("set");
  klass->add_method<Lua,&Lua::load>("load");
  klass->add_method<Lua,&Lua::script>("script");
}
