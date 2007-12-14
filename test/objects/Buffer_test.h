#include "test_helper.h"


class BufferObjectTest : public CxxTest::TestSuite, public ParseTest
{
public:
  void test_single( void ) 
  { 
    setup_with_print("n=Buffer(3)\n");
    
    assert_print("n.b(1)\n", "<Matrix [  0.00  0.00  1.00 ], 3>\n");
    assert_print("n.b(2)\n", "<Matrix [  0.00  1.00  2.00 ], 3>\n");
    assert_print("n.b(3)\n", "<Matrix [  1.00  2.00  3.00 ], 3>\n");
    assert_print("n.b(4)\n", "<Matrix [  2.00  3.00  4.00 ], 3>\n");
    assert_print("n.b(5)\n", "<Matrix [  3.00  4.00  5.00 ], 3>\n");
    assert_print("n.b(6)\n", "<Matrix [  4.00  5.00  6.00 ], 3>\n");
    assert_print("n.b(7)\n", "<Matrix [  5.00  6.00  7.00 ], 3>\n");
  }
  
 // void test_array( void )
 // { 
 //   setup_with_print("n=Peak(descent:0.5)\n");
 //   
 //   assert_print("n.bang(1, 4)\n",   "<Matrix [  1.00  4.00 ], 2>\n");
 //   assert_print("n.bang\n",         "<Matrix [  0.50  3.50 ], 2>\n");
 //   assert_print("n.bang\n",         "<Matrix [  0.00  3.00 ], 2>\n");
 //   assert_print("n.bang(3, 1)\n",   "<Matrix [  3.00  2.50 ], 2>\n");
 // }
  
};