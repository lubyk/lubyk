// must be run before all other tests (hence the 'aaa' name).
#include <cxxtest/TestSuite.h>
#include "class.h"

class SetupTest : public CxxTest::TestSuite
{
  virtual void setUp()
  {
    Class::set_lib_path("lib");
  }
public:
  void test_lib_path( void ) 
  {
    TS_ASSERT_EQUALS(Class::get_lib_path(), std::string("lib"));
  }
};