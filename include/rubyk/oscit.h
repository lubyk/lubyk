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

#ifndef RUBYK_INCLUDE_RUBYK_OSCIT_H_
#define RUBYK_INCLUDE_RUBYK_OSCIT_H_

#include "oscit/oscit.h"
using namespace oscit;

// TODO move this in a types.h file ?
#define CLASS_URL   "/class"
#define PATCH_VIEW_URL "/views/patch"

#define CLASS_URL_LENGTH 6

/** This is where all the dynamic nodes are created.
 */
#define NODES_KEY     "nodes"
#define NODE_OUT_KEY  "out"

/** This is ?
 */
#define LIB_URL     "/class/lib"
#define INSPECT_URL "/.inspect"

#define RUBYK_URL   "/rubyk"
#define LINK_URL    "/rubyk/link"
#define QUIT_URL    "/rubyk/quit"

#endif // RUBYK_INCLUDE_RUBYK_OSCIT_H_