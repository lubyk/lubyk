#include "test_helper.h"


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
  
  void test_from_string( void )
  {
    Value b("Bang!");
    TS_ASSERT(b.is_bang());
  }
  
  void test_is_a( void )
  {
    Bang e;
    Bang e2(true);
    Value v;
    TS_ASSERT(!e.is_bang());
    TS_ASSERT(e2.is_bang());
    TS_ASSERT(!v.is_bang());
    e2.set(v);
    TS_ASSERT(v.is_bang());
  }
  
  void test_stream( void )
  {
    std::ostringstream out(std::ostringstream::out);
    Bang b(true);
    out << b;
    TS_ASSERT_EQUALS(out.str(), "Bang!");
  }
};