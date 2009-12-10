#include "test_helper.h"
#include "oscit/time_ref.h"

class TimeRefTest : public TestHelper
{
public:
  void test_create( void ) {
    TimeRef timer;
    assert_true( timer.elapsed() < 2);
  }

  void test_elapsed( void ) {
    TimeRef timer;
    millisleep(30);
    assert_true( timer.elapsed() >= 30);
  }
};

