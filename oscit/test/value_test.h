#include "test_helper.h"
#include "oscit/values.h"

class RealValueTest : public TestHelper
{  
public:
  void test_create( void )
  {
    Value r1(3.5);
    Value r2(3);
    Value r3(3.0);
    Value empty;
    
    assert_equal(3.5, r1.r); // real
    assert_equal(3.5, r1.f); // alias as float
    assert_equal(3.5, r1.d); // alias as double
    assert_equal(0.0, empty.f);
  }
  
  void test_copy( void )
  {
    Value r1(3.2);
    Value r2(r1);
    Value r3;
    r3 = r1;
    assert_equal(3.2, r1.r);
    assert_equal(3.2, r2.r);
    assert_equal(3.2, r3.r);
    r1.r = 4.5;
    assert_equal(4.5, r1.r);
    assert_equal(3.2, r2.r);
    assert_equal(3.2, r3.r);
  }
};