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

#include "test_helper.h"


class ErrorTest : public ValueTestHelper
{
public:
  void test_create_cast( void )
  {
    Error n("Something bad happened.");
    assert_id(n,1);
    TS_ASSERT(n.message());
    TS_ASSERT_EQUALS( std::string(n.message()), "Something bad happened.");
    // anonymize
    Value val(n);
    
    // cannot get as string
    String n2;
    TS_ASSERT(!val.set(n2));
    
    // can get as error
    Error e;
    TS_ASSERT(val.set(e));
    TS_ASSERT_EQUALS( std::string(e.message()), "Something bad happened.");
  }
  
  void test_is_a( void )
  {
    Error e;
    Error e2("Bad");
    Value v;
    TS_ASSERT(!e.is_error());
    TS_ASSERT(e2.is_error());
    TS_ASSERT(!v.is_error());
    e2.set(v);
    TS_ASSERT(v.is_error());
  }
  
  void test_operator_equal( void )
  {
    Error n("Something bad happened.");
    TS_ASSERT( n == "Something bad happened.");
    TS_ASSERT( n == std::string("Something bad happened."));
  }
  
  void test_stream( void )
  {
    std::ostringstream out(std::ostringstream::out);
    Error v;
    out << v;
    TS_ASSERT_EQUALS(out.str(), "Nil");
    v = "Bad things happen.";
    out.str(std::string(""));
    out << v;
    TS_ASSERT_EQUALS(out.str(), "[1] #Bad things happen.");
  }
};
