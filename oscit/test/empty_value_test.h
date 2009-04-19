#include "test_helper.h"
#include "oscit/values.h"

class EmptyValueTest : public TestHelper
{  
public:
  void test_create( void ) {
    Value v;
    
    assert_true (v.is_empty());
    assert_false(v.is_nil());
    assert_false(v.is_real());
    assert_false(v.is_string());
    assert_false(v.is_list());
    assert_false(v.is_error());
    assert_false(v.is_hash());
    assert_false(v.is_matrix());
    assert_false(v.is_any());
    
    assert_equal("", v.type_tag());
  }
  
  void test_create_with_TypeTag( void ) {
    Value v(TypeTag(""));
    
    assert_true(v.is_empty());
  }
  
  void test_copy( void ) {
    Value v;
    Value v2(v);
    Value v3(1.2);
    
    assert_true(v3.is_real());
    
    v3 = v;
    
    assert_true(v3.is_empty());
    assert_true(v2.is_empty());
  }
  
  void test_set( void ) {
    Value v(1.2);
    
    assert_true(v.is_real());
    v.set_empty();
    assert_true(v.is_empty());
  }
  
  void test_set_tag( void ) {
    Value v(1.2);
    
    v.set_type_tag("");
    assert_true(v.is_empty());
  }
  
  void test_set_type( void ) {
    Value v(1.2);
    
    v.set_type(EMPTY_VALUE);
    assert_true(v.is_empty());
  }
  
  void test_to_json( void ) {
    Value v;
    std::ostringstream os(std::ostringstream::out);
    os << v;
    assert_equal("", os.str());
    assert_equal("", v.to_json());
  }
  
  void test_from_json( void ) {
    Value v(Json(""));
    assert_true(v.is_empty());
  }
};