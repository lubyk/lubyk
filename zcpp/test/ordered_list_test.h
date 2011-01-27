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

#include "test_helper.h"

struct OrderedListTest_Object
{
  OrderedListTest_Object(Real value) : value_(value) {}

  bool operator>=(const OrderedListTest_Object &object) const {
    return value_ >= object.value_;
  }

  Real value_;
};

class OrderedListTest : public TestHelper
{
public:
  void test_push_pop( void ) {
    OrderedList<int> list;
    int i;

    assert_equal( list.size(), 0);

    list.push(2);
    assert_equal( list.size(), 1);
    assert_equal( list.front(), 2);
    assert_true( list.get(&i) );
    assert_equal( i, 2);
    list.pop();
    assert_true( list.empty() );
    assert_true( !list.get(&i) );

    // adding keeps element sorted
    list.push(3);
    list.push(2);
    assert_equal( list.front(), 2);
    list.push(1);
    assert_equal( list.front(), 1);
    list.push(1);

    assert_equal( list.front(), 1);

    // pop to make sure we have a single copy, ordered
    list.pop();
    assert_equal( list.front(), 2);
    list.pop();
    assert_equal( list.front(), 3);
    list.pop();
    assert_true( list.empty());
  }

  void test_reorder( void ) {
    OrderedList<OrderedListTest_Object*> list;
    OrderedListTest_Object a(1), b(2), c(3);
    OrderedListTest_Object * res;

    // elements are sorted
    list.push(&c);
    list.push(&a);
    list.push(&b);
    assert_equal(3, list.size());
    assert_equal(&a,list.front());
    assert_true(list.get(&res));
    assert_equal(&a, res);

    // reordering
    a.value_ = 2.5;
    // not reordered yet
    assert_equal(&a, list.front());

    list.sort();
    assert_equal(&b, list.front());
    list.pop();
    assert_equal(&a, list.front());
    list.pop();
    assert_equal(&c, list.front());
  }

  void test_reorder_two( void ) {
    OrderedList<OrderedListTest_Object*> list;
    OrderedListTest_Object a(10), b(20), c(30);

    // elements are sorted (10, 20, 30)
    list.push(&b);
    list.push(&a);
    list.push(&c);
    assert_equal(3, list.size());
    assert_equal(&a, list.front());

    a.value_ = 100;
    b.value_ = 200;
    // before reordering (100, 200, 30)
    list.sort();

    // 30, 100, 200
    assert_equal(&c, list.front());
    list.pop();
    assert_equal(&a, list.front());
  }
};