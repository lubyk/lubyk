#include "test_helper.h"
#include "oscit/values.h"

class AnyValueTest : public TestHelper
{  
public:
  void test_is_any( void ) {
    Value v('*');
    
    assert_false(v.is_empty());
    assert_false(v.is_nil());
    assert_false(v.is_real());
    assert_false(v.is_string());
    assert_false(v.is_list());
    assert_false(v.is_error());
    assert_false(v.is_hash());
    assert_false(v.is_matrix());
    assert_false(v.is_midi());
    assert_true (v.is_any());
    
    assert_equal("*", v.type_tag());
  }
  
  void test_create_with_TypeTag( void ) {
    Value v(TypeTag("*"));
    
    assert_true(v.is_any());
  }
  
  void test_copy( void ) {
    Value v('*');
    Value v2(v);
    Value v3;
    
    assert_true(v2.is_any());
    
    v3 = v;
    
    assert_true(v3.is_any());
  }
  
  void test_set( void ) {
    Value v;
    
    v.set_any();
    assert_true(v.is_any());
  }
  
  void test_set_tag( void ) {
    Value v;
    
    v.set_type_tag("*");
    assert_true(v.is_any());
    assert_equal("*", v.type_tag());
  }
  
  void test_set_type( void ) {
    Value v;
    
    v.set_type(ANY_VALUE);
    assert_true(v.is_any());
  }
  
  void test_to_json( void ) {
    Value v('*');
    std::ostringstream os(std::ostringstream::out);
    os << v;
    assert_equal("null", os.str());
    assert_equal("null", v.to_json());
  }
  
  void test_any_input( void ) {
    Value any(AnyIO("La la."));
    assert_equal("*s", any.type_tag());
  }
  
  void test_json_value( void ) {
    JsonValue v("[[1,2],\"hello\"]");
    assert_true(v.is_list());
    assert_equal("[ff]s", v.type_tag());
    assert_equal("[[1, 2], \"hello\"]", v.to_json());
  }
  
  // from_json is not possible for AnyValue
};