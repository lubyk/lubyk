#include "test_helper.h"

class DiffTest : public CxxTest::TestSuite, public ParseTest
{
public:
  
  void test_diff_values( void ) 
  { 
    setup_with_print("n=Diff(3)\n");

    assert_print("n.bang(1,2,3,4,5,6)\n",    "3.00\n");
  }

  void test_diff_vector( void ) 
  { 
    setup_with_print("b=Buffer(4)\nb=>n\nn=Diff(2)\n"); // 100 increments in 1 second

    // 100 [ms] minimum * 100 / 1000 => 10 steps ==> 64 + 10 = 74
    assert_print("b.bang(1,2,3)\n",  "<Matrix [  1.00  2.00  3.00 ], 1x3>\n");
    assert_print("b.bang(2,2,1)\n",  "<Matrix [  2.00  2.00  1.00 ], 1x3>\n");
    assert_print("b.bang(3,2,0)\n",  "<Matrix [  2.00  0.00 -3.00 ], 1x3>\n");
    assert_print("b.bang(4,2,-1)\n", "<Matrix [  2.00  0.00 -2.00 ], 1x3>\n");
  }

};