#include "value_test_helper.h"


class BangTest : public ValueTestHelper
{
public:
  void test_create( void )
  {
    Bang b(true);
    Bang b2;
    TS_ASSERT( b2.is_nil());
    TS_ASSERT( b.is_bang());
    
    // anonymize
    Value v(b);
    TS_ASSERT( v.is_bang());
  }
  
  void test_stream( void )
  {
    std::ostringstream out(std::ostringstream::out);
    Bang b(true);
    out << b;
    TS_ASSERT_EQUALS(out.str(), "Bang!");
  }
};