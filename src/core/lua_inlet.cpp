/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

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

#include "lua_inlet.h"
#include "lua_script.h"

// TODO: This looks quite slow...

LuaInlet::LuaInlet(LuaScript *node, const char *name, const Value &type)
    : Inlet(node, name, &LuaInlet::receive_method, type),
      lua_script_(node) {}

// static
void LuaInlet::receive_method(Inlet *inlet, const Value &val) {
  Value res = ((LuaScript*)inlet->node())->call_lua(inlet->name().c_str(), val);
  if (res.is_error()) {
    // FIXME: we should create a .log (for observers) and an error
    // notification thing
    std::cerr << res << std::endl;
  }
}

const Value LuaInlet::trigger(const Value &val) {
  return lua_script_->call_lua(name().c_str(), val);
}
