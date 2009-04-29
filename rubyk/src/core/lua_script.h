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
   *  @return number of values on lua stack
   */
  int lua_inlet(const Value &val);
  
  template <class T, int (T::*Tmethod)(const Value &)>
  static int cast_method_for_lua(lua_State *L) {
    T *node = (T*)lua_this(L);
    if (node) {
      Value res = node->stack_to_value();
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
  const Value stack_to_value();
  
  /** Get the value at the given index from the lua context.
   *  If index is 0, get all the stack as a list value.
   */
  bool value_from_lua(int index, Value *res);
  
  /** Get a list of values from the table at the given index.
   *  This method assumes the object at the given index is
   *  a table.
   */
  bool list_from_lua(int index, Value *res);
  
  /** Push a value on top of lua stack.
   */
  bool lua_pushvalue(const Value &val);

  /** Push a ListValue as a table on top of lua stack.
   */
  bool lua_pushlist(const Value &val);
  
  template <class T, int (T::*Tmethod)(const Value &)>
  void register_lua_method(const char *name) {
    register_lua_method(name, &cast_method_for_lua<T, Tmethod>);
  }
  
  void register_lua_method(const char *name, lua_CFunction function);
  
  /** Printout the stack content. This is useful during debugging.
   */
  void dump_stack(const char *msg, int index);
  
  /** Every script has its own lua environment.
   */
  lua_State * lua_;
};

#endif // RUBYK_SRC_CORE_LUA_SCRIPT_H_