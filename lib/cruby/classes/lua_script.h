#ifndef _LUA_SCRIPT_
#define _LUA_SCRIPT_
#include "script.h"

extern "C" {
// we compiled as C code
#include "lua/src/lua.h"
}

#define LUA_RETURN_BUFFER_SIZE 32

class LuaScript : public Script
{
public:
  ~LuaScript();
  
  bool init_lua (const Params& p);
  
  void call_lua (const char * pFunctionName, Signal& sig, float f = 0);
  
  
  void eval_script (const std::string& pScript);
  
  // FIXME, we could use self::method() syntax and get first arg as 'self' == this.
  template <class T, int (T::*Tmethod)()>
  static int cast_method_for_lua(lua_State * L)
  {
    Signal sig;
    T * node = (T*)get_node_from_lua(L);
    if (node) {
      return (node->*Tmethod)();
    } else {
      fprintf(stderr, "Lua error: 'rubyk_this' not set.\n");
      return 0;
    }
  }
  
  // FIXME, we could use self::get_char() syntax and get first arg as 'self' == this.
  template <class T, class S, int (S::*Tmethod)()>
  static int cast_super_method_for_lua(lua_State * L)
  {
    T * node = (T*)get_node_from_lua(L);
    if (node) {
      return (node->*Tmethod)();
    } else {
      fprintf(stderr, "Lua error: 'rubyk_this' not set.\n");
      return 0;
    }
  }
  
  /** Define a signal from lua stack/parameters. */
  void sig_from_lua (Signal * sig, int index);
  
  /** Define a signla from lua stack/parameters, with a custom buffer. */
  void sig_from_lua (Signal * sig, int index, double * pBuffer, int pBufSize);
  
  static Node * get_node_from_lua(lua_State * L);
protected:  
  lua_State * mLua;
  double      mLuaReturn[LUA_RETURN_BUFFER_SIZE]; /**< Used to return multiple values from lua. */
};

#define METHOD_FOR_LUA(klass,method) {Class::find(#klass)->add_method_for_lua(#method, &LuaScript::cast_method_for_lua<klass, &klass::method>);}
#define SUPER_METHOD_FOR_LUA(klass,super,method) {Class::find(#klass)->add_method_for_lua(#method, &LuaScript::cast_method_for_lua<klass, super, &super::method>);}

#endif // _LUA_SCRIPT_