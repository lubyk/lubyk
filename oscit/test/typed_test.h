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
#include "mock/dummy_object.h"

class TypedTest : public TestHelper {
public:

  // void test_is_a( void ) {
  //   Object object;
  //   DummyObject dummy("doudou", 1.0);
  //   DummyObject2 dummy2("doudou", "hello");
  //   SubDummyObject sub("couscous", 2.0);
  //   assert_true(object.is_a("Object"));
  //   assert_false(object.is_a("Object.DummyObject"));
  //   assert_false(object.is_a("Object.DummyObject2"));
  //   assert_false(object.is_a("Object.DummyObject.SubDummyObject"));
  //
  //   assert_true(dummy.is_a("Object"));
  //   assert_true(dummy.is_a("Object.DummyObject"));
  //   assert_false(dummy.is_a("Object.DummyObject2"));
  //   assert_false(dummy.is_a("Object.DummyObject.SubDummyObject"));
  //
  //   assert_true(dummy2.is_a("Object"));
  //   assert_false(dummy2.is_a("Object.DummyObject"));
  //   assert_true(dummy2.is_a("Object.DummyObject2"));
  //   assert_false(dummy2.is_a("Object.DummyObject.SubDummyObject"));
  //
  //   assert_true(sub.is_a("Object"));
  //   assert_true(sub.is_a("Object.DummyObject"));
  //   assert_false(sub.is_a("Object.DummyObject2"));
  //   assert_true(sub.is_a("Object.DummyObject.SubDummyObject"));
  // }

  void test_kind_of( void ) {
    Object object;
    DummyObject dummy("doudou", 1.0);
    SubDummyObject sub("couscous", 2.0);
    assert_true(object.kind_of(Object));
    assert_false(object.kind_of(DummyObject));
    assert_false(object.kind_of(SubDummyObject));

    assert_true(dummy.kind_of(Object));
    assert_true(dummy.kind_of(DummyObject));
    assert_false(dummy.kind_of(SubDummyObject));

    assert_true(sub.kind_of(Object));
    assert_true(sub.kind_of(DummyObject));
    assert_true(sub.kind_of(SubDummyObject));
  }

  void test_type_cast( void ) {
    Object base;
    DummyObject dummy("doudou", 1200.0);
    Object   * res1;
    DummyObject  * res2;
    res1 = TYPE_CAST(Object, &base);
    assert_equal(&base, res1);
#ifndef USE_RTTI_TYPE
    // Using RTTI, this raises a warning and will always fail.
    res2 = TYPE_CAST(DummyObject, &base);
    assert_equal((DummyObject*)NULL, res2);
#endif
    res1 = TYPE_CAST(Object, &dummy);
    assert_equal((Object*)&dummy, res1);
    res2 = TYPE_CAST(DummyObject, &dummy);
    assert_equal(&dummy, res2);
  }

  void test_type_name( void ) {
    Object obj;
    RootProxy proxy(Location("oscit", "my place"));
    assert_equal("Object", obj.class_name());
    assert_equal("RootProxy", proxy.class_name());
  }
};