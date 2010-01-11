#include "test_helper.h"


class BangTest : public ParseHelper
{
public:
  void test_abs( void ) 
  { 
    parse("n=Bang()\nn=>p\n");
    
    assert_print("n.bang(4.0)\n", "Bang!\n");
    assert_print("n.bang\n", "Bang!\n");
    assert_print("n.bang(1,2,3)\n", "Bang!\n");
  }
  
};