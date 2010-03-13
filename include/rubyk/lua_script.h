/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

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

#ifndef RUBYK_INCLUDE_RUBYK_LUA_SCRIPT_H_
#define RUBYK_INCLUDE_RUBYK_LUA_SCRIPT_H_

#include "oscit/script.h"
#include "oscit/mutex.h"
#include "rubyk/node.h"

struct lua_State;
typedef int (*lua_CFunction) (lua_State *L);

namespace rk {

class Outlet;

class LuaScript : public Node, public Script {
public:
  virtual const Value init() {
    return lua_init();
  }

  virtual ~LuaScript();

  /** Call a function in lua. If the Value argument is an array, each element
   * will be pushed on the stack before the method call thus passing [1,2,3] as
   * argument to method 'foo' produces the call <tt>foo(1,2,3)</tt>.
   * Thread-safe.
   */
  const Value call_lua(const char *function_name, const Value &val);

  const Value call_lua(const char *function_name) {
    return call_lua(function_name, gNilValue);
  }

  /** Template to give lua access to member methods.
   * Take care, these methods run within the mutex_ lock.
   */
  template <class T, int (T::*Tmethod)(const Value &)>
  static int cast_method_for_lua(lua_State *L) {
    T *node = (T*)lua_this(L);
    if (node) {
      Value res = stack_to_value(L);
      if (!res.is_empty()) {
        // check signature ?
        return (node->*Tmethod)(res);
      } else {
        return 0;
      }
    } else {
      return 0;
    }
  }

  /** Initialization (build methods, load libraries, etc).
   * Thread-safe.
   */
  const Value lua_init(const char *init_script = NULL);

  /** Convert all the stack as a list value (does not pop).
   */
  static const Value stack_to_value(lua_State *L, int start_index = 1);

  /** Get the value at the given index from the lua context.
   *  If index is 0, get all the stack as a list value.
   */
  static bool value_from_lua(lua_State *L, int index, Value *res);

protected:
  /** Open lua libraries. You can overwrite this method
   * to load your super special libraries.
   */
  virtual void open_lua_libs();

  /** Open a single library from a lua_CFunction and a
   * name.
   */
  void open_lua_lib(const char *name, lua_CFunction func);

  /** @internal.
   * Script compilation.
   */
  virtual const Value eval_script();

  /** @internal.
   * "inlet" method in lua to create/update an inlet.
   * @param val name of the inlet & type
   * @return number of values on lua stack (0)
   */
  int lua_inlet(const Value &val);

  /** @internal
   * "build_outlet_" method in lua to create/update an outlet (used by rubyk library).
   * @param val name of the outlet & type
   * @return number of values on stack: 1 = pointer to outlet
   */
  int lua_build_outlet(const Value &val);

  virtual void set_script_ok(bool state) {
    this->Script::set_script_ok(state);
    set_is_ok(state);
  }

  static bool lua_is_userdata(lua_State *L, int index, const char *tname);

  /** Generic 'call' from lua.
   * @param parameters passed from Lua
   * @return number of values on stack
   */
  int lua_call_rk(const Value &val);
private:
  /** Return 'this' object from lua.
   */
  static LuaScript *lua_this(lua_State *L);

  /** Get a list of values from the table at the given index.
   *  This method assumes the object at the given index is
   *  a table.
   */
  static bool list_from_lua(lua_State *L, int index, Value *res);

  /** Push a value on top of lua stack.
   */
  static bool lua_pushvalue(lua_State *L, const Value &val);

  /** Push a ListValue as a table on top of lua stack.
   */
  static bool lua_pushlist(lua_State *L, const Value &val);

  /** "send_" method in lua (used by rubyk library in Outlet 'class').
   */
  static int lua_send(lua_State *L);

  /** Retrieve outlet pointer from the element at index i in the lua stack.
   */
  static bool outlet_from_lua(lua_State *L, int index, Outlet **outlet);

  template <class T, int (T::*Tmethod)(const Value &)>
  void register_lua_method(const char *name) {
    register_lua_method(name, &cast_method_for_lua<T, Tmethod>);
  }

  void register_lua_method(const char *name, lua_CFunction function);

  void register_custom_types();

  /** Evaluate some lua content.
   */
  const Value eval(const char *script, size_t script_size);

  /** Printout the stack content. This is useful during debugging.
   */
  static void dump_stack(lua_State *L, const char *msg, int index);

  /** Every script has its own lua environment.
   */
  lua_State * lua_;

  /** Protect lua environment with a mutex.
   */
  Mutex mutex_;
};

} // rk

#endif // RUBYK_INCLUDE_RUBYK_LUA_SCRIPT_H_