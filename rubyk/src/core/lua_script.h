#ifndef RUBYK_SRC_CORE_LUA_SCRIPT_H_
#define RUBYK_SRC_CORE_LUA_SCRIPT_H_

#include "script.h"

struct lua_State;

class LuaScript : public Script
{
public:
  LuaScript();
  
  virtual ~LuaScript();
  
protected:

  /** Script compilation.
   */
  virtual const Value eval_script();
  
private:
  /** Every script has its own lua environment.
   */
  lua_State * lua_;
};

#endif // RUBYK_SRC_CORE_LUA_SCRIPT_H_