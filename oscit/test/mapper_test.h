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
#include "oscit/mapper.h"

class MapperTest : public TestHelper
{
public:
  void test_simple_parse( void ) {
    Mapper mapper;
    assert_true(mapper.parse(std::string("/slider1/1 [0,1] --> /m/tempo [60,240]")));
    std::string target;
    Real value;
    assert_true(mapper.map("/slider1/1", 0.5, &target, &value));
    assert_equal("/m/tempo", target);
    assert_equal(150.0, value);
  }
  
  void test_parse_empty_lines( void ) {
    Mapper mapper;
    assert_true(mapper.parse(std::string("\n\n/slider1/1 [0,1] --> /m/tempo [60,240]\n\n/foo/bar [1,2] -> /baz [10, 20]")));
    std::string target;
    Real value;
    assert_true(mapper.map("/foo/bar", 0.5, &target, &value));
    assert_equal("/baz", target);
    assert_equal(10.0, value);
  }
  
  void test_parse_return_in_definition( void ) {
    Mapper mapper;
    assert_true(mapper.parse(std::string("\n\n/slider1/1 [0,1]\n--> /m/tempo [60,240]\n\n/foo/bar [1,2] -> /baz [10, 20]")));
    std::string target;
    Real value;
    assert_true(mapper.map("/slider1/1", 1.5, &target, &value));
    assert_equal("/m/tempo", target);
    assert_equal(240.0, value);
  }
  
  void test_parse_comment( void ) {
    Mapper mapper;
    assert_true(mapper.parse(std::string("\n\n/slider1/1 [0,1] --> /m/tempo [60,240]\n# comment\n/foo/bar [1,2] -> /baz [10, 20]")));
    std::string target;
    Real value;
    assert_true(mapper.map("/slider1/1", 1.5, &target, &value));
    assert_equal("/m/tempo", target);
    assert_equal(240.0, value);
  }
  
  void test_parse_comment_in_definition( void ) {
    Mapper mapper(10);
    assert_true(mapper.parse(std::string("\n\n/slider1/1 [0,1] #foo bar\n -->#baz\n /m/tempo #blah\n[60,240]\n# comment\n/foo/bar [1,2] -> /baz [10, 20]")));
    std::string target;
    Real value;
    assert_true(mapper.map("/slider1/1", 1.5, &target, &value));
    assert_equal("/m/tempo", target);
    assert_equal(240.0, value);
  }
};