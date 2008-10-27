// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "hash.h"


class TestStringHash : public CxxTest::TestSuite
{
public:
  void testSetGet( void )
  {
    Hash<std::string, std::string> hash(10);
    std::string res;
    
    hash.set(std::string("hello"), std::string("world"));
    hash.set(std::string("my"),    std::string("mum"));
    
    TS_ASSERT(hash.get(&res, "hello"));
    TS_ASSERT_EQUALS( res,  std::string("world") );
    TS_ASSERT(hash.get(&res, "my"));
    TS_ASSERT_EQUALS( res,  std::string("mum"  ) );
    TS_ASSERT_EQUALS(  hash.get(&res, "money"),  false );
  }
  
  void testGetDefault( void )
  {
    Hash<std::string, std::string> hash(10);
    std::string res;
    
    TS_ASSERT( !hash.get(&res));
    
    hash.set(std::string("hello"), std::string("world"));
    TS_ASSERT(hash.get(&res));
    TS_ASSERT_EQUALS( res,  std::string("world") );
    
    hash.set(std::string("my"),    std::string("mum"));
    TS_ASSERT(hash.get(&res));
    TS_ASSERT_EQUALS( res,  std::string("mum") );
  }

  void testKeys( void )
  {
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
  
  void test_clear( void )
  {
    Hash<std::string, std::string> hash(10);
    std::string res;
    
    hash.set(std::string("a"), std::string("world"));
    hash.set(std::string("b"), std::string("mum"));

    
    
    TS_ASSERT_EQUALS( 2,  hash.keys()->size());
    
    TS_ASSERT(hash.get(&res, "a"));
    TS_ASSERT_EQUALS( res,  std::string("world") );
    TS_ASSERT(hash.get(&res, "b"));
    TS_ASSERT_EQUALS( res,  std::string("mum"  ) );
    
    hash.clear();
    TS_ASSERT_EQUALS( 0,  hash.keys()->size());
    TS_ASSERT( !hash.get(&res, "a"));
    TS_ASSERT( !hash.get(&res, "b"));
  }
  
  void test_clear_int( void )
  {
    Hash<std::string, int> hash(10);
    int res;
    
    hash.set(std::string("a"), 1);
    hash.set(std::string("b"), 2);
    hash.set(std::string("c"), 3);

    
    TS_ASSERT_EQUALS( 3,  hash.keys()->size());
    
    TS_ASSERT(hash.get(&res, "a"));
    TS_ASSERT_EQUALS( res,  1);
    TS_ASSERT(hash.get(&res, "b"));
    TS_ASSERT_EQUALS( res,  2);
    TS_ASSERT(hash.get(&res, "c"));
    TS_ASSERT_EQUALS( res,  3);
    
    hash.clear();
    TS_ASSERT_EQUALS( 0,  hash.keys()->size());
    TS_ASSERT( !hash.get(&res, "a"));
    TS_ASSERT( !hash.get(&res, "b"));
    TS_ASSERT( !hash.get(&res, "c"));
  }
  
  void test_H( void )
  {
    TS_ASSERT_DIFFERS(H("a"), H("b"));
    TS_ASSERT_DIFFERS(H("one"), H("two"));
    TS_ASSERT_DIFFERS(H("longname_a"), H("longname_b"));
    TS_ASSERT_DIFFERS(H("longname_a"), H("longname_c"));
  }
};