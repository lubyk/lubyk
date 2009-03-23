#include "test_helper.h"
#include "oscit/values.h"

class ListValueTest : public TestHelper
{  
public:
  void testAlloc( void )
  {
    Value v(Value::alloc(2));
    Value * l = v.list;
    
    assertEqual(0.0, l[0].f);
    assertEqual(0.0, l[1].f);
    l[0].f = 3.5;
    assertEqual(3.5, l[0].f);
    assertEqual(0.0, l[1].f);
    v.dealloc();
  }
  
  void testShare( void )
  {
    Value v(Value::alloc(2));
    Value v2;
    Value * l = v.list;
    
    l[0].f = 1.0;
    l[1].f = 2.0;
    
    assertEqual(1.0, v.list[0].f);
    assertEqual(2.0, v.list[1].f);
    
    v2 = v;
    
    assertEqual(1.0, v2.list[0].f);
    assertEqual(2.0, v2.list[1].f);
    
    v.list[0].f = 3.0;
    assertEqual(3.0,  v.list[0].f);
    assertEqual(3.0, v2.list[0].f);
    
    v.dealloc();
  }
};