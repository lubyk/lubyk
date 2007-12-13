#include "test_helper.h"

class ReplayTest : public CxxTest::TestSuite, public ParseTest
{
public:
  void test_play( void ) 
  { 
    setup_with_print("n=Replay(vector:5 file:\"test/fixtures/replay.rec\")\nn.play\n");
    
    assert_print("n.b\n",   "<Matrix [  1.00  2.00  3.00  4.00  5.00 ], 5>\n");
    assert_print("n.b\n",   "<Matrix [  11.00  12.00  13.00  14.00  15.00 ], 5>\n");
    assert_print("n.b\n",   "<Matrix [  21.00  22.00  23.00  24.00  25.00 ], 5>\n");
  }
};