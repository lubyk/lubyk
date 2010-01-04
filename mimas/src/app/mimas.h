/*
  ==============================================================================

   This file is part of the MIMAS project (http://rubyk.org/mimas)
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

// This file lets us set up any special config that we need for this app..
#include "juce_AppConfig.h"

// And this includes all the juce headers..
#include "../../lib/juce/juce_amalgamated.h"

#include "oscit/oscit.h"
using namespace oscit;

// ======================= GUI constants

// TODO: replace these constants by settings in the layout
#define LAYOUT_BROWSER_WIDTH 200
#define LAYOUT_BROWSER_BORDER_WIDTH 3
#define LAYOUT_BROWSER_PATHS_HEIGHT 400

#define BREADCRUMBS_BORDER_WIDTH 2

#define DEVICE_BORDER_WIDTH 2.0f
#define DEVICE_ROUNDED_RADIUS 8.0f