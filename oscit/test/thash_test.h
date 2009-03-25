// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "thash.h"


class StringTHashTest : public CxxTest::TestSuite
{
public:
  void test_set_get( void )
  {
    THash<std::string, std::string> hash(10);
    std::string res;
    
    hash.set(std::string("hello"), std::string("world"));
    hash.set(std::string("my"),    std::string("mum"));
    
    TS_ASSERT(hash.get(&res, "hello"));
    TS_ASSERT_EQUALS( res,  std::string("world") );
    TS_ASSERT(hash.get(&res, "my"));
    TS_ASSERT_EQUALS( res,  std::string("mum"  ) );
    TS_ASSERT_EQUALS(  hash.get(&res, "money"),  false );
  }
  
  void test_set_get_empty( void )
  {
    THash<std::string, std::string> hash(10);
    std::string res;

    hash.set(std::string(""), std::string("empty"));
    hash.set(std::string("other"),    std::string("string"));

    TS_ASSERT(hash.get(&res, ""));
    TS_ASSERT_EQUALS( res,  std::string("empty") );
    TS_ASSERT(hash.get(&res, "other"));
    TS_ASSERT_EQUALS( res,  std::string("string"  ) );
  }
  
  void test_get_key( void )
  {
    THash<std::string, std::string> hash(10);
    std::string res;
    
    hash.set(std::string("hello"), std::string("world"));
    hash.set(std::string("my"),    std::string("mum"));
    
    TS_ASSERT(hash.get_key(&res, "world"));
    TS_ASSERT_EQUALS( res,  std::string("hello") );
    TS_ASSERT(hash.get_key(&res, "mum"));
    TS_ASSERT_EQUALS( res,  std::string("my"  ) );
    TS_ASSERT_EQUALS(  hash.get_key(&res, "money"),  false );
  }
  
  void test_keys( void )
  {
    THash<std::string, std::string> hash(10);
    const std::list<std::string> * keys = &hash.keys();
    
    hash.set(std::string("hello"), std::string("world"));
    TS_ASSERT_EQUALS( 1, keys->size());
    TS_ASSERT_EQUALS( keys->front(), std::string("hello"));
    
    hash.set(std::string("my"),    std::string("mum"));
    TS_ASSERT_EQUALS( 2, keys->size());
    TS_ASSERT_EQUALS( keys->front(), std::string("hello"));
    TS_ASSERT_EQUALS( keys->back(), std::string("my"));
    
    hash.remove(std::string("bob")); // does nothing
    TS_ASSERT_EQUALS( keys->front(), std::string("hello"));
    TS_ASSERT_EQUALS( keys->back(), std::string("my"));
    
    hash.remove(std::string("hello"));
    TS_ASSERT_EQUALS( 1, keys->size());
    TS_ASSERT_EQUALS( keys->front(), std::string("my"));
  }
  
  void test_clear( void )
  {
    THash<std::string, std::string> hash(10);
    std::string res;
    
    hash.set(std::string("a"), std::string("world"));
    hash.set(std::string("b"), std::string("mum"));
  
    
    
    TS_ASSERT_EQUALS( 2,  hash.keys().size());
    
    TS_ASSERT(hash.get(&res, "a"));
    TS_ASSERT_EQUALS( res,  std::string("world") );
    TS_ASSERT(hash.get(&res, "b"));
    TS_ASSERT_EQUALS( res,  std::string("mum"  ) );
    
    hash.clear();
    TS_ASSERT_EQUALS( 0,  hash.keys().size());
    TS_ASSERT( !hash.get(&res, "a"));
    TS_ASSERT( !hash.get(&res, "b"));
  }
  
  void test_clear_int( void )
  {
    THash<std::string, int> hash(10);
    int res;
    
    hash.set(std::string("a"), 1);
    hash.set(std::string("b"), 2);
    hash.set(std::string("c"), 3);
  
    
    TS_ASSERT_EQUALS( 3,  hash.size());
    
    TS_ASSERT(hash.get(&res, "a"));
    TS_ASSERT_EQUALS( res,  1);
    TS_ASSERT(hash.get(&res, "b"));
    TS_ASSERT_EQUALS( res,  2);
    TS_ASSERT(hash.get(&res, "c"));
    TS_ASSERT_EQUALS( res,  3);
    
    hash.clear();
    TS_ASSERT_EQUALS( 0,  hash.size());
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
  
  void test_copy( void )
  {
    THash<std::string, std::string> hash(10);
    THash<std::string, std::string> hash2(10);
    std::string res;

    hash.set(std::string("hello"), std::string("world"));
    hash.set(std::string("my"),    std::string("mum"));
    
    // copy
    hash2 = hash;
    
    // change hash
    hash.set(std::string("hello"), std::string("planet"));

    TS_ASSERT(hash.get(&res, "hello"));
    TS_ASSERT_EQUALS( res,  std::string("planet") );
    TS_ASSERT(hash.get(&res, "my"));
    TS_ASSERT_EQUALS( res,  std::string("mum"  ) );
    
    TS_ASSERT(hash2.get(&res, "hello"));
    TS_ASSERT_EQUALS( res,  std::string("world") );
    TS_ASSERT(hash2.get(&res, "my"));
    TS_ASSERT_EQUALS( res,  std::string("mum"  ) );
  }
};