#include "value_test_helper.h"


class ErrorTest : public ValueTestHelper
{
public:
  void test_create_cast( void )
  {
    Error n("Something bad happened.");
    assert_id(n,1);
    TS_ASSERT(n.message());
    TS_ASSERT_EQUALS( std::string(n.message()), "Something bad happened.");
    // anonymize
    Value sig(n);
    
    // cannot get as string
    String n2;
    TS_ASSERT(!sig.set(n2));
    
    // can get as error
    Error e;
    TS_ASSERT(sig.set(e));
    TS_ASSERT_EQUALS( std::string(e.message()), "Something bad happened.");
  }
  
  void test_is_a( void )
  {
    Error e;
    Error e2("Bad");
    Value v;
    TS_ASSERT(!e.is_error());
    TS_ASSERT(e2.is_error());
    TS_ASSERT(!v.is_error());
    e2.set(v);
    TS_ASSERT(v.is_error());
  }
  
  void test_operator_equal( void )
  {
    Error n("Something bad happened.");
    TS_ASSERT( n == "Something bad happened.");
    TS_ASSERT( n == std::string("Something bad happened."));
  }
  
  void test_stream( void )
  {
    std::ostringstream out(std::ostringstream::out);
    Error v;
    out << v;
    TS_ASSERT_EQUALS(out.str(), "Nil");
    v = "Bad things happen.";
    out.str(std::string(""));
    out << v;
    TS_ASSERT_EQUALS(out.str(), "[1] #\"Bad things happen.\"");
  }
};
