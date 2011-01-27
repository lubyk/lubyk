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

#include "lubyk.h"
using namespace rk;

class Lua : public LuaScript {
};

extern "C" void init(Planet &planet) {
  CLASS(Lua, "Lua script.", "script: [script content] or file: [path to file]")
  // We use SUPER_LAST_METHOD instead of SUPER_METHOD so that the methods inserted here are kept at the end of the object's method list.

  // {1}
  SUPER_LAST_METHOD(Lua, Script, file, Oscit::string_io("Path to script content [filepath]."))
  // {2}
  SUPER_LAST_METHOD(Lua, Script, script, Oscit::string_io("Script content [lua]."))
  // {3}
  SUPER_LAST_METHOD(Lua, Script, reload, Oscit::real_io("How often to stat file for reload [s]."))
}