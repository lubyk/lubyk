#include "test_helper.h"
#include "oscit/values.h"

class ErrorValueTest : public TestHelper
{  
public:
  void test_create( void ) {
    Value v(BAD_REQUEST_ERROR, "foo");
    
    assert_false(v.is_empty());
    assert_false(v.is_nil());
    assert_false(v.is_real());
    assert_false(v.is_string());
    assert_false(v.is_list());
    assert_true (v.is_error());
    assert_false(v.is_hash());
    assert_false(v.is_matrix());
    assert_false(v.is_any());
    
    assert_equal("foo", v.error_message());
    assert_equal(BAD_REQUEST_ERROR, v.error_code());
    
    assert_equal("E", v.type_tag());
    int i = H("E");
    assert_equal(i, v.type_id());
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

  void test_copy( void ) {
    Value v(BAD_REQUEST_ERROR, "foo");
    Value v2(v);
    Value v3;
    
    assert_true(v2.is_error());
    
    v3 = v;
    
    assert_true(v3.is_error());
    
    assert_equal("foo", v2.error_message());
    assert_equal(BAD_REQUEST_ERROR, v2.error_code());
    assert_equal("foo", v3.error_message());
    assert_equal(BAD_REQUEST_ERROR, v3.error_code());
    
    v.error_->set_message("bar");
    assert_equal("bar", v.error_message());
    assert_equal("bar", v2.error_message());
    assert_equal("bar", v3.error_message());
  }
  
  void test_set( void ) {
    Value v;

    v.set(BAD_REQUEST_ERROR, "foobar");
    assert_true(v.is_error());
    assert_equal("foobar", v.error_message());
  }
  
  void test_append( void ) {
    ErrorValue v(BAD_REQUEST_ERROR, "bzz");
    assert_equal("bzz", v.error_message());
    v.append(" says the bee");
    assert_equal("bzz says the bee", v.error_message());
  }
  
  // set_type_tag does not exist for errors.
  // void test_set_type_tag( void ) {}
  
  void test_set_type( void ) {
    Value v;
    
    v.set_type(ERROR_VALUE);
    assert_true(v.is_error());
    assert_equal(UNKNOWN_ERROR, v.error_code());
  }
  
  void test_to_json( void ) {
    Value v(NOT_FOUND_ERROR, "banana");
    std::ostringstream os(std::ostringstream::out);
    os << v;
    assert_equal("\"404 banana\"", os.str());
    assert_equal("\"404 banana\"", v.to_json());
  }
  
  void test_can_receive( void ) {
    Object object("foo", Value(BAD_REQUEST_ERROR, "bar").push_back("Receives errors..."));
    assert_false(object.can_receive(Value()));
    assert_true (object.can_receive(gNilValue));
    assert_false(object.can_receive(Value(1.23)));
    assert_false(object.can_receive(Value("foo")));
    assert_true (object.can_receive(Value(BAD_REQUEST_ERROR, "foo")));
    assert_false(object.can_receive(JsonValue("['','']")));
    assert_false(object.can_receive(HashValue()));
    assert_false(object.can_receive(MatrixValue(1,1)));
  }
};