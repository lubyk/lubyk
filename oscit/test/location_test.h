/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
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

#include "test_helper.h"
#include "oscit/location.h"

class LocationTest : public TestHelper
{
public:
  void test_create( void ) {
    Location loc1("oscit", "my place");
    Location loc2("oscit", 167772167, 45);
  }

  void test_to_stream( void ) {
    Location loc1("oscit", "my place");
    Location loc2("oscit", 167772167, 45);
    std::ostringstream out(std::ostringstream::out);
    out << loc1;
    assert_equal("oscit://\"my place\"", out.str());
    out.str("");
    out << loc2;
    assert_equal("oscit://10.0.0.7:45", out.str());
  }

  void test_hashId( void ) {
    Location loc1("oscit", "some place");
    Location loc2("oscit", "other place");
    assert_equal(1838689659, hashId(loc1));
  }

  void test_equal_operator( void ) {
    Location loc1("oscit", "some place");
    Location loc2("oscit", "other place");
    Location loc3("oscit", "some place");
    assert_false(loc1 == loc2);
    assert_true( loc1 == loc3);
  }

};
