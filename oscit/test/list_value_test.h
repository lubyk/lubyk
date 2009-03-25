#include "test_helper.h"
#include "oscit/values.h"

class ListValueTest : public TestHelper
{  
public:
  void test_read( void ) {
    Value v(LIST_VALUE, "ff");
    List &l = *v.list;
    
    assert_equal(0.0, l[0].f);
    assert_equal(0.0, l[1].f);
    l[0].f = 3.5;
    assert_equal(3.5, l[0].f);
    assert_equal(0.0, l[1].f);
  }
  
  void test_should_not_share( void ) {
    Value v(LIST_VALUE, "ff");
    Value v2;
    List &l = *v.list;
    
    l[0].f = 1.0;
    l[1].f = 2.0;
    
    assert_equal(1.0, v[0].f);
    assert_equal(2.0, v[1].f);
    
    v2 = v;
    
    assert_equal(1.0, v2[0].f);
    assert_equal(2.0, v2[1].f);
    
    v[0].f = 3.0;
    assert_equal(3.0,  v[0].f);
    assert_equal(1.0, v2[0].f); // should not change
  }
};