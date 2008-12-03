// must be run before all other tests (hence the 'aaa' name).
#include <cxxtest/TestSuite.h>
#include "class.h"

class SetupTest : public CxxTest::TestSuite
{
  virtual void setUp()
  {
    ClassListing::set_lib_path("lib");
  }
public:
  void test_lib_path( void ) 
  {
    TS_ASSERT_EQUALS(ClassListing::sClasses.url(), std::string(CLASS_ROOT));
    TS_ASSERT_EQUALS(ClassListing::get_lib_path(), std::string("lib"));
  }
};