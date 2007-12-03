#include "test_helper.h"

class CutTest : public CxxTest::TestSuite, public ParseTest
{
public:
  void test_cut_zone( void ) 
  { 
    setup_with_print("n=Cut(from:2 to:5)\n"); // index by 1 in scripts
    
    assert_print("n.bang(1,2,3,4,5,6)\n",   "<Matrix [  2.00  3.00  4.00  5.00 ], 4>\n");
  }
  
  void test_cut_negative_index( void ) 
  { 
    setup_with_print("n=Cut(from:-3 to:-1)\n");
    
    assert_print("n.bang(1,2,3,4,5,6)\n",   "<Matrix [  4.00  5.00  6.00 ], 3>\n");
  }
  
};