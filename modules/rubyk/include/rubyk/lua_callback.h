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

namespace rubyk {
/** Calls a lua function back.
 */
class LuaCallback
{
public:
  LuaCallback(rubyk::Worker *worker, int lua_func_idx) :
    worker_(worker), func_idx_(lua_func_idx) {}

  virtual ~LuaCallback() {
    // release function
    luaL_unref(worker_->lua_, LUA_REGISTRYINDEX, func_idx_);
  }

protected:
  /** The caller should lock before calling this.
   */
  void push_lua_callback() {
    // push LUA_REGISTRYINDEX on top
    lua_rawgeti(worker_->lua_, LUA_REGISTRYINDEX, func_idx_);
  }

  rubyk::Worker *worker_;
private:
  int func_idx_;
};

} // rubyk

#endif // RUBYK_INCLUDE_RUBYK_LUA_CALLBACK_H_