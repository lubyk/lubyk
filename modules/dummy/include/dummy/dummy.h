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
#ifndef LUBYK_INCLUDE_DUMMY_DUMMY_H_
#define LUBYK_INCLUDE_DUMMY_DUMMY_H_

#include "lubyk.h"
using namespace lubyk;

namespace dummy {

/** Dummy (a simple class to test bindings).
 *
 * @dub lib_name: 'Dummy_core'
 *      filename: 'Dummy_core/Dummy'
 */
class Dummy : public LuaObject
{
public:
  Dummy() {}

  /** Used to test if the callback is properly set.
   * In a normal class, the callback method is called from a C context and should
   * protect the worker_ with ScopedLock lock(worker_);
   */
  float callback(const char *func, float value) {
    // lua_ = LuaCallback's thread
    lua_State *L = lua_;

    pushLuaCallback(func, strlen(func));
    lua_pushnumber(L, value);

    // first argument is self
    int status = lua_pcall(L, 2, 1, 0);

    if (status) {
      throw dub::Exception("Error in callback function: %s\n", lua_tostring(L, -1));
    }
    return lua_tonumber(L, -1);
  }

  /** This method becomes a lua binding to C and will be
   * overloaded in Lua.
   */
  float addSomething(float value) {
    return value + 123.0;
  }

  static const char *plat();
};

} // dummy

#endif // LUBYK_INCLUDE_DUMMY_DUMMY_H_

