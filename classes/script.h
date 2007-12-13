#include "class.h"
#include	<sys/types.h>
#include	<sys/stat.h> // stat for file modification time
#include  <fstream>    // file io

/** Abstract class to manage all the script load, reload. */
class Script : public Node
{
public:
  Script () : mScriptModTime(0), mReloadEvery(0), mNextReload(0), mScriptDead(true) {}
  
  bool set_script(const Params& p)
  {
    std::string str;
    
    if (p.get(&str, "load")) {
      mScriptFile  = str;
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
    oss << "\n";
    eval_script(oss.str());
    *mOutput << mName << ": script loaded.\n";
  }
  
  virtual void eval_script(const std::string& pScript) = 0;
  
  void script(const Params& p)
  {  
    if (mScript == "" && mReloadEvery) reload_script();
    *mOutput << mScript << std::endl;
  }
  
protected:
  std::string mScript;         /**< Script text. */
  std::string mScriptFile;     /**< Script file path. */
  time_t      mScriptModTime;  /**< Script file's modification time on last load. */
  time_t      mReloadEvery;    /**< How often should we check for a modification in the source file (in seconds). */
  time_t      mNextReload;     /**< Compute time for the next check to reload the script. */
  bool        mScriptDead;     /**< Script compilation failed. Might change on next reload. */
  
};