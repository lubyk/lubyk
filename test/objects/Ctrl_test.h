#include "test_helper.h"

class CtrlTest : public CxxTest::TestSuite, public ParseTest
{
public:
  
  void test_send_ctrl( void ) 
  { 
    setup_with_print("n=Ctrl(channel:2)\n");

    // defaults
    assert_print("n.bang()\n",    "<Midi ~2:1(64), 0>\n");
    assert_print("n.bang(100)\n", "<Midi ~2:1(100), 0>\n");
    assert_print("n.bang(82)\n",  "<Midi ~2:1(82), 0>\n");
    assert_print("n.bang()\n",    "<Midi ~2:1(82), 0>\n");
  }

  void test_ctrl_slope( void ) 
  { 
    setup_with_print("n=Ctrl(slope:1000)\n"); // 1000 increments in 1 second

    // defaults
    assert_print("n.bang(100)\n", "<Midi ~1:1(74), 0>\n");
    // 10 [ms] second * 1000 = 10 steps ==> 64 + 10 = 74
    assert_print("n.move()\n",    "<Midi ~1:1(84), 0>\n");
    assert_print("n.move()\n",    "<Midi ~1:1(94), 0>\n");
    assert_print("n.move()\n",    "<Midi ~1:1(100), 0>\n");
  }

};