// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "group.h"
#include "test_helper.h"


class GroupTest : public ValueTestHelper
{
public:
  void test_trigger( void )
  {
    Root root;
    DummyNumber * one = root.adopt(new DummyNumber("one", 1.0)); // [/one]
    Group       * grp = one->adopt(new Group("grp"));            // [/one/sub]
    DummyNumber * two = grp->adopt(new DummyNumber("two", 2.0)); // in group [/two]
    Value res;
    
    TS_ASSERT_EQUALS( one->url(),     std::string("/one") );
    TS_ASSERT_EQUALS( grp->url(),     std::string("/one/grp") );
    TS_ASSERT_EQUALS( two->url(),     std::string("/two") );
    
    
    res = root.call("/one");
    TS_ASSERT_EQUALS( Number(res).value(), 1.00);
    res = root.call("/two");
    TS_ASSERT_EQUALS(res.to_string(), "[3] #\"Object \'/two\' not found.\"");
    res = grp->root()->call("/two");
    TS_ASSERT_EQUALS( Number(res).value(), 2.00);
    res = grp->root()->call("/one");
    TS_ASSERT_EQUALS(res.to_string(), "[4] #\"Object \'/one\' not found.\"");
  }
  
  void test_expose( void )
  {
    TS_ASSERT( false ); // TODO
  }
};