#include "test_helper.h"
#include "oscit/values.h"

class ValueTest : public TestHelper
{  
public:
  void testCreate( void )
  {
    Value v;
    
    assertTrue (v.isNil());
    assertFalse(v.isReal());
    assertFalse(v.isString());
    assertFalse(v.isError());
    
    assertEqual("N", v.type_tag());
  }
  
  void testCreateChar( void )
  {
    Value v('N');
    
    assertTrue(v.isNil());
  }
  
  void testCopy( void )
  {
    Value v;
    Value v2(v);
    Value v3(1.2);
    
    assertTrue(v3.isReal());
    
    v3 = v;
    
    assertTrue(v3.isNil());
    assertTrue(v2.isNil());
  }
  
  void testSet( void )
  {
    Value v(1.2);
    
    assertTrue(v.isReal());
    v.set();
    assertTrue(v.isNil());
  }
  
  void testSetTag( void )
  {
    Value v(1.2);
    
    v.set_type_tag("N");
    assertTrue(v.isNil());
  }
  
  void testSetType( void )
  {
    Value v(1.2);
    
    v.setType(NIL_VALUE);
    assertTrue(v.isNil());
  }
};