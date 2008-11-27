#include "value_test_helper.h"
//#include "error_value.h"


class ErrorTest : public ValueTestHelper
{
public:
  void test_todo( void )
  {
    TS_ASSERT(false);
  }
};
