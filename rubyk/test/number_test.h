// loop buffer test
// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "number.h"


class NumberTest : public CxxTest::TestSuite
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
};