#include "test_helper.h"


class TuringTest : public CxxTest::TestSuite, public ParseTest
{
public:
  /*
    a  x  -> b { X }
    a  'y'-> b { Y }
    b  ----> b {   }
    
    goto ======
       . x y
    a  0 1 1
    b  1 0 0

    send ======
       . x y
    a  0 X Y
    b  / 0 0
  */
  
  void test_compile( void ) 
  { assert_result("n=Turing(\"a  x  -> b { X }\na  'y'-> b { Y }\nb  ----> b {   }\")\nn.tables\n",
                  "#<Turing:n -->\ngoto\n 0 1 1\n 1 0 0\nsend\n 0 88 89\n / 0 0\n"); }
  void test_compile_numbers( void ) 
  { assert_result("n=Turing(\"a  x  -> b { 23 }\na  'y'-> b { 22 }\nb  ----> b {   }\")\nn.tables\n",
                  "#<Turing:n -->\ngoto\n 0 1 1\n 1 0 0\nsend\n 0 23 22\n / 0 0\n"); }
  /*
    a  x  -> b { X }
    a  'y'-> b { Y }
    b  ----> b {   }
    
    goto ======
       . x y
    a  0 1 1
    b  0 1 0

    send ======
       . x y
    a  0 X Y
    b  A / 0
  */
  
  void test_comment( void ) 
  { assert_result("n=Turing(\"a  x  -> b { X } # this is a comment\n=begin\nmulti\nline\ncomment\n=end\na  'y'-> b { Y }\nb  ----> b {   }\")\nn.tables\n",
                  "#<Turing:n -->\ngoto\n 0 1 1\n 1 0 0\nsend\n 0 88 89\n / 0 0\n"); }
  /*
    a  x  -> b { X }
    a  'y'-> b { Y }
    b  ----> a { A }
    b  -x--> b {   }
    
    'x' = 120
    'y' = 121

    'X' = 88
    'Y' = 89

    x : 1
    y : 2

    a : 0
    b : 1

    goto ======
       . x y
    a  0 1 1
    b  0 1 0

    send ======
       . x y
    a  0 X Y
    b  A / 0
    
    test sequence is:
    x => b (X)
    ? => a (A)
    y => b (Y)
  */
  
  void test_sequence( void )
  {
    setup_with_print("n=Turing(\"a  x  -> b { X } # this is a comment\n=begin\nmulti\nline\ncomment\n=end\na  'y'-> b { Y }\nb  ----> a { A }\nb  -x--> b {   }\")\n");
    assert_bang("n.input(120)\n",  "88\n");
    assert_bang("n.input(5644)\n", "65\n");
    assert_bang("n.input(121)\n",  "89\n");
  }
  
};