#include "value_test_helper.h"
//#include "error_value.h"


class ErrorTest : public ValueTestHelper
{
public:
  void test_create_cast( void )
  {
    Error n("Something bad happened.");
    assert_id(n,1);
    TS_ASSERT(n.string());
    TS_ASSERT_EQUALS( *n.string(), "Something bad happened.");
    // anonymize
    Value sig(n);
    
    // cannot get as string
    String n2;
    TS_ASSERT(!sig.set(n2));
    
    // can get as error
    Error e;
    TS_ASSERT(sig.set(e));
    TS_ASSERT_EQUALS( *e.string(), "Something bad happened.");
  }
  
  void test_operator_equal( void )
  {
    Error n("Something bad happened.");
    TS_ASSERT( n == "Something bad happened.");
    TS_ASSERT( n == std::string("Something bad happened."));
  }
};
