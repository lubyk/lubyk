#include "test_helper.h"


class AbsTest : public ParseTest
{
public:
  void test_abs( void ) 
  { 
    parse("n=Abs()\nn=>p\n");
    
    // real_ts
    assert_print("n.bang(4.0)\n", "4.00\n");
    assert_print("n.bang(-3.5)\n", "3.50\n");
    assert_print("n.bang(-1.0)\n", "1.00\n");
    assert_print("n.bang(0.0)\n", "0.00\n");
    // matrix
    assert_print("n.bang(1.0,-2.0,-0.4,0.0)\n", "<Matrix [  1.00  2.00  0.40  0.00 ], 1x4>\n");
  }
  
};