#include "class.h"
#include	<sys/types.h>
#include	<sys/stat.h> // stat for file modification time
#include  <fstream>    // file io

/** Abstract class to manage all the script load, reload. */
class Script : public Node
{
public:
  Script () : mScriptModTime(0), mReloadEvery(0), mNextReload(0), mScriptDead(true) {}
  
  bool init_script(const Params& p, const char * pLoadName = NULL, bool pUseDefault = true)
  {
    std::string str;
    
    if (( pLoadName && p.get(&str, pLoadName)) || p.get(&str, "load")) {
      mScriptFile = str;
      mReloadEvery = p.val("reload", 1);
      load_script_from_file(true);
    } else if (p.get(&str, "script", true)) {
      eval_script(str);
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
  
  virtual void eval_script(const std::string& pScript) = 0;
  
  void script(const Params& p)
  {  
    if (mScript == "" && mReloadEvery) reload_script();
    *mOutput << mScript << std::endl;
  }
  
protected:
  bool        mScriptDead;
  std::string mScript;       /**< The function definition. */
  std::string mScriptFile;
  time_t      mScriptModTime;
  int         mReloadEvery;
  time_t      mNextReload;
};