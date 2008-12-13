#include "test_helper.h"


class CaboxTest : public ParseHelper
{
public:
  void test_get_frame( void ) 
  { 
    parse("n=Cabox(buffer:2)\nn=>p\n");
    
    assert_print("n.b(255,255,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,10,13)\n",   
    "<Matrix [  0.00  0.00  0.00  0.00  0.01  0.01  0.01  0.01  0.01  0.01  0.01  0.01  0.01  0.01  0.01  0.01 ], 2x12>\n");
  }
  
};