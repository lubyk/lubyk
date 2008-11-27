// loop buffer test
// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "values.h"
#include <sstream>
#include "value_test_helper.h"

class ValueTest : public ValueTestHelper
{  
public:
  
  void test_create_destroy( void )
  {
    TestValue v1(new TestData);
    assert_log("[1] new");
    
    TestValue v2;
    // nothing created
    assert_log("");
    // deletes last reference on [1]
    v1 = v2;
    assert_log("[1] dead");
  }
  
  void test_clone( void )
  {
    TestValue v1(new TestData);
    assert_log("[1] new");
    TestValue v2;
    assert_ref_count(v1, 1);
    
    // increase reference count
    v2 = v1;
    assert_log("");
    // v1 and v2 share the same object
    assert_id(v1, 1);
    assert_id(v2, 1);
    assert_ref_count(v1, 2);
    assert_ref_count(v2, 2);
    
    const TestData * d;
    // const: no copy
    d = v1.data();
    assert_log("");
    // copy on write
    d = v1.mutable_data();
    assert_log("[1] clone, [2] copy");
  }
  
  void test_type_conversion( void )
  {
    TestValue v1(new TestData);
    assert_log("[1] new");
    assert_ref_count(v1, 1);
    
    // anonymize & share data
    Value sig(v1);
    assert_log("");
    assert_id(sig, 1);
    assert_ref_count(v1, 2);
    // type conversion & share data
    TestValue v3(sig);
    assert_log(""); // no object created
    assert_id(v3, 1);
    assert_ref_count(v1, 3);
  }
  
  void test_set( void )
  {
    TestValue v1(new TestData);
    assert_log("[1] new");
    assert_ref_count(v1, 1);
    
    // anonymize & share data
    Value sig(v1);
    assert_log("");
    assert_id(sig, 1);
    assert_ref_count(v1, 2);
    // type conversion & share data
    TestValue v3;
    TS_ASSERT(sig.set(v3));
    assert_log(""); // no object created
    assert_id(v3, 1);
    assert_ref_count(v3, 3);
    // make sure we can alter the object
    v3.mutable_data();
    assert_ref_count(v3, 1);
    assert_log("[1] clone, [2] copy");
  }
  
  void test_accessor( void )
  {
    TestValue v1(new TestData);
    assert_log("[1] new");
    assert_ref_count(v1, 1);
    
    // share data
    TestValue v2(v1);
    assert_log("");
    assert_id(v2, 1);
    assert_ref_count(v1, 2);
    
    // const accessor on shared data
    TS_ASSERT_EQUALS(v1->id(), 1);
    TS_ASSERT_EQUALS((*v1).id(), 1);
    assert_log("");
    assert_id(v2, 1);
    assert_ref_count(v1, 2);
    
    // non-const accessor on shared
    TS_ASSERT_EQUALS(v1.mutable_data()->id(), 2);
    assert_log("[1] clone, [2] copy");
    assert_id(v1, 2);
    assert_id(v2, 1);
    assert_ref_count(v1, 1);
    assert_ref_count(v2, 1);
    
    // non-const accessor on non-shared
    TS_ASSERT_EQUALS(v1.mutable_data()->id(), 2);
    assert_log("");
    assert_id(v1, 2);
    assert_ref_count(v1, 1);
  }
};