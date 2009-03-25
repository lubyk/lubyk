#include "test_helper.h"
#include "oscit/values.h"

class RealValueTest : public TestHelper
{  
public:
  void test_create( void )
  {
    Value v(3.5);
    
    assert_false(v.isNil());
    assert_true (v.isReal());
    assert_false(v.isString());
    assert_false(v.isError());
    
    
    assert_equal(3.5, v.r); // real
    assert_equal(3.5, v.f); // alias as float
    assert_equal(3.5, v.d); // alias as double
    
    assert_equal("f", v.type_tag());
  }
  
  void test_createChar( void )
  {
    Value v('f');
    
    assert_true(v.isReal());
    assert_equal(0.0, v.r);
  }
  
  void testCopy( void )
  {
    Value v(3.2);
    Value v2(v);
    Value v3;
    
    assert_true(v2.isReal());
    assert_true(v3.isNil());
    
    v3 = v;
    
    assert_true(v3.isReal());
    
    assert_equal(3.2, v.r);
    assert_equal(3.2, v2.r);
    assert_equal(3.2, v3.r);
    v.r = 4.5;
    assert_equal(4.5, v.r);
    assert_equal(3.2, v2.r);
    assert_equal(3.2, v3.r);
  }
  
  void testSet( void )
  {
    Value v;
    
    assert_true(v.isNil());
    v.set(3.5);
    assert_true(v.isReal());
    assert_equal(3.5, v.r);
  }
  
  void testSetTag( void )
  {
    Value v;
    
    v.set_type_tag("f");
    assert_true(v.isReal());
    assert_equal(0.0, v.r);
  }
  
  void testSetType( void )
  {
    Value v;
    
    v.setType(REAL_VALUE);
    assert_true(v.isReal());
    assert_equal(0.0, v.r);
  }
};