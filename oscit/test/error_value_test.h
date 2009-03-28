#include "test_helper.h"
#include "oscit/values.h"

class ErrorValueTest : public TestHelper
{  
public:
  void test_create( void ) {
    Value v(BAD_REQUEST_ERROR, "foo");
    
    assert_false(v.is_nil());
    assert_false(v.is_real());
    assert_false(v.is_string());
    assert_false(v.is_list());
    assert_true(v.is_error());
    
    assert_equal("foo", v.error_message());
    assert_equal(BAD_REQUEST_ERROR, v.error_code());
    
    assert_equal("s", v.type_tag());
  }
  
  void test_create_error_value( void ) {
    ErrorValue v(BAD_REQUEST_ERROR, "bad");
    ErrorValue v2;
    
    assert_true(v.is_error());
    assert_true(v2.is_error());
    
    
    assert_equal("bad", v.error_message());
    assert_equal("unknown error", v2.error_message());
    assert_equal(UNKNOWN_ERROR, v2.error_code());
  }
  
  void test_create_error_value_std_string( void ) {
    ErrorValue v(BAD_REQUEST_ERROR, std::string("bad"));
    Value v2(BAD_REQUEST_ERROR, std::string("bad2"));
    
    assert_true(v.is_error());
    assert_true(v2.is_error());
    
    
    assert_equal("bad", v.error_message());
    assert_equal("bad2", v2.error_message());
    assert_equal(BAD_REQUEST_ERROR, v.error_code());
    assert_equal(BAD_REQUEST_ERROR, v2.error_code());
  }
//  void test_copy( void ) {
//    Value v("foo");
//    Value v2(v);
//    Value v3;
//    
//    assert_true(v2.is_string());
//    assert_true(v3.is_nil());
//    
//    v3 = v;
//    
//    assert_true(v3.is_string());
//    
//    assert_equal("foo", v.s);
//    assert_equal("foo", v2.s);
//    assert_equal("foo", v3.s);
//    
//    // WARNING: never use v.s = "some string" (crash guaranteed !).
//    v.set("bar");
//    assert_equal("bar", v.s);
//    assert_equal("foo", v2.s);
//    assert_equal("foo", v3.s);
//  }
//  
//  void test_set( void ) {
//    Value v;
//    
//    assert_true(v.is_nil());
//    v.set("foobar");
//    assert_true(v.is_string());
//    assert_equal("foobar", v.s);
//  }
//  
//  void test_set_type_tag( void ) {
//    Value v;
//    
//    v.set_type_tag("s");
//    assert_true(v.is_string());
//    assert_equal("", v.s);
//  }
//  
//  void test_set_type( void ) {
//    Value v;
//    
//    v.set_type(STRING_VALUE);
//    assert_true(v.is_string());
//    assert_equal("", v.s);
//  }
};