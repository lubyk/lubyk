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
    assert_ref_count(1, v1);
    
    // increase reference count
    v2 = v1;
    assert_log("");
    // v1 and v2 share the same object
    assert_id(1, v1);
    assert_id(1, v2);
    assert_ref_count(2, v1);
    assert_ref_count(2, v2);
    
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
    assert_ref_count(1, v1);
    
    // anonymize & share data
    Value sig(v1);
    assert_log("");
    assert_id(1, sig);
    assert_ref_count(2, v1);
    // type conversion & share data
    TestValue v3(sig);
    assert_log(""); // no object created
    assert_id(1, v3);
    assert_ref_count(3, v1);
  }
  
  void test_set( void )
  {
    TestValue v1(new TestData);
    assert_log("[1] new");
    assert_ref_count(1, v1);
    
    // anonymize & share data
    Value sig(v1);
    assert_log("");
    assert_id(1, sig);
    assert_ref_count(2, v1);
    // type conversion & share data
    TestValue v3;
    TS_ASSERT(sig.set(&v3));
    assert_log(""); // no object created
    assert_id(1, v3);
    assert_ref_count(3, v3);
    // make sure we can alter the object
    v3.mutable_data();
    assert_ref_count(1, v3);
    assert_log("[1] clone, [2] copy");
  }
};