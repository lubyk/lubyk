#include "test_helper.h"

class CutTest : public CxxTest::TestSuite, public ParseTest
{
public:
  void test_single( void ) 
  { 
    setup_with_print("n=Cut(from:2 to:5)\n");
    
    assert_print("n.bang(1,2,3,4,5,6)\n",   "<Array [  2.00  3.00  4.00  5.00 ], 4>\n");
  }
  
};