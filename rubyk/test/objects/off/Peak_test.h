#include "test_helper.h"


class PeakTest : public ParseHelper
{
public:
  void test_single( void ) 
  { 
    parse("n=Peak(descent:0.5)\nn=>p\n");
    
    assert_print("n.bang(4)\n",   "4.00\n");
    assert_print("n.bang\n",      "3.50\n");
    assert_print("n.bang\n",      "3.00\n");
    assert_print("n.bang(3.5)\n", "3.50\n");
    assert_print("n.bang(0.5)\n", "3.00\n");
    assert_print("n.bang\n",      "2.50\n");
    assert_print("n.bang\n",      "2.00\n");
    assert_print("n.bang\n",      "1.50\n");
    assert_print("n.bang\n",      "1.00\n");
    assert_print("n.bang\n",      "0.50\n");
    assert_print("n.bang\n",      "0.00\n");
    assert_print("n.bang\n",      "0.00\n");
  }
  
  void test_array( void )
  { 
    parse("n=Peak(descent:0.5)\nn=>p\n");
    
    assert_print("n.bang(1, 4)\n",   "<Matrix [  1.00  4.00 ], 1x2>\n");
    assert_print("n.bang\n",         "<Matrix [  0.50  3.50 ], 1x2>\n");
    assert_print("n.bang\n",         "<Matrix [  0.00  3.00 ], 1x2>\n");
    assert_print("n.bang(3, 1)\n",   "<Matrix [  3.00  2.50 ], 1x2>\n");
  }
  
};