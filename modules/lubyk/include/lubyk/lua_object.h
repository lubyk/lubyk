/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher (http://teti.ch).

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
#ifndef LUBYK_INCLUDE_LUBYK_LUA_OBJECT_H_
#define LUBYK_INCLUDE_LUBYK_LUA_OBJECT_H_

#include "lubyk.h"

#define pushLuaCallback(s) pushLuaCallbackl(s, strlen(s))
namespace lubyk {
/** Calls a lua function back.
 */
class LuaObject
{
  int thread_in_env_idx_;
  const char *class_name_;
  /** This is used to invalidate the userdata in case the object is deleted
   * out of Lua.
   */
  void **userdata_;
public:
  /** Prepare tables to work with the table based self idion.
   * expects stack to be:
   * ... self
   * if self (last argument) is a table, it is used as self. 
   * Otherwise, a new table is created.
   * The method leaves "self" on top of the stack, with self.super = this.
   */
  LuaObject() throw();
  
  int luaInit(lua_State *L, void *ptr, const char *type_name) throw();

  virtual ~LuaObject();

  void luaDestroy();

  /** The caller should lock before calling this.
   * @todo: The 'const' stuff is stupid: can't we remove it ?
   * @fixme: OPTIMIZATION: Register the keys and pass a ref in the registry.
   * @return: true if the callback is set, false otherwise.
   */
  bool pushLuaCallbackl(const char *method, int len) const;

  lubyk::Worker *worker_;
  lua_State *lua_;

protected:
  /** When using a custom destructor, this method must be called
   * from within the custom destructor.
   */
  void luaCleanup();

private:
  void setupSuper(lua_State *L, void *ptr) throw();
  void setupMetatable(lua_State *L, const char *type_name) throw() ;
  void setupLuaThread(lua_State *L) throw();
};

} // lubyk

#endif // LUBYK_INCLUDE_LUBYK_LUA_OBJECT_H_
