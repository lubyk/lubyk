// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "hash.h"
#include <string>


class TestStringHash : public CxxTest::TestSuite
{
public:
  void testSetGet( void ) {
    Hash<std::string, std::string> hash(10);
    
    hash.set(std::string("hello"), std::string("world"));
    hash.set(std::string("my"),    std::string("mum"));
    
    TS_ASSERT_EQUALS( *hash.get("hello"),  std::string("world") );
    TS_ASSERT_EQUALS( *hash.get("my"   ),  std::string("mum"  ) );
    TS_ASSERT_EQUALS(  hash.get("money"),  (std::string*)NULL );
  }
  
  void testKeys( void ) {
    Hash<std::string, std::string> hash(10);
    const std::vector<std::string> * keys = hash.keys();
    
    hash.set(std::string("hello"), std::string("world"));
    TS_ASSERT_EQUALS( 1, keys->size());
    TS_ASSERT_EQUALS( keys->at(0), std::string("hello"));
    
    hash.set(std::string("my"),    std::string("mum"));
    TS_ASSERT_EQUALS( 2, keys->size());
    TS_ASSERT_EQUALS( keys->at(0), std::string("hello"));
    TS_ASSERT_EQUALS( keys->at(1), std::string("my"));
    
    hash.remove(std::string("bob")); // does nothing
    TS_ASSERT_EQUALS( keys->at(0), std::string("hello"));
    TS_ASSERT_EQUALS( keys->at(1), std::string("my"));
    
    hash.remove(std::string("hello"));
    TS_ASSERT_EQUALS( 1, keys->size());
    TS_ASSERT_EQUALS( keys->at(0), std::string("my"));
  }
  
  void testClear( void ) {
    Hash<std::string, std::string> hash(10);

    hash.set(std::string("hello"), std::string("world"));
    hash.set(std::string("my"),    std::string("mum"));

    TS_ASSERT_EQUALS( 2,  hash.keys()->size());
    TS_ASSERT_EQUALS( *hash.get("hello"),  std::string("world") );
    TS_ASSERT_EQUALS( *hash.get("my"   ),  std::string("mum"  ) );
    
    hash.clear();
    TS_ASSERT_EQUALS( 0,  hash.keys()->size());
    TS_ASSERT_EQUALS( hash.get("hello"),  (std::string*)NULL );
    TS_ASSERT_EQUALS( hash.get("my"   ),  (std::string*)NULL );
  }
};