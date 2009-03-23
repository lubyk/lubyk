#include "test_helper.h"
#include "oscit/values.h"

class ListValueTest : public TestHelper
{  
public:
  void test_alloc( void )
  {
    Value v(Value::alloc(2));
    Value * l = v.list;
    
    assert_equal(0.0, l[0].f);
    assert_equal(0.0, l[1].f);
    l[0].f = 3.5;
    assert_equal(3.5, l[0].f);
    assert_equal(0.0, l[1].f);
    v.dealloc();
  }
  
  void test_share( void )
  {
    Value v(Value::alloc(2));
    Value v2;
    Value * l = v.list;
    
    l[0].f = 1.0;
    l[1].f = 2.0;
    
    assert_equal(1.0, v.list[0].f);
    assert_equal(2.0, v.list[1].f);
    
    v2 = v;
    
    assert_equal(1.0, v2.list[0].f);
    assert_equal(2.0, v2.list[1].f);
    
    v.list[0].f = 3.0;
    assert_equal(3.0,  v.list[0].f);
    assert_equal(3.0, v2.list[0].f);
    
    v.dealloc();
  }
};