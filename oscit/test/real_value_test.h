#include "test_helper.h"
#include "oscit/values.h"

class RealValueTest : public TestHelper
{  
public:
  void testCreate( void )
  {
    Value v(3.5);
    
    assertFalse(v.isNil());
    assertTrue (v.isReal());
    assertFalse(v.isString());
    assertFalse(v.isError());
    
    
    assertEqual(3.5, v.r); // real
    assertEqual(3.5, v.f); // alias as float
    assertEqual(3.5, v.d); // alias as double
    
    assertEqual("f", v.type_tag());
  }
  
  void testCreateChar( void )
  {
    Value v('f');
    
    assertTrue(v.isReal());
    assertEqual(0.0, v.r);
  }
  
  void testCopy( void )
  {
    Value v(3.2);
    Value v2(v);
    Value v3;
    
    assertTrue(v2.isReal());
    assertTrue(v3.isNil());
    
    v3 = v;
    
    assertTrue(v3.isReal());
    
    assertEqual(3.2, v.r);
    assertEqual(3.2, v2.r);
    assertEqual(3.2, v3.r);
    v.r = 4.5;
    assertEqual(4.5, v.r);
    assertEqual(3.2, v2.r);
    assertEqual(3.2, v3.r);
  }
  
  void testSet( void )
  {
    Value v;
    
    assertTrue(v.isNil());
    v.set(3.5);
    assertTrue(v.isReal());
    assertEqual(3.5, v.r);
  }
  
  void testSetTag( void )
  {
    Value v;
    
    v.set_type_tag("f");
    assertTrue(v.isReal());
    assertEqual(0.0, v.r);
  }
  
  void testSetType( void )
  {
    Value v;
    
    v.setType(REAL_VALUE);
    assertTrue(v.isReal());
    assertEqual(0.0, v.r);
  }
};