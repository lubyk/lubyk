/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
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

#include "rubyk/class.h"
#include "oscit/alias.h"

namespace rk {

/** Build all inlets for an object from prototypes. */
void Class::make_outlets(Node *object)
{
  std::list<OutletPrototype>::iterator it;
  std::list<OutletPrototype>::iterator begin = outlet_prototypes_.begin();
  std::list<OutletPrototype>::iterator end   = outlet_prototypes_.end();
  Object * outlets = object->adopt(new Object("out", DEFAULT_TYPE, true));
  //FIX: Object * method;
  Outlet * outlet;

  for (it = begin; it != end; it++) {
    outlet = outlets->adopt(new Outlet(object, *it));
    if (it == begin) {
      // alias for first "link" in out/link
      //FIX: method = (Object*)outlet->child("link");
      //FIX: if (method) outlets->adopt(new Alias("link", method)); // out/link ---> out/counter/link
      //FIX: method = (Object*)outlet->child("unlink");
      //FIX: if (method) outlets->adopt(new Alias("unlink", method)); // out/unlink ---> out/counter/unlink
    }
  }
}

} // rk
