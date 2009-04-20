#include "test/test_helper.h"
#include "oscit/thash.h"


class StringTHashTest : public TestHelper
{
public:
  void test_set_get( void ) {
    THash<std::string, std::string> hash(10);
    std::string res;
    
    hash.set(std::string("hello"), std::string("world"));
    hash.set(std::string("my"),    std::string("mum"));
    
    assert_true(hash.get("hello", &res));
    assert_equal("world", res);
    assert_true(hash.get("my", &res));
    assert_equal("mum", res);
    assert_false(hash.get("money", &res));
  }
  
  void test_set_get_empty( void ) {
    THash<std::string, std::string> hash(10);
    std::string res;

    hash.set(std::string(""), std::string("empty"));
    hash.set(std::string("other"),    std::string("string"));

    assert_true(hash.get("", &res));
    assert_equal("empty", res);
    assert_true(hash.get("other", &res));
    assert_equal("string", res);
  }
  
  void test_get_key( void ) {
    THash<std::string, std::string> hash(10);
    std::string res;
    
    hash.set(std::string("hello"), std::string("world"));
    hash.set(std::string("my"),    std::string("mum"));
    
    assert_true(hash.get_key("world", &res));
    assert_equal("hello", res);
    assert_true(hash.get_key("mum", &res));
    assert_equal("my", res);
    assert_false(hash.get_key("money", &res));
  }
  
  void test_keys( void ) {
    THash<std::string, std::string> hash(10);
    const std::list<std::string> * keys = &hash.keys();
    
    hash.set(std::string("hello"), std::string("world"));
    assert_equal(1, keys->size());
    assert_equal("hello", keys->front());
    
    hash.set(std::string("my"),    std::string("mum"));
    assert_equal(2, keys->size());
    assert_equal("hello", keys->front());
    assert_equal("my", keys->back());
    
    hash.remove(std::string("bob")); // does nothing
    assert_equal("hello", keys->front());
    assert_equal("my", keys->back());
    
    hash.remove(std::string("hello"));
    assert_equal(1, keys->size());
    assert_equal("my", keys->front());
  }
  
  void test_clear( void ) {
    THash<std::string, std::string> hash(10);
    std::string res;
    
    hash.set(std::string("a"), std::string("world"));
    hash.set(std::string("b"), std::string("mum"));
  
    
    
    assert_equal(2, hash.keys().size());
    
    assert_true(hash.get("a", &res));
    assert_equal( res,  std::string("world") );
    assert_true(hash.get("b", &res));
    assert_equal( res,  std::string("mum"  ) );
    
    hash.clear();
    assert_equal(0, hash.keys().size());
    assert_false(hash.get("a", &res));
    assert_false(hash.get("b", &res));
  }
  
  void test_clear_int( void ) {
    THash<std::string, int> hash(10);
    int res;
    
    hash.set(std::string("a"), 1);
    hash.set(std::string("b"), 2);
    hash.set(std::string("c"), 3);
  
    
    assert_equal(3, hash.size());
    
    assert_true(hash.get("a", &res));
    assert_equal(1, res);
    assert_true(hash.get("b", &res));
    assert_equal(2, res);
    assert_true(hash.get("c", &res));
    assert_equal(3, res);
    
    hash.clear();
    assert_equal(0,  hash.size());
    assert_false(hash.get("a", &res));
    assert_false(hash.get("b", &res));
    assert_false(hash.get("c", &res));
  }
  
  void test_H( void ) {
    assert_false(H("a") == H("b"));
    assert_false(H("one") == H("two"));
    assert_false(H("longname_a") == H("longname_b"));
    assert_false(H("longname_a") == H("longname_c"));
    // this is the upper limit for H() macro
    assert_true(H("longnamelong_a") == H("longnamelong_c"));
    // no upper limit for hashId() function
    assert_false(hashId("longnamelong_a") == hashId("longnamelong_c"));
  }
  
  void test_N_type_id( void ) {
    assert_false(H("N") == H("f"));
    assert_false(H("N") == H("s"));
    assert_false(H("N") == H("H"));
    assert_false(H("N") == H("M"));
    assert_false(H("N") == H("*"));
  }
  
  void test_f_type_id( void ) {
    assert_false(H("f") == H("N"));
    assert_false(H("f") == H("s"));
    assert_false(H("f") == H("H"));
    assert_false(H("f") == H("M"));
    assert_false(H("f") == H("*"));
  }
  
  void test_s_type_id( void ) {
    assert_false(H("s") == H("N"));
    assert_false(H("s") == H("f"));
    assert_false(H("s") == H("H"));
    assert_false(H("s") == H("M"));
    assert_false(H("s") == H("*"));
  }
  
  void test_H_type_id( void ) {
    assert_false(H("H") == H("N"));
    assert_false(H("H") == H("f"));
    assert_false(H("H") == H("s"));
    assert_false(H("H") == H("M"));
    assert_false(H("H") == H("*"));
  }
  
  void test_M_type_id( void ) {
    assert_false(H("M") == H("N"));
    assert_false(H("M") == H("f"));
    assert_false(H("M") == H("s"));
    assert_false(H("M") == H("H"));
    assert_false(H("M") == H("*"));
  }
  
  void test_any_type_id( void ) {
    assert_false(H("*") == H("N"));
    assert_false(H("*") == H("f"));
    assert_false(H("*") == H("s"));
    assert_false(H("*") == H("H"));
    assert_false(H("*") == H("M"));
  }
  
  void test_copy( void ) {
    THash<std::string, std::string> hash(10);
    THash<std::string, std::string> hash2(10);
    std::string res;

    hash.set(std::string("hello"), std::string("world"));
    hash.set(std::string("my"),    std::string("mum"));
    
    // copy
    hash2 = hash;
    
    // change hash
    hash.set(std::string("hello"), std::string("planet"));

    assert_true(hash.get("hello", &res));
    assert_equal("planet", res);
    assert_true(hash.get("my", &res));
    assert_equal("mum", res);
    
    assert_true(hash2.get("hello", &res));
    assert_equal("world", res);
    assert_true(hash2.get("my", &res));
    assert_equal("mum", res);
  }
};