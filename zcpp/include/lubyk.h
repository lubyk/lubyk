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

#include "lubyk/oscit.h"
#include "lubyk/lua.h"

namespace rk {

  /** Value used to measure time.
   * This is a double by default.
  */
  typedef double Time;
} // rk

// slot management
#include "lubyk/inlet.h"
#include "lubyk/outlet.h"
#include "lubyk/node.h"
#include "lubyk/planet.h"
#include "lubyk/event.h"

// object creation
#include "lubyk/class.h"
#include "lubyk/class_finder.h"
#include "lubyk/new_method.h"
#include "lubyk/text_command.h"
#include "lubyk/lua_script.h"

#include "lubyk/node.hpp"

// classes helpers
#include "lubyk/print.h"
#include "lubyk/opengl.h"
