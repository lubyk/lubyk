// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "Params.h"
#include <string>


class TestParams : public CxxTest::TestSuite
{
public:
  void testGet( void ) {
    Params p("first:1 second:2 name:\"Gaspard Buma\" joy:3.5");
    TS_ASSERT_EQUALS( (int)p.get("first",   3), 1 );
    TS_ASSERT_EQUALS( (int)p.get("default", 3), 3 );
    
    TS_ASSERT_EQUALS( p.get("name", std::string("no-name"))    , std::string("Gaspard Buma"));
    TS_ASSERT_EQUALS( p.get("surnname", std::string("no-name")), std::string("no-name")     );
    
    TS_ASSERT_EQUALS( (float)p.get("joy", 1.1), (float)3.5 );
    TS_ASSERT_EQUALS( (float)p.get("bad", 1.1), (float)1.1 );
  }
};