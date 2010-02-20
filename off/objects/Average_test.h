#include "test_helper.h"

class AverageTest : public ParseHelper
{
public:
  void test_sum( void ) 
  { 
    parse("n=Average()\nn=>p\n");
    
    // value  ==> value
    assert_print("n.bang(4.0)\n", "4.00\n");
    // vector ==> value
    assert_print("n.bang(-3.5, 3.7)\n", "0.10\n");
    assert_print("n.bang(-1.0, 2.0, 4.0, 1.0, -1.5)\n", "0.90\n");
    // matrix ==> vector
    assert_print("b = Buffer(2)\nb.bang(1.0, -1.0, 2.0)\nb=>n\nb.bang(-1.5, 3.0, 1.0)\n", "<Matrix [ -0.25  1.00  1.50 ], 1x3>\n");
  }
  
};