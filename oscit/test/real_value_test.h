#include "test_helper.h"
#include "oscit/values.h"

class RealValueTest : public TestHelper
{  
public:
  void test_create( void ) {
    Value v(3.5);
    
    assert_false(v.is_empty());
    assert_false(v.is_nil());
    assert_true (v.is_real());
    assert_false(v.is_string());
    assert_false(v.is_list());
    assert_false(v.is_error());
    assert_false(v.is_hash());
    assert_false(v.is_matrix());
    assert_false(v.is_any());
    
    
    assert_equal(3.5, v.r); // real
    assert_equal(3.5, v.f); // alias as float
    assert_equal(3.5, v.d); // alias as double
    
    assert_equal("f", v.type_tag());
    int i = H("f");
    assert_equal(i, v.type_id());
  }
  
  void test_create_real_value( void ) {
    RealValue v(3.5);
    RealValue v2;
    
    assert_true(v.is_real());
    assert_true(v2.is_real());
    
    
    assert_equal(3.5, v.r);
    assert_equal(0.0, v2.r);
  }
  
  void test_create_with_char( void ) {
    Value v('f');
    
    assert_true(v.is_real());
    assert_equal(0.0, v.r);
  }
  
  void test_create_with_TypeTag( void ) {
    Value v(TypeTag("f"));
    
    assert_true(v.is_real());
    assert_equal(0.0, v.r);
  }
  
  void testCopy( void )
  {
    Value v(3.2);
    Value v2(v);
    Value v3;
    
    assert_true(v2.is_real());
    assert_true(v3.is_empty());
    
    v3 = v;
    
    assert_true(v3.is_real());
    
    assert_equal(3.2, v.r);
    assert_equal(3.2, v2.r);
    assert_equal(3.2, v3.r);
    v.r = 4.5;
    assert_equal(4.5, v.r);
    assert_equal(3.2, v2.r);
    assert_equal(3.2, v3.r);
  }
  
  void test_set( void )
  {
    Value v;
    
    assert_true(v.is_empty());
    v.set(3.5);
    assert_true(v.is_real());
    assert_equal(3.5, v.r);
  }
  
  void test_set_tag( void )
  {
    Value v;
    
    v.set_type_tag("f");
    assert_true(v.is_real());
    assert_equal(0.0, v.r);
  }
  
  void test_set_type( void )
  {
    Value v;
    
    v.set_type(REAL_VALUE);
    assert_true(v.is_real());
    assert_equal(0.0, v.r);
  }
  
  void test_to_json( void ) {
    Value v(1.35);
    std::ostringstream os(std::ostringstream::out);
    os << v;
    assert_equal("1.35", os.str());
    assert_equal("1.35", v.to_json());
  }
  
  void test_from_json( void ) {
    Value v(Json("2.45"));
    assert_true(v.is_real());
    assert_equal(2.45, v.r);
  }
};