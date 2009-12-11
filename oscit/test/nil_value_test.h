#include "test_helper.h"
#include "oscit/values.h"

class NilValueTest : public TestHelper
{
public:
  void test_is_nil( void ) {

    assert_false(gNilValue.is_empty());
    assert_true (gNilValue.is_nil());
    assert_false(gNilValue.is_real());
    assert_false(gNilValue.is_string());
    assert_false(gNilValue.is_list());
    assert_false(gNilValue.is_error());
    assert_false(gNilValue.is_hash());
    assert_false(gNilValue.is_matrix());
    assert_false(gNilValue.is_any());

    assert_equal("N", gNilValue.type_tag());
  }

  void test_create_with_char( void ) {
    Value v('N');

    assert_true(v.is_nil());
  }

  void test_create_with_TypeTag( void ) {
    Value v(TypeTag("N"));

    assert_true(v.is_nil());
  }

  void test_copy( void ) {
    Value v('N');
    Value v2(v);
    Value v3(1.2);

    assert_true(v3.is_real());

    v3 = v;

    assert_true(v3.is_nil());
    assert_true(v2.is_nil());
  }

  void test_set( void ) {
    Value v(1.2);

    assert_true(v.is_real());
    v.set_nil();
    assert_true(v.is_nil());
  }

  void test_set_tag( void ) {
    Value v(1.2);

    v.set_type_tag("N");
    assert_true(v.is_nil());
  }

  void test_set_type( void ) {
    Value v(1.2);

    v.set_type(NIL_VALUE);
    assert_true(v.is_nil());
  }

  void test_to_json( void ) {
    Value v('N');
    std::ostringstream os(std::ostringstream::out);
    os << v;
    assert_equal("null", os.str());
    assert_equal("null", v.to_json());
  }

  void test_from_json( void ) {
    Value v(Json("null"));
    assert_true(v.is_nil());
  }

  void test_equal( void ) {
    Value a('N');
    Value b('N');
    assert_equal(a, b);
    a.set(3);
    assert_false(a == b);
    a.set_nil();
    assert_equal(a, b);
  }
};