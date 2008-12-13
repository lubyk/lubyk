// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "test_helper.h"


class GroupTest : public NodeTestHelper
{
public:
  void test_new_inlet( void )
  {
    create("/one", "Number", "value: 1"    , "[5] \"/one\"");
    create("/grp", "Group",  ""            , "[9] \"/grp\"");
    create("/grp/two", "Number", "value: 2", "[15] \"/grp/two\"");
    create("/grp/val", "Inlet",  ""        , "[19] \"/grp/val\""); // /grp/val, /grp/in/val
    assert_call("/one/out/value/link", "/grp/in/val", "/grp/in/val");         // one ---> grp
    assert_call("/grp/val/out/port/link", "/grp/two/in/bang", "/grp/two/in/bang"); // val ---> two
    
    assert_call("/grp/two/value", "", "[13] 2.00");
    assert_call("/one/in/bang", "Bang!", "Nil"); // one ---> grp ---> Inlet (val) --> two
    assert_call("/grp/two/value", "", "[13] 1.00");
  }
  
  void test_new_outlet( void )
  {
    // FIXME
  }
};