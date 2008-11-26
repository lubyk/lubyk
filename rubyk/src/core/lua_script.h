#ifndef _LUA_SCRIPT_
#define _LUA_SCRIPT_
#include "script.h"

extern "C" {
// we compiled as C code
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#define LUA_RETURN_BUFFER_SIZE 32
#define LUA_MATRIX_NAME "Matrix"

class LuaScript : public Script
{
public:
  LuaScript() : mLua(NULL) {}
  ~LuaScript();
  
  bool set_lua (const Params& p);
  
  /* call lua function without parameters */
  void call_lua (const char * pFunctionName)
  { call_lua(NULL, pFunctionName, NULL); }
  
  /* call lua function with one parameter */
  void call_lua (const char * pFunctionName, const Signal& sig)
  { call_lua(NULL, pFunctionName, &sig); }
  
  /* call lua function without parameters, one value returned */
  void call_lua (Signal * retSig, const char * pFunctionName)
  { call_lua(retSig, pFunctionName, NULL); }
  
  /* call lua function with one parameter, one value returned */
  void call_lua (Signal * retSig, const char * pFunctionName, const Signal& sig)
  { call_lua(retSig, pFunctionName, &sig); }
  
  /* call lua function with one parameter, one value returned: prototype */
  void call_lua (Signal * retSig, const char * pFunctionName, const Signal * sig);
  
  bool eval_script (const std::string& pScript);
  
  bool eval_lua_script(const std::string& pScript);
  
  static int send_for_lua(lua_State * L);
  
  static int send_note_for_lua(lua_State * L);
  
  static int send_ctrl_for_lua(lua_State * L);
  
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
  bool signal_from_lua (Signal * sig, int index = -1);
  
  /** Define a signal from lua stack/parameters, with a custom buffer. */
  bool signal_from_lua (Signal * sig, int index, Matrix& pMat, MidiMessage& pMsg);
  
  /** Get a real_t from the current parameter list. */
  bool real_t_from_lua (real_t *);
  
  /** Get a string from the current parameter list. */
  bool string_from_lua (std::string *);
  
  /** Get a matrix from a lua table. */
  bool matrix_from_lua_table (Matrix * pMat, int pIndex);
  
  /** Get a matrix from lua table at top index. */
  bool matrix_from_lua_table (Matrix * pMat);
  
  /** Get a midi message from a lua table. */
  bool midi_message_from_lua_table (MidiMessage * pMsg, int pIndex);
  
  /** Get a midi message at top index. */
  bool midi_message_from_lua_table (MidiMessage * pMsg);
  
  static Node * get_node_from_lua (lua_State * L);
  
  /** Push a matrix on top of the lua stack. */
  bool lua_pushsignal (const Signal& sig);
  
  /** Push a matrix on top of the lua stack. */
  void lua_pushmatrix (const Matrix& pMat);
  
  /** Push a midi message on top of the lua stack. */
  void lua_pushmidi (const MidiMessage& pMessage);
  
  /** Get a matrix from lua. */
  static bool matrix_from_lua (lua_State *L, Matrix ** pMat, int pIndex);
  
  void set_lua_global (const char * key, const Signal& sig);
  
  /** Return true if the function named 'key' is defined in Lua. */
  bool lua_has_function (const char * key);
  
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
  
  /* open all standard libraries for this lua script type */
  virtual void open_lua_libs ();

protected:
  
  /* open a lua library */
  void open_lua_lib (const char* pName, lua_CFunction pFunc);
  
  /* open base lua libraries */
  void open_base_lua_libs ();
  
  lua_State * mLua;
  Matrix      mLuaMatrix;       /**< Used to return multiple values from lua. */
  MidiMessage mLuaMidiMessage;  /**< Used to return midi messages from lua. */
};

#define METHOD_FOR_LUA(klass,method) {Class::find(#klass)->add_method_for_lua(#method, &LuaScript::cast_method_for_lua<klass, &klass::method>);}
#define SUPER_METHOD_FOR_LUA(klass,super,method) {Class::find(#klass)->add_method_for_lua(#method, &LuaScript::cast_method_for_lua<klass, super, &super::method>);}

#endif // _LUA_SCRIPT_