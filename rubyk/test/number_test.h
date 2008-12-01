#include "value_test_helper.h"

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
    TS_ASSERT(v.set(n2));
    TS_ASSERT_EQUALS( 123.456, n2.value());
  }
  
  void test_is_a( void )
  {
    Number e;
    Number e2(1.2);
    Value v;
    TS_ASSERT(!e.is_number());
    TS_ASSERT(e2.is_number());
    TS_ASSERT(!v.is_number());
    e2.set(v);
    TS_ASSERT(v.is_number());
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
  
  void test_set( void )
  {
    real_t d = 0;
    Number n(3.45);
    TS_ASSERT(n.set(d));
    TS_ASSERT_EQUALS(d, 3.45);
  }
  
  void test_or_default( void )
  {
    Number n(3.45);
    Number nothing;
    Value  niet;
    real_t d = nothing || 1.2;
    TS_ASSERT_EQUALS(d, 1.2);
    d = n || 8.9;
    TS_ASSERT_EQUALS(d, 3.45);
    
    int i = niet || 12;
    TS_ASSERT_EQUALS(i, 12);
    i = n || 89;
    TS_ASSERT_EQUALS(i, 3);
  
  }
  
  void test_type_conversion( void )
  {
    Number n(3.45);
    double d2 = n;
    int    i2 = n;
    TS_ASSERT_EQUALS(d2, 3.45);
    TS_ASSERT_EQUALS(i2, 3);
  }
  
  void test_stream( void )
  {
    std::ostringstream out(std::ostringstream::out);
    Number v;
    out << v;
    TS_ASSERT_EQUALS(out.str(), "Nil");
    v = 1.23;
    out.str(std::string(""));
    out << v;
    TS_ASSERT_EQUALS(out.str(), "[1] 1.23");
  }
};