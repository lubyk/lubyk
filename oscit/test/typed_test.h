#include "test_helper.h"
#include "mock/dummy_object.h"

class TypedTest : public TestHelper
{
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
};