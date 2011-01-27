/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
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

#ifndef LUBYK_INCLUDE_LUBYK_LUA_INLET_H_
#define LUBYK_INCLUDE_LUBYK_LUA_INLET_H_

#include "lubyk/inlet.h"

namespace rk {

class LuaScript;

class LuaInlet : public Inlet {
public:
  LuaInlet(LuaScript *node, const char *name, const Value &attrs);

  /** Method called whenever we receive a new value.
   */
  virtual const Value trigger(const Value &val);

  /** Dummy method (should never be called since we overwrite trigger).
   */
  static const Value dummy(void *receiver, const Value &val) {
    return gNilValue;
  }

private:
  LuaScript *lua_script_;
};

} // rk

#endif // LUBYK_INCLUDE_LUBYK_LUA_INLET_H_