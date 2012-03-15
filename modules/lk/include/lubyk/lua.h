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

#ifndef LUBYK_INCLUDE_LUBYK_H_
#define LUBYK_INCLUDE_LUBYK_H_

/** Pseudo return value for complex types or to push
 * variable args on stack.
 */
typedef int LuaStackSize;

/** Include Lua
 */
extern "C" {
  #include "lua/lua.h"
  #include "lua/lauxlib.h"

  /** Output debugging information on the current lua stack.
   * Must link with static lubyk_core if used.
   */
  void luaDump(lua_State *L, const char *msg, bool inspect_tables = true);
}

// We often check for 'false' return value, this helps
#define lua_isfalse(L,i) (lua_isboolean(L,i) && !lua_toboolean(L,i))
#define lua_istrue(L,i)  (lua_isboolean(L,i) && lua_toboolean(L,i))

#endif // LUBYK_INCLUDE_LUBYK_H_