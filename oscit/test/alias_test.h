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

#include <cxxtest/TestSuite.h>
#include "test_helper.h"
#include "mock/dummy_object.h"
#include "oscit/alias.h"

class AliasTest : public TestHelper {
 public:
  void test_trigger( void ) {
    Root root;
    Object   * one = root.adopt(new Object("one"));
    DummyObject  * sub = one->adopt(new DummyObject("sub", 3));
    Alias        * ali = root.adopt(new Alias("ali", sub));
    DummyObject  * two = root.adopt(new DummyObject("two", 2));
    
    Value res;
    
    assert_equal("/one" ,    one->url());
    assert_equal("/one/sub", sub->url());
    assert_equal("/ali",     ali->url());
    assert_equal("/two",     two->url());
    
    assert_equal(3.00, sub->real());
    res = root.call("/one/sub");
    assert_equal(3.00, res.r);
    res = root.call("/ali");
    assert_equal(3.00, res.r);
    
    res = root.call("/one/sub", Value(4.5));
    assert_equal(4.50, res.r);
    res = root.call("/one/sub");
    assert_equal(4.50, res.r);
    res = root.call("/ali");
    assert_equal(4.50, res.r);
    res = root.call("/two");
    assert_equal(2.00, res.r);
    
    res = root.call("/ali", Value(23.0));
    assert_equal(23.00, res.r);
    res = root.call("/one/sub");
    assert_equal(23.00, res.r);
    
    delete one; // delete original first
    res = root.call("/ali", Value(23.0));
    assert_equal("/ali", res.error_message());
    assert_equal(NOT_FOUND_ERROR, res.error_code());
  }
  
  void test_delete_alias_first( void )
  {
    Root root;
    Object       * one = root.adopt(new Object("one")); //   [/one]
    DummyObject  * sub = one->adopt(new DummyObject("sub", 3)); //   [/one/sub]
    Alias        * ali = root.adopt(new Alias("ali", sub));
    Value res;
    
    assert_equal("/one", one->url());
    assert_equal("/one/sub", sub->url());
    assert_equal("/ali", ali->url());
    
    assert_equal(3.00, sub->real());
    res = root.call("/one/sub");
    assert_equal(3.00, res.r);
    res = root.call("/ali");
    assert_equal(3.00, res.r);
    
    delete ali;
    res = root.call("/ali");
    assert_equal("/ali", res.error_message());
    assert_equal(NOT_FOUND_ERROR, res.error_code());
    res = root.call("/one/sub");
    assert_equal(3.00, res.r);
    
    delete one;
    res = root.call("/one/sub");
    assert_equal("/one/sub", res.error_message());
    assert_equal(NOT_FOUND_ERROR, res.error_code());
  }
  
};