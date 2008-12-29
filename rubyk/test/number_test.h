#include "test_helper.h"

class NumberTest : public ValueTestHelper
{
public:
  void test_create( void )
  {
    Number n(123.456);
    TS_ASSERT_EQUALS( n.value(), 123.456);
    // anonymize
    Value v(n);
    
    Number n2;
    TS_ASSERT(v.set(n2));
    TS_ASSERT_EQUALS( n2.value(), 123.456);
  }
  
  void test_from_string( void )
  {
    Value n("1.45");
    TS_ASSERT(n.is_number());
    TS_ASSERT_EQUALS( Number(n).value(), 1.45);
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
  
  void test_operator_plus_plus( void )
  {
    Number n;
    n = 123.12;
    TS_ASSERT_EQUALS( n.value(), 123.12);
    n++;
    TS_ASSERT_EQUALS( n.value(), 124.12);
  }
  
  void test_operator_equal( void )
  {
    Number n;
    n = 123.456;
    TS_ASSERT_EQUALS( n.value(), 123.456);
    n = 456.789;
    TS_ASSERT_EQUALS( n.value(), 456.789);
  }
  
  void test_operator_equal_integer( void )
  {
    Number n;
    n = 123;
    TS_ASSERT_EQUALS( n.value(), 123.0);
    n = 456.789;
    TS_ASSERT_EQUALS( n.value(), 456.789);
  }
  
  void test_operator_plus_equal( void )
  {
    Number n;
    Number n2(5);
    n = 123.45;
    TS_ASSERT_EQUALS( n.value(), 123.45);
    n += 1.0;
    TS_ASSERT_EQUALS( n.value(), 124.45);
    n += n2;
    TS_ASSERT_EQUALS( n.value(), 129.45);
  }
  
  void test_operator_greater( void )
  {
    Number n;
    Number n2(5);
    
    TS_ASSERT(n2 > n);
    TS_ASSERT(n2 < 10.0);
    TS_ASSERT(15.0 > n2);
    TS_ASSERT(3.0  < n2);
    
    TS_ASSERT(n2 >= n);
    TS_ASSERT(n2 <= 10.0);
    TS_ASSERT(15.0 >= n2);
    TS_ASSERT(3.0  <= n2);
    
    TS_ASSERT(n2 > n);
    TS_ASSERT(n2 < 10);
    TS_ASSERT(15 > n2);
    TS_ASSERT(3  < n2);
    
    TS_ASSERT(n2 >= n);
    TS_ASSERT(n2 <= 10);
    TS_ASSERT(15 >= n2);
    TS_ASSERT(3  <= n2);
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
    TS_ASSERT_EQUALS( n1.value(), 555.23);
    TS_ASSERT_EQUALS( n2.value(), 555.23);
    // alter n1
    n1 = 888.12;
    d1 = n1.data(); // pointer to new data
    TS_ASSERT_EQUALS( n1.value(), 888.12);
    TS_ASSERT_EQUALS( n2.value(), 555.23);
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