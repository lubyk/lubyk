// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "test_helper.h"


class AliasNodeTest : public NodeTestHelper
{
public:
  void test_alias_of_method( void )
  {
    create("/grp", "Group",  ""            , "[3] \"/grp\"");
    create("/grp/one", "Number", "value: 1", "[9] \"/grp/one\"");
    create("/grp/ali", "Alias",  "original: /grp/one/value", "[15] \"/grp/ali\"");
    
    assert_call("/grp/one/value", "", "[7] 1.00");
    assert_call("/grp/ali", "3.45",   "[7] 3.45"); // /grp/ali ---> /grp/one/value
    assert_call("/grp/one/value", "", "[7] 3.45");
  }
};