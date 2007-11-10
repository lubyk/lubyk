#include "test_helper.h"


class TuringTest : public CxxTest::TestSuite, public ParseTest
{
public:

  void test_compile( void ) 
  { assert_result("n=Turing(\"a  x  -> b { X }\na  'y'-> b { Y }\nb  ----> b {   }\")\nn.tables\n",
                  "#<Turing:n -->\ngoto\n 0 1 1\n 1 0 0\nsend\n 0 88 89\n 0 0 0\n"); }

  void test_comment( void ) 
  { assert_result("n=Turing(\"a  x  -> b { X } # this is a comment\n=begin\nmulti\nline\ncomment\n=end\na  'y'-> b { Y }\nb  ----> b {   }\")\nn.tables\n",
                  "#<Turing:n -->\ngoto\n 0 1 1\n 1 0 0\nsend\n 0 88 89\n 0 0 0\n"); }
  
  /*
  a  x  -> b { X }
  a  'y'-> b { Y }
  b  ----> b {   }
  
  x : 1
  y : 2
  
  a : 0
  b : 1
  
  goto ======
     . x y
  a  0 1 1
  b  1 0 0
  
  send ======
     . x y
  a  0 X Y
  b  0 0 0
  */
  
};