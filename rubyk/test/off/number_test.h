#include "value_test_helper.h"
#include "number_value.h"


class NumberTest : public ValueTestHelper
{
public:
  void test_create( void )
  {
    Number n(123.456);
    TS_ASSERT_EQUALS( 123.456, n.value());
    // anonymize
    Value v(n);
    
    Number n2;
    TS_ASSERT(v.set(&n2));
    TS_ASSERT_EQUALS( 123.456, n2.value());
  }
  
  void test_operator_equal( void )
  {
    Number n;
    n = 123.456;
    TS_ASSERT_EQUALS( 123.456, n.value());
    n = 456.789;
    TS_ASSERT_EQUALS( 456.789, n.value());
  }
  
  void test_data( void )
  {
    Number n1(123.45);
    const NumberData * d1 = n1.data();
    TS_ASSERT_EQUALS( d1, n1.data());
    // alter n1 (no need to copy)
    n1 = 555.23;
    TS_ASSERT_EQUALS( d1, n1.data());
    
    Number n2(n1); // light copy
    const NumberData * d2 = n2.data();
    TS_ASSERT_EQUALS( d1, d2);
    TS_ASSERT_EQUALS( 555.23, n1.value());
    TS_ASSERT_EQUALS( 555.23, n2.value());
    // alter n1
    n1 = 888.12;
    d1 = n1.data(); // pointer to new data
    TS_ASSERT_EQUALS( 888.12, n1.value());
    TS_ASSERT_EQUALS( 555.23, n2.value());
    TS_ASSERT_DIFFERS( d1, d2);
  }
};