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
#include "opencv/cxtypes.h"

class MatrixValueTest : public TestHelper
{
public:
  void test_is_matrix( void ) {
    Value v(TypeTag("M"));

    assert_false(v.is_empty());
    assert_false(v.is_nil());
    assert_false(v.is_real());
    assert_false(v.is_string());
    assert_false(v.is_list());
    assert_false(v.is_error());
    assert_false(v.is_hash());
    assert_true (v.is_matrix());
    assert_false(v.is_any());

    Matrix * m = v.matrix_;
    assert_equal(0, m->rows());
    assert_equal(0, m->cols());
    assert_true(m->type() == 0);

    assert_equal("M", v.type_tag());

    int i = H("M");
    assert_equal(i, v.type_id());
  }

  void test_create_matrix_value( void ) {
    MatrixValue v(3,2);
    MatrixValue v2;

    assert_true(v.is_matrix());
    assert_true(v2.is_matrix());


    assert_equal(6, v.mat_size());
    assert_equal(0, v2.mat_size());
  }

  void test_create_with_char( void ) {
    Value v('M');

    assert_true(v.is_matrix());
    assert_equal(0, v.mat_size());
  }

  void test_create_with_TypeTag( void ) {
    Value v(TypeTag("M"));

    assert_true(v.is_matrix());
    assert_equal(0, v.mat_size());
  }

  void test_copy( void ) {
    MatrixValue v(2,3);
#if Real == double
    assert_true(v.mat_type() == CV_64FC1);
#else
    assert_true(v.mat_type() == CV_32FC1);
#endif
    Real * mat = v.mat_data();

    mat[0] = 0.0; mat[1] = 1.0; mat[2] = 2.0;
    mat[3] = 3.0; mat[4] = 4.0; mat[5] = 5.0;

    Value v2(v);
    Value v3;

    assert_true(v2.is_matrix());
    assert_true(v3.is_empty());

    assert_equal(3.0, v2.mat_data()[3]);
    assert_equal(6, v.mat_size());
    assert_equal(6, v2.mat_size());

    mat[3] = 3.5;

    // shared data
    assert_equal(3.5,  v.mat_data()[3]);
    assert_equal(3.5, v2.mat_data()[3]);

    v3 = v;

    assert_true(v3.is_matrix());
    assert_equal(6, v3.mat_size());
    assert_equal(3.5, v3.mat_data()[3]);

    mat[3] = 3.8;

    assert_equal(3.8,  v.mat_data()[3]);
    assert_equal(3.8, v2.mat_data()[3]);
    assert_equal(3.8, v3.mat_data()[3]);
  }

  void test_set( void ) {
    Value v;
    Matrix m(2,2);
    assert_true(v.is_empty());
    v.set(m);
    assert_true(v.is_matrix());
    assert_equal(4, v.mat_size());
  }

  void test_set_tag( void ) {
    Value v;

    v.set_type_tag("M");
    assert_true(v.is_matrix());
    assert_equal(0, v.mat_size());
  }

  void test_set_type( void ) {
    Value v;

    v.set_type(MATRIX_VALUE);
    assert_true(v.is_matrix());
    assert_equal(0, v.mat_size());
  }

  void test_to_json( void ) {
    MatrixValue v(2,3);
    std::ostringstream os(std::ostringstream::out);
    os << v;
    assert_equal("\"Matrix 2x3\"", os.str());
    assert_equal("\"Matrix 2x3\"", v.to_json());
  }

  void test_can_receive( void ) {
    Object object("foo", MatrixIO(1,5,"bar"));
    assert_false(object.can_receive(Value()));
    assert_true (object.can_receive(gNilValue));
    assert_false(object.can_receive(Value(1.23)));
    assert_false(object.can_receive(Value("foo")));
    assert_false(object.can_receive(Value(BAD_REQUEST_ERROR, "foo")));
    assert_false(object.can_receive(JsonValue("['','']")));
    assert_false(object.can_receive(HashValue()));
    assert_true (object.can_receive(MatrixValue(1,1)));
    assert_false(object.can_receive(MidiValue()));
  }

  void test_equal( void ) {
    Value a(MatrixValue(2,3));
    Value b(MatrixValue(2,3));
    assert_equal(a, b);
    a.set(MatrixValue(3,3));
    assert_false(a == b);
    a.set(4);
    assert_false(a == b);
    a.set_nil();
    assert_false(a == b);
  }
};