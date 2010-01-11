#include "test_helper.h"

class SumTest : public ParseHelper
{
public:
  void test_sum( void ) 
  { 
    parse("n=Sum()\nn=>p\n");
    
    // value  ==> value
    assert_print("n.bang(4.0)\n", "4.00\n");
    // vector ==> value
    assert_print("n.bang(-3.5, 3.7)\n", "0.20\n");
    assert_print("n.bang(-1.0, 2.0, 4.0, 1.0, -1.5)\n", "4.50\n");
    // matrix ==> vector
    assert_print("b = Buffer(2)\nb.bang(1.0, -1.0, 2.0)\nb=>n\nb.bang(-1.5, 3.0, 1.0)\n", "<Matrix [ -0.50  2.00  3.00 ], 1x3>\n");
  }
  
};