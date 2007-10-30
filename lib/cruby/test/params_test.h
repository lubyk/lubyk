// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "params.h"
#include <string>
#include <iostream>


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
  
  void testSet( void ) {
    Params p;
    p.set("first","1");
    TS_ASSERT_EQUALS( (int)p.get("first",   3), 1 );
    TS_ASSERT_EQUALS( (int)p.get("second",  7), 7 );
    
    p.set("second","2");
    TS_ASSERT_EQUALS( (int)p.get("second",  7), 2 );
    
    p.set("name", "Gaspard Buma");
    p.set("joy", "3.5");
    TS_ASSERT_EQUALS( (int)p.get("default", 3), 3 );
    
    TS_ASSERT_EQUALS( p.get("name", std::string("no-name"))    , std::string("Gaspard Buma"));
    TS_ASSERT_EQUALS( p.get("surnname", std::string("no-name")), std::string("no-name")     );
    
    TS_ASSERT_EQUALS( (float)p.get("joy", 1.1), (float)3.5 );
    TS_ASSERT_EQUALS( (float)p.get("bad", 1.1), (float)1.1 );
  }
  
  void testOstream( void ) {
    Params p("first:1 second:2 name:\"Gaspard Buma\" joy:3.5");
    std::ostringstream res(std::ostringstream::out); // allow output operations
    res << p;
    TS_ASSERT_EQUALS( res.str(), std::string("first:1 second:2 name:\"Gaspard Buma\" joy:3.5") );
  }
  
  void testClear( void ) {
    Params p("first:1 second:2 name:\"Gaspard Buma\" joy:3.5");
    std::ostringstream res1(std::ostringstream::out); // allow output operations
    std::ostringstream res2(std::ostringstream::out); // allow output operations
    res1 << p;
    TS_ASSERT_EQUALS( res1.str(), std::string("first:1 second:2 name:\"Gaspard Buma\" joy:3.5") );
    p.clear();
    res2 << p;
    TS_ASSERT_EQUALS( res2.str(), std::string("") );
  }
};