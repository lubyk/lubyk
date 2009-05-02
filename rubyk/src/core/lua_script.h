#ifndef RUBYK_SRC_CORE_LUA_SCRIPT_H_
#define RUBYK_SRC_CORE_LUA_SCRIPT_H_

#include "script.h"

struct lua_State;
typedef int (*lua_CFunction) (lua_State *L);

class LuaScript : public Script
{
public:
  virtual const Value init() {
    return lua_init();
  }
  
  virtual ~LuaScript();
  
  /** Call a function in lua.
   */
  const Value call_lua(const char *function_name, const Value &val);
protected:
  /** Initialization (build methods, load libraries, etc).
   */
  const Value lua_init();
  
  /** Script compilation.
   */
  virtual const Value eval_script();
  
  /** "inlet" method in lua to create/update an inlet.
   *  @param val name of the inlet & type
   *  @return number of values on lua stack (0)
   */
  int lua_inlet(const Value &val);
  
  /** "build_outlet_" method in lua to create/update an outlet (used by rubyk library).
   *  @param val name of the outlet & type
   *  @return number of values on stack: 1 = pointer to outlet
   */
  int lua_build_outlet(const Value &val);
  
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
  
private:
  static LuaScript *lua_this(lua_State *L);
  
  /** Pop all the stack as a list value.
   */
  static const Value stack_to_value(lua_State *L, int start_index = 1);
  
  /** Get the value at the given index from the lua context.
   *  If index is 0, get all the stack as a list value.
   */
  static bool value_from_lua(lua_State *L, int index, Value *res);
  
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
  
  /** Printout the stack content. This is useful during debugging.
   */
  static void dump_stack(lua_State *L, const char *msg, int index);
  
  /** Every script has its own lua environment.
   */
  lua_State * lua_;
};

#endif // RUBYK_SRC_CORE_LUA_SCRIPT_H_