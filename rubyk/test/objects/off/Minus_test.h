#include "test_helper.h"

class MinusTest : public ParseHelper
{
public:
  void test_single( void ) 
  { 
    parse("n=Minus(0.2)\nn=>p\n");
    
    assert_print("n.bang(2.3)\n",   "2.10\n");
  }
  
  void test_array( void ) 
  { 
    parse("n=Minus(2)\nn=>p\n");
    
    assert_print("n.bang(1,2,3,4)\n",   "<Matrix [ -1.00  0.00  1.00  2.00 ], 1x4>\n");
  }
  
};