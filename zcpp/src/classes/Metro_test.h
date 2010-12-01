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

#include "test_helper.h"


class MetroTest : public ParseHelper
{
public:

  void test_bang_bang( void ) {
    setup_with_print("n=Metro(1000)\n"); // 0 [bang] lost (before link to print)
    assert_run("p: Bang!\np: Bang!\n", (60 * 2) + 10);  // ... 60 [bang] ... 120 [bang] ... 130.
  }

  void test_set_tempo( void ) {
    setup("n=Metro()\n");
    assert_result("# 10\n", "n(10)\n");
    assert_result("# {\"tempo\":20}\n", "n(tempo:20)\n");
    assert_result("# 30\n", "n/tempo(30)\n");
    assert_result("# <Metro:/n tempo:30 run:1>\n", "n\n");
  }
};