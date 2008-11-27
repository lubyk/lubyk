// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "values.h"

static real_t cast_tester(real_t v)
{
  return v;
}

class ValueTest : public CxxTest::TestSuite
{
public:
  void testType( void )
  {
    Value sig;

    TS_ASSERT_EQUALS(sig.type, NilValue);

    sig.set(34.0);

    TS_ASSERT_EQUALS(sig.type, DoubleValue);
  }
  
  void testBool( void )
  {
    Value sig;
    bool res = false;
    
    sig.set(true);
    sig.get(&res);
    TS_ASSERT(res);
    
    sig.set(false);
    sig.get(&res);
    TS_ASSERT(!res);
  }
  
  void testString( void )
  {
    Value sig;
    sig.set(std::string("Foo"));
    
    TS_ASSERT_EQUALS(StringValue, sig.type);
    TS_ASSERT_EQUALS(*(sig.str_ptr.value), std::string("Foo"));
  }
  
  void testMeta( void )
  {  
    // FIXME: string meta values crash !
    // std::string name;
    real_t age;
    int    year;
    Value sig;
    
    sig.set(34.0);
    //sig.set_meta(H("name"), "John");
    sig.set_meta(H("age"), 32.0);
    sig.set_meta(H("year"), 1975);

    //TS_ASSERT(sig.get_meta(&name, H("name")));
    TS_ASSERT(sig.get_meta(&age, H("age")));
    TS_ASSERT(sig.get_meta(&year, H("year")));

    //TS_ASSERT_EQUALS(name, std::string("John"));
    TS_ASSERT_EQUALS(age, 32.0);
    TS_ASSERT_EQUALS(year, 1975);
  }
  
  void test_cast( void )
  {
    Value sig;
    sig.set(345);
    TS_ASSERT_EQUALS(cast_tester(sig), 345.0);
  }
};