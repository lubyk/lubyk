#include "test_helper.h"


class MetroTest : public ParseHelper
{
public:
  
  void test_bang_bang( void ) 
  { 
    setup_with_print("n=Metro(1000)\n");
    assert_run(60 * 4, "Bang!\nBang!\nBang!\nBang!\n");  
  }
};