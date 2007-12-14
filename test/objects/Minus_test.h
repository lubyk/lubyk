#include "test_helper.h"

class MinusTest : public CxxTest::TestSuite, public ParseTest
{
public:
  void test_single( void ) 
  { 
    setup_with_print("n=Minus(0.2)\n");
    
    assert_print("n.bang(2.3)\n",   "2.10\n");
  }
  
  void test_array( void ) 
  { 
    setup_with_print("n=Minus(2)\n");
    
    assert_print("n.bang(1,2,3,4)\n",   "<Matrix [ -1.00  0.00  1.00  2.00 ], 1x4>\n");
  }
  
};