#include "test_helper.h"
#include "oscit/values.h"

class ListValueTest : public TestHelper
{  
public:
  void test_read( void ) {
    Value v(LIST_VALUE, "ff");
    List &l = *v.list;
    
    assertEqual(0.0, l[0].f);
    assertEqual(0.0, l[1].f);
    l[0].f = 3.5;
    assertEqual(3.5, l[0].f);
    assertEqual(0.0, l[1].f);
  }
  
  void test_should_not_share( void ) {
    Value v(LIST_VALUE, "ff");
    Value v2;
    List &l = *v.list;
    
    l[0].f = 1.0;
    l[1].f = 2.0;
    
    assertEqual(1.0, v[0].f);
    assertEqual(2.0, v[1].f);
    
    v2 = v;
    
    assertEqual(1.0, v2[0].f);
    assertEqual(2.0, v2[1].f);
    
    v[0].f = 3.0;
    assertEqual(3.0,  v[0].f);
    assertEqual(1.0, v2[0].f); // should not change
  }
};