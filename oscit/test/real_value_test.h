#include "test_helper.h"
#include "oscit/values.h"

class RealValueTest : public TestHelper
{
public:
  void test_is_real( void ) {
    Value v(3.5);

    assert_false(v.is_empty());
    assert_false(v.is_nil());
    assert_true (v.is_real());
    assert_false(v.is_string());
    assert_false(v.is_list());
    assert_false(v.is_error());
    assert_false(v.is_hash());
    assert_false(v.is_matrix());
    assert_false(v.is_midi());
    assert_false(v.is_any());


    assert_equal(3.5, v.r);

    assert_equal("f", v.type_tag());
    int i = H("f");
    assert_equal(i, v.type_id());
  }

  void test_true_value( void ) {
    assert_equal("f", gTrueValue.type_tag());
    assert_equal(1.0, gTrueValue.r);
    assert_true(gTrueValue.is_true());
    assert_true(Value(1.0).is_true());
  }

  void test_false_value( void ) {
    assert_equal("f", gFalseValue.type_tag());
    assert_equal(0.0, gFalseValue.r);
    assert_false(gFalseValue.is_true());
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

  void test_can_receive( void ) {
    Object object("foo", RealIO("bar", "info"));
    assert_false(object.can_receive(Value()));
    assert_true (object.can_receive(gNilValue));
    assert_true (object.can_receive(Value(1.23)));
    assert_false(object.can_receive(Value("foo")));
    assert_false(object.can_receive(Value(BAD_REQUEST_ERROR, "foo")));
    assert_false(object.can_receive(JsonValue("['','']")));
    assert_false(object.can_receive(HashValue()));
    assert_false(object.can_receive(MatrixValue(1,1)));
    assert_false(object.can_receive(MidiValue()));
  }


  void test_equal( void ) {
    Value a(1);
    Value b(1);
    assert_equal(a, b);
    a.set(2);
    assert_false(a == b);
    a.set(4).push_back(5);
    assert_false(a == b);
    a.set_nil();
    assert_false(a == b);
  }

  void test_get_real( void ) {
    Value a("hello");
    Value b;
    Value c(4.5);
    assert_equal(0, a.get_real());
    assert_equal(3, a.get_real(3));
    assert_equal(0, b.get_real());
    assert_equal(1.1, b.get_real(1.1));
    assert_equal(4.5, c.get_real());
    assert_equal(4.5, c.get_real(1.1));
  }
};