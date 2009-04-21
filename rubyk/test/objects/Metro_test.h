#include "test_helper.h"


class MetroTest : public ParseHelper
{
public:
  
  void test_bang_bang( void ) { 
    setup_with_print("n=Metro(1000)\n");
    assert_run("print: Bang!\nprint: Bang!\nprint: Bang!\n", (60 * 3) + 10);  // 0 ... 20 ... 60 [bang] ... 120 [bang] ... 180 [bang] ... 190.
  }
  
  void test_set_tempo( void ) { 
    setup("n=Metro()\n");
    assert_result("# 10\n", "n(10)\n");
    assert_result("# {\"tempo\":20}\n", "n(tempo:20)\n");
    assert_result("# 30\n", "n/tempo(30)\n");
    assert_result("# <Metro:/n tempo:40 run:1>\n", "n/in/tempo(40)\n");
  }
};