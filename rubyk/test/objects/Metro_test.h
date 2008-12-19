#include "test_helper.h"


class MetroTest : public ParseHelper
{
public:
  
  void test_bang_bang( void ) 
  { 
    setup_with_print("n=Metro(1000)\n");
    assert_run((60 * 3) + 10, "Bang!\nBang!\nBang!\n");  // 0 ... 20 ... 60 [bang] ... 120 [bang] ... 180 [bang] ... 190.
  }
};