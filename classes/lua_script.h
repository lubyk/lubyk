#ifndef _LUA_SCRIPT_
#define _LUA_SCRIPT_
#include "script.h"

extern "C" {
// we compiled as C code
#include "lua/src/lua.h"
#include "lua/src/lauxlib.h"  // luaL_ref, luaL_unref
}

#define LUA_RETURN_BUFFER_SIZE 32
#define LUA_MATRIX_NAME "Matrix"

class LuaScript : public Script
{
public:
  LuaScript() : mLua(NULL) {}
  ~LuaScript();
  
  bool set_lua (const Params& p);
  
  void call_lua (const char * pFunctionName);
  
  bool eval_script (const std::string& pScript);
  
  bool eval_lua_script(const std::string& pScript);
  
  static int send_for_lua(lua_State * L)
  {
    LuaScript * node = (LuaScript *)get_node_from_lua(L);
    if (node) {
      // value, port
      Signal sig;
      double p;
      if (!node->sig_from_lua(&sig)) {
        node->error("could not get signal");
        return 0;
      }
      if (!node->double_from_lua(&p)) {
        node->error("could not get port from lua in 'send'");
        return 0;
      }
      node->send((int)p, sig);
    } else {
      printf("send_for_lua error: no node\n");
    }
    return 0;
  }
  
  // FIXME, we could use self::method() syntax and get first arg as 'self' == this.
  template <class T, int (T::*Tmethod)()>
  static int cast_method_for_lua(lua_State * L)
  {
    T * node = (T*)get_node_from_lua(L);
    if (node) {
      return (node->*Tmethod)();
    } else {
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
      return 0;
    }
  }
  
  /** Define a signal from lua stack/parameters. Default get from top. */
  bool sig_from_lua (Signal * sig, int index = -1);
  
  /** Get a double from the current parameter list. */
  bool double_from_lua(double *);
  
  /** Define a signla from lua stack/parameters, with a custom buffer. */
  bool sig_from_lua (Signal * sig, int index, Matrix& pMat);
  
  static Node * get_node_from_lua (lua_State * L);
  
  /** Push a matrix on top of the lua stack. */
  void lua_pushmatrix (const Matrix& pMat);
  
  /** Get a matrix pointer from lua. */
  static bool matrix_from_lua (lua_State *L, Matrix ** pMat, int pIndex);

private:
  static const luaL_reg sMatrix_methods[];
  static const luaL_reg sMatrix_meta[];
  
  /** Get value at index. */
  static int Matrix_get (lua_State *L);
  
  /** Garbage collect. */
  static int Matrix_gc (lua_State *L);

  /** Printout. */
  static int Matrix_tostring (lua_State *L);
  
  /* register methods */
  void register_lua_methods ();
  
  /* register matrix */
  void register_lua_Matrix ();
  
protected:  
  
  lua_State * mLua;
  Matrix mLuaReturn; /**< Used to return multiple values from lua. */
};

#define METHOD_FOR_LUA(klass,method) {Class::find(#klass)->add_method_for_lua(#method, &LuaScript::cast_method_for_lua<klass, &klass::method>);}
#define SUPER_METHOD_FOR_LUA(klass,super,method) {Class::find(#klass)->add_method_for_lua(#method, &LuaScript::cast_method_for_lua<klass, super, &super::method>);}

#endif // _LUA_SCRIPT_