#include "test_helper.h"


class CaboxTest : public CxxTest::TestSuite, public ParseTest
{
public:
  void test_get_frame( void ) 
  { 
    setup_with_print("n=Cabox(buffer:2)\n");
    
    assert_print("n.b(255,255,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,10,13)\n",   
    "<Matrix [  0.00  0.00  0.00  0.00  0.01  0.01  0.01  0.01  0.01  0.01  0.01  0.01  0.01  0.01  0.01  0.01 ], 2x12>\n");
  }
  
};