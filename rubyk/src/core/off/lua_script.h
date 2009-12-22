/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#ifndef RUBYK_SRC_CORE_LUA_SCRIPT_H_
#define RUBYK_SRC_CORE_LUA_SCRIPT_H_

extern "C" {
// we compiled as C code
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "script.h"

#define LUA_RETURN_BUFFER_SIZE 32
#define LUA_MATRIX_NAME "Matrix"

class LuaScript : public Script
{
public:
  LuaScript() : lua_(NULL) {}
  ~LuaScript();
  
protected:
  /** Call a lua function
   */
  const Value call_lua(const char *function_name, const Value &param);
  
  /** Script compilation.
   */
  virtual const Value eval_script();
  
  /** "send" method in lua.
   */
  static int send(lua_State *L);
  
  /** "send_note" method in lua.
   */
  static int send_note(lua_State *L);
  
  /** "send_ctrl" method in lua.
   */
  static int send_ctrl(lua_State *L);
  
  // FIXME, we could use self::get_char() syntax and get first arg as 'self' == this.
  template <class T, class S, int (S::*Tmethod)()>
  static int cast_super_method_for_lua(lua_State *L) {
    T * node = (T*)get_node_from_lua(L);
    if (node) {
      return (node->*Tmethod)();
    } else {
      return 0;
    }
  }
  
  
  /** Get a value on top lua stack/parameters.
   */
  bool value_from_lua(Value *val) {
    return value_from_lua(-1, val);
  }
  
  
  /** Get a value from lua stack/parameters.
   */
  bool value_from_lua(int index, Value *val);
  
  /** Get a Real from the current parameter list. */
  bool real_from_lua(Real *val);
  
  /** Get a string from the current parameter list. */
  bool string_from_lua(std::string *string);
  
  /** Get a list of values from a lua table. */
  bool list_from_lua_table(int index, Value *mat);
  
  /** Get a list of values from lua table at top index. */
  bool list_from_lua_table(Value *val) {
    return list_from_lua_table(lua_gettop(lua_), val);
  }
  
  /** Get a midi message from a lua table. */
  bool midi_message_from_lua_table(int index, Value *msg);
  
  /** Get a midi message at top index. */
  bool midi_message_from_lua_table(Value *msg) {
    return (midi_message_from_lua_table(lua_gettop(lua_), msg));
  }
  
  /** Push a single value on top of the lua stack.
   */
  bool lua_pushvalue(const Value &val);
  
  /** Push a list of values as a table on top of the lua stack.
   */
  void lua_pushlist(const Matrix& pMat);
  
  /** Push a midi message on top of the lua stack. */
  void lua_pushmidi(const MidiMessage& pMessage);
  
  /** Get a matrix from lua. */
  static bool matrix_from_lua(lua_State *L, Matrix ** pMat, int index);
  
  /** Set a global value in lua.
   */
  void set_lua_global(const char *key, const Value &val);
  
  /** Return true if the function name is defined in Lua.
   */
  bool lua_has_function(const char *function_name);
  
  /* open a lua library */
  void open_lua_lib(const char *name, lua_CFunction function);
  
  /* open base lua libraries */
  void open_base_lua_libs();
  
private:
  static const luaL_reg sMatrix_methods[];
  static const luaL_reg sMatrix_meta[];
  
  /** Get value at index. */
  static int Matrix_get(lua_State *L);
  
  /** Garbage collect. */
  static int Matrix_gc(lua_State *L);

  /** Printout. */
  static int Matrix_tostring(lua_State *L);
  
  /* register methods */
  void register_lua_methods();
  
  /* register matrix */
  void register_lua_Matrix();
  
  /* open all standard libraries for this lua script type */
  virtual void open_lua_libs();
  
  lua_State * lua_;
  Matrix      lua_Matrix;       /**< Used to return multiple values from lua. */
  MidiMessage lua_MidiMessage;  /**< Used to return midi messages from lua. */
};

#define METHOD_FOR_LUA(klass,method) {Class::find(#klass)->add_method_for_lua(#method, &LuaScript::cast_method_for_lua<klass, &klass::method>);}
#define SUPER_METHOD_FOR_LUA(klass,super,method) {Class::find(#klass)->add_method_for_lua(#method, &LuaScript::cast_method_for_lua<klass, super, &super::method>);}

#endif // RUBYK_SRC_CORE_LUA_SCRIPT_H_