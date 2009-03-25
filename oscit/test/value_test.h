#include "test_helper.h"
#include "oscit/values.h"

class ValueTest : public TestHelper
{  
public:
  void test_create( void )
  {
    Value v;
    
    assert_true (v.isNil());
    assert_false(v.isReal());
    assert_false(v.isString());
    assert_false(v.isError());
    
    assert_equal("N", v.type_tag());
  }
  
  void test_createChar( void )
  {
    Value v('N');
    
    assert_true(v.isNil());
  }
  
  void testCopy( void )
  {
    Value v;
    Value v2(v);
    Value v3(1.2);
    
    assert_true(v3.isReal());
    
    v3 = v;
    
    assert_true(v3.isNil());
    assert_true(v2.isNil());
  }
  
  void testSet( void )
  {
    Value v(1.2);
    
    assert_true(v.isReal());
    v.set();
    assert_true(v.isNil());
  }
  
  void testSetTag( void )
  {
    Value v(1.2);
    
    v.set_type_tag("N");
    assert_true(v.isNil());
  }
  
  void testSetType( void )
  {
    Value v(1.2);
    
    v.setType(NIL_VALUE);
    assert_true(v.isNil());
  }
};