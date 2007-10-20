// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "hash.h"
#include <string>


class TestStringHash : public CxxTest::TestSuite
{
public:
  void testSetGet( void ) {
    Hash<std::string, std::string> keys(10);
    
    keys.set(std::string("hello"), std::string("world"));
    keys.set(std::string("my"),    std::string("mum"));
    
    TS_ASSERT_EQUALS( *keys.get("hello"),  std::string("world") );
    TS_ASSERT_EQUALS( *keys.get("my"   ),  std::string("mum"  ) );
    TS_ASSERT_EQUALS(  keys.get("money"),  (std::string*)NULL );
  }
};