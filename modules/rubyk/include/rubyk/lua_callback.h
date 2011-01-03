/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
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
#ifndef RUBYK_INCLUDE_RUBYK_LUA_CALLBACK_H_
#define RUBYK_INCLUDE_RUBYK_LUA_CALLBACK_H_

#include "rubyk.h"

namespace rubyk {
/** Calls a lua function back.
 */
class LuaCallback
{
public:
  LuaCallback(rubyk::Worker *worker) :
    worker_(worker),
    lua_(NULL),
    func_idx_(-1) {
  }

  virtual ~LuaCallback() {}

protected:

  /** Set a callback. The stack should be
   * 1. userdata from rk.Thread / rk.Socket / etc
   * 2. function()
   */
  void set_lua_callback(lua_State *L) {
    luaL_checktype(L, 2, LUA_TFUNCTION);

    // Store the function and the thread in the Thread/Socket's environment table so it is not GC too soon
    lua_getfenv(L, 1); // get environment for 'self'

    // env.callback = func
    lua_pushstring(L, "callback");
    lua_pushvalue(L, 2); // push func on top
    lua_settable(L, 3);

    // env.thread = thread
    lua_pushstring(L, "thread");
    lua_ = lua_newthread(L);
    lua_settable(L, 3);

    lua_pop(L, 1); // remove env table

    // get weak table
    lua_rawgeti(L, LUA_REGISTRYINDEX, worker_->lua_weak_idx_);

    // create reference
    // push func on top
    lua_pushvalue(L, 2);
    func_idx_ = luaL_ref(L, -2);
    lua_pop(L, 1); // remove weak table
  }

  /** The caller should lock before calling this.
   */
  void push_lua_callback() {
    if (func_idx_ == -1) throw Exception("Callback function not set.");

    // push weak table on top and get function
    lua_rawgeti(lua_, LUA_REGISTRYINDEX, worker_->lua_weak_idx_);
    lua_rawgeti(lua_, -1, func_idx_);
    lua_remove(lua_, -2); // remove weak index from stack
  }

  rubyk::Worker *worker_;
  lua_State *lua_;
private:
  int func_idx_;
};

} // rubyk

#endif // RUBYK_INCLUDE_RUBYK_LUA_CALLBACK_H_