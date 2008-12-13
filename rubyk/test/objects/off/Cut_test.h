#include "test_helper.h"

class CutTest : public ParseHelper
{
public:
  void test_cut_zone( void ) 
  { 
    parse("n=Cut(from:2 to:5)\nn=>p\n"); // index by 1 in scripts
    
    assert_print("n.bang(1,2,3,4,5,6)\n",   "<Matrix [  2.00  3.00  4.00  5.00 ], 1x4>\n");
  }
  
  void test_cut_negative_index( void ) 
  { 
    parse("n=Cut(from:-3 to:-1)\nn=>p\n");
    
    assert_print("n.bang(1,2,3,4,5,6)\n",   "<Matrix [  4.00  5.00  6.00 ], 1x3>\n");
  }
  
};