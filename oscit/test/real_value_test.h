#include "test_helper.h"
#include "oscit/values.h"

class RealValueTest : public TestHelper
{  
public:
  void testCreate( void )
  {
    Value r1(3.5);
    Value r2(3);
    Value r3(3.0);
    Value empty;
    
    assertEqual(3.5, r1.r); // real
    assertEqual(3.5, r1.f); // alias as float
    assertEqual(3.5, r1.d); // alias as double
    assertEqual(0.0, empty.f);
  }
  
  void testCopy( void )
  {
    Value r1(3.2);
    Value r2(r1);
    Value r3;
    r3 = r1;
    assertEqual(3.2, r1.r);
    assertEqual(3.2, r2.r);
    assertEqual(3.2, r3.r);
    r1.r = 4.5;
    assertEqual(4.5, r1.r);
    assertEqual(3.2, r2.r);
    assertEqual(3.2, r3.r);
  }
};