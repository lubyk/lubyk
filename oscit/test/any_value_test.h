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
#include "oscit/values.h"

class AnyValueTest : public TestHelper
{
public:
  void test_is_any( void ) {
    Value v('*');

    assert_false(v.is_empty());
    assert_false(v.is_nil());
    assert_false(v.is_real());
    assert_false(v.is_string());
    assert_false(v.is_list());
    assert_false(v.is_error());
    assert_false(v.is_hash());
    assert_false(v.is_matrix());
    assert_false(v.is_midi());
    assert_true (v.is_any());

    assert_equal("*", v.type_tag());
  }

  void test_create_with_TypeTag( void ) {
    Value v(TypeTag("*"));

    assert_true(v.is_any());
  }

  void test_copy( void ) {
    Value v('*');
    Value v2(v);
    Value v3;

    assert_true(v2.is_any());

    v3 = v;

    assert_true(v3.is_any());
  }

  void test_set( void ) {
    Value v;

    v.set_any();
    assert_true(v.is_any());
  }

  void test_set_tag( void ) {
    Value v;

    v.set_type_tag("*");
    assert_true(v.is_any());
    assert_equal("*", v.type_tag());
  }

  void test_set_type( void ) {
    Value v;

    v.set_type(ANY_VALUE);
    assert_true(v.is_any());
  }

  void test_to_json( void ) {
    Value v('*');
    std::ostringstream os(std::ostringstream::out);
    os << v;
    assert_equal("null", os.str());
    assert_equal("null", v.to_json());
  }

  void test_any_input( void ) {
    Value any(AnyIO("La la."));
    assert_equal("*s", any.type_tag());
  }

  void test_json_value( void ) {
    JsonValue v("[[1,2],\"hello\"]");
    assert_true(v.is_list());
    assert_equal("[ff]s", v.type_tag());
    assert_equal("[[1, 2], \"hello\"]", v.to_json());
  }

  void test_equal( void ) {
    Value a('*');
    Value b('*');
    assert_equal(a, b);
    a.set("I'm a string");
    assert_false(a == b);
    a.set_nil();
    assert_false(a == b);
  }

  // from_json is not possible for AnyValue
};