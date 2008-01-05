#include "test_helper.h"


class BangTest : public CxxTest::TestSuite, public ParseTest
{
public:
  void test_abs( void ) 
  { 
    setup_with_print("n=Bang()\n");
    
    assert_print("n.bang(4.0)\n", "Bang!\n");
    assert_print("n.bang\n", "Bang!\n");
    assert_print("n.bang(1,2,3)\n", "Bang!\n");
  }
  
};