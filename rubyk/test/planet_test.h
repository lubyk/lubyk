// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "planet.h"
#include "test_helper.h"


class PlanetTest : public ValueTestHelper
{
public:
  void test_trigger( void )
  {
    Root root;
    DummyNumber * one = root.adopt(new DummyNumber("one", 1.0));   // [/one]
    Planet      * venus = one->adopt(new Planet());                // [/one/sub]
    venus->set_name("venus");
    DummyNumber * two = venus->adopt(new DummyNumber("two", 2.0)); // in group [/two]
    Value res;
    
    TS_ASSERT_EQUALS( one->url(),     std::string("/one") );
    TS_ASSERT_EQUALS( venus->url(),     std::string("/one/venus") );
    TS_ASSERT_EQUALS( two->url(),     std::string("/two") );
    
    
    res = root.call("/one");
    TS_ASSERT_EQUALS( Number(res).value(), 1.00);
    res = root.call("/two");
    TS_ASSERT_EQUALS(res.to_string(), "[3] #\"Object \'/two\' not found.\"");
    res = venus->tree()->call("/two");
    TS_ASSERT_EQUALS( Number(res).value(), 2.00);
    res = venus->tree()->call("/one");
    TS_ASSERT_EQUALS(res.to_string(), "[4] #\"Object \'/one\' not found.\"");
  }
  
  void test_socket( void )
  {
    // FIXME
  }
};