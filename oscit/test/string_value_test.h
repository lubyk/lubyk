#include "test_helper.h"
#include "oscit/values.h"

class StringValueTest : public TestHelper
{  
public:
  void test_create( void ) {
    Value v("foo");
    
    assert_false(v.is_nil());
    assert_false(v.is_real());
    assert_true (v.is_string());
    assert_false(v.is_list());
    assert_false(v.is_error());
    assert_false(v.is_hash());
    
    assert_equal("foo", v.s);
    
    assert_equal("s", v.type_tag());
  }
  
  void test_create_string_value( void ) {
    StringValue v("hello");
    StringValue v2;
    
    assert_true(v.is_string());
    assert_true(v2.is_string());
    
    
    assert_equal("hello", v.s);
    assert_equal("", v2.s);
  }
  
  void test_create_std_string( void ) {
    std::string str("foo");
    Value v(str);
    
    assert_false(v.is_nil());
    assert_false(v.is_real());
    assert_true (v.is_string());
    assert_false(v.is_error());
    assert_false(v.is_hash());
    
    assert_equal("foo", v.s);
    
    str.append("bar");
    assert_equal("foobar", str);
    assert_equal("foo", v.s);
    
    assert_equal("s", v.type_tag());
  }
  
  void test_create_with_char( void ) {
    Value v('s');
    
    assert_true(v.is_string());
    assert_equal("", v.s);
  }
  
  void test_create_with_TypeTag( void ) {
    Value v(TypeTag("s"));
    
    assert_true(v.is_string());
    assert_equal("", v.s);
  }
  
  void test_copy( void ) {
    Value v("foo");
    Value v2(v);
    Value v3;
    
    assert_true(v2.is_string());
    assert_true(v3.is_nil());
    
    v3 = v;
    
    assert_true(v3.is_string());
    
    assert_equal("foo", v.s);
    assert_equal("foo", v2.s);
    assert_equal("foo", v3.s);
    
    // WARNING: never use v.s = "some string" (crash guaranteed !).
    v.set("bar");
    assert_equal("bar", v.s);
    assert_equal("foo", v2.s);
    assert_equal("foo", v3.s);
  }
  
  void test_set( void ) {
    Value v;
    
    assert_true(v.is_nil());
    v.set("foobar");
    assert_true(v.is_string());
    assert_equal("foobar", v.s);
  }
  
  void test_set_type_tag( void ) {
    Value v;
    
    v.set_type_tag("s");
    assert_true(v.is_string());
    assert_equal("", v.s);
  }
  
  void test_set_type( void ) {
    Value v;
    
    v.set_type(STRING_VALUE);
    assert_true(v.is_string());
    assert_equal("", v.s);
  }

  void test_stream( void ) {
    Value v("cake");
    std::ostringstream os(std::ostringstream::out);
    os << v;
    assert_equal("\"cake\"", os.str());
  }
};