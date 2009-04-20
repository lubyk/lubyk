#include "test_helper.h"
#include "oscit/values.h"

class StringValueTest : public TestHelper
{  
public:
  void test_create( void ) {
    Value v("foo");
    
    assert_false(v.is_empty());
    assert_false(v.is_nil());
    assert_false(v.is_real());
    assert_true (v.is_string());
    assert_false(v.is_list());
    assert_false(v.is_error());
    assert_false(v.is_hash());
    assert_false(v.is_matrix());
    
    assert_equal("foo", v.str());
    
    assert_equal("s", v.type_tag());
    
    int i = H("s");
    assert_equal(i, v.type_id());
  }
  
  void test_create_string_value( void ) {
    StringValue v("hello");
    StringValue v2;
    
    assert_true(v.is_string());
    assert_true(v2.is_string());
    
    
    assert_equal("hello", v.str());
    assert_equal("", v2.str());
  }
  
  void test_create_std_string( void ) {
    std::string str("foo");
    Value v(str);
    
    assert_false(v.is_empty());
    assert_false(v.is_nil());
    assert_false(v.is_nil());
    assert_false(v.is_real());
    assert_true (v.is_string());
    assert_false(v.is_error());
    assert_false(v.is_hash());
    assert_false(v.is_any());
    
    assert_equal("foo", v.str());
    
    str.append("bar");
    assert_equal("foobar", str);
    assert_equal("foo", v.str());
    
    assert_equal("s", v.type_tag());
  }
  
  void test_create_with_char( void ) {
    Value v('s');
    
    assert_true(v.is_string());
    assert_equal("", v.str());
  }
  
  void test_create_with_TypeTag( void ) {
    Value v(TypeTag("s"));
    
    assert_true(v.is_string());
    assert_equal("", v.str());
  }
  
  void test_copy( void ) {
    Value v("foo");
    assert_equal(1, v.string_->ref_count());
    
    Value * v2 = new Value(v);
    assert_equal(v.string_, v2->string_);
    assert_equal(2, v.string_->ref_count());
    assert_true(v2->is_string());
    
    Value v3;
    
    assert_true(v3.is_empty());
    
    v3 = v;
    assert_true(v3.is_string());
    assert_equal(v.string_, v3.string_);
    assert_equal(3, v.string_->ref_count());
    
    assert_equal("foo", v.str());
    assert_equal("foo", v2->str());
    assert_equal("foo", v3.str());
    
    v.str() = "bar";
    assert_equal("bar", v.str());
    assert_equal("bar", v2->str());
    assert_equal("bar", v3.str());
    delete v2;
    assert_equal(2, v.string_->ref_count());
  }
  
  void test_set( void ) {
    Value v;
    
    assert_true(v.is_empty());
    v.set("foobar");
    assert_true(v.is_string());
    assert_equal("foobar", v.str());
  }
  
  void test_set_type_tag( void ) {
    Value v;
    v.set_type_tag("s");
    assert_true(v.is_string());
    assert_equal("", v.str());
  }
  
  void test_set_type( void ) {
    Value v;
    
    v.set_type(STRING_VALUE);
    assert_true(v.is_string());
    assert_equal("", v.str());
  }

  void test_to_json( void ) {
    Value v("cake");
    std::ostringstream os(std::ostringstream::out);
    os << v;
    assert_equal("\"cake\"", os.str());
    assert_equal("\"cake\"", v.to_json());
  }
  
  void test_from_json( void ) {
    Value v(Json("\"This is some \\\"super\\\" string !\""));
    assert_true(v.is_string());
    assert_equal("This is some \"super\" string !", v.str());
  }
  
  void test_from_json_single_quote( void ) {
    Value v(Json("'It took 25\" for \\'John\\' to \\\"get here !'"));
    assert_true(v.is_string());
    assert_equal("\"It took 25\\\" for 'John' to \\\"get here !\"", v.to_json());
  }
};