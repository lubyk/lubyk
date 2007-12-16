#include "test_helper.h"

/** Test Turing class.... not the 'Turing test' ;-)  */
class TuringTest : public CxxTest::TestSuite, public ParseTest
{
public:
  /*
    x = 10
    y = 4
    
    a  x:15 -> b
    a  y:53 -> b
    b  ----> b
    
    tokens
       0: x = 10
       1: y = 4

    goto    -   x   y
       a:   0   1   1
       b:   1   -   -

    send    -   x   y
       a:   /  15  53
       b:   /   -   -

  */
  
  void test_compile( void ) 
  { assert_result("n=Turing(\"x = 10\ny=4\na  x:15 -> b\na y:53 -> b\nb  ----> b\")\nn.tables\n",
                  "#<Turing:n 2, 2>\ntokens\n   0: x = 10\n   1: y = 4\n\ngoto    -   x   y\n   a:   0   1   1\n   b:   1   -   -\n\nsend    -   x   y\n   a:   /  15  53\n   b:   /   -   -\n\nmethods\n"); }
  
  /*

    a  10:15 -> b
    a  4:53 -> b
    b  ----> b
    
    tokens
      0 : 10
      1 : 4

    goto    -  10   4
       a:   0   1   1
       b:   1   -   -

    send    -  10   4
       a:   /  15  53
       b:   /   -   -
    
  */
  void test_compile_val_tokens( void )
  { assert_result("n=Turing(\"a  10:15 -> b\na 4:53 -> b\nb  ----> b\")\nn.tables\n",
                  "#<Turing:n 2, 2>\ntokens\n  0 : 10\n  1 : 4\n\ngoto    -  10   4\n   a:   0   1   1\n   b:   1   -   -\n\nsend    -  10   4\n   a:   /  15  53\n   b:   /   -   -\n\nmethods\n"); }
 /*
    x = 10
    y = 4 #this is a comment
    
    a  x:15 -> b
    =begin
    multi line
    comment
    
    =end
    a  y:53 -> b
    b  ----> b
    
    tokens
       0: x = 10
       1: y = 4

    goto    -   x   y
       a:   0   1   1
       b:   1   -   -

    send    -   x   y
       a:   /  15  53
       b:   /   -   -

  */
  
  void test_comment( void ) 
  { assert_result("n=Turing(\"x = 10\ny=4 # this is a comment\n\na  x:15 -> b\n\n\n=begin\nmulti line\ncomment\n\n=end\na y:53 -> b\nb  ----> b\")\nn.tables\n",
                  "#<Turing:n 2, 2>\ntokens\n   0: x = 10\n   1: y = 4\n\ngoto    -   x   y\n   a:   0   1   1\n   b:   1   -   -\n\nsend    -   x   y\n   a:   /  15  53\n   b:   /   -   -\n\nmethods\n"); }
  /*    
    x = 10
    y = 20
    
    a  x:15 -> b x:14 -> c y:13 -> a
  */
  
  void test_sequence( void )
  {
    setup_with_print("n=Turing(\"x = 10\ny = 20\na  x:15 -> b x:14 -> c y:13 -> a\")\n");
    assert_print("n.bang(10)\n", "15\n");
    assert_print("n.bang(20)\n", "");
    assert_print("n.bang(10)\n", "14\n");
    assert_print("n.bang(10)\n", "");
    assert_print("n.bang(20)\n", "13\n");
  }
  
 /*
    x = 10
    y = 4 #this is a comment
    
    a  x:bip(1) -> b
    a  y:send(1,{1,2,3}) -> b
    b  ----> b
    
    =begin lua
    function bip(v)
      print("lua",v)
    end
    =end
    
    tokens
       0: x = 10
       1: y = 4

    goto    -   x   y
       a:   0   1   1
       b:   1   -   -

    send    -   x   y
       a:   / (1) (2)
       b:   /   -   -
    
    lua calls
       1:  bip(1)
       2:  send(1,{1,2,3})

  */  
  void test_lua_calls( void )
  {
    assert_result("n=Turing(\"x = 10\ny=4\n\na  x:15 -> b\n\n\n=begin\nmulti line\ncomment\n\n=end\na y:53 -> b\nb  ----> b\")\nn.tables\n",
                    "#<Turing:n 2, 2>\ntokens\n   0: x = 10\n   1: y = 4\n\ngoto    -   x   y\n   a:   0   1   1\n   b:   1   -   -\n\nsend    -   x   y\n   a:   /  15  53\n   b:   /   -   -\n\nmethods\n");
  }
};