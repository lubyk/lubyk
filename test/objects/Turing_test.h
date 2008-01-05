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
       -:
       a:   0   1   1
       b:   1   -   -

    send    -   x   y
       -:
       a:   /  15  53
       b:   /   -   -

  */
  
  void test_compile( void ) 
  { assert_result("n=Turing(\"x = 10\ny=4\na  x:15 -> b\na y:53 -> b\nb  ----> b\")\nn.tables\n",
                  "#<Turing:n 2, 2>\ntokens\n   1: x = 10\n   2: y = 4\n\ngoto        -     x     y\n     -:                  \n     a:     1     2     2\n     b:     2     -     -\n\nsend        -     x     y\n     -:                  \n     a:     /    15    53\n     b:     /     -     -\n\nmethods\n"); }
  
  /*

    a  10:15 -> b
    a  4:53 -> b
    b  ----> b
    
    tokens
      0 : 10
      1 : 4

    goto    -  10   4
       -:
       a:   0   1   1
       b:   1   -   -

    send    -  10   4
       -:
       a:   /  15  53
       b:   /   -   -
    
  */
  void test_compile_val_tokens( void )
  { assert_result("n=Turing(\"a  10:15 -> b\na 4:53 -> b\nb  ----> b\")\nn.tables\n",
                  "#<Turing:n 2, 2>\ntokens\n   1: 10\n   2: 4\n\ngoto        -    10     4\n     -:                  \n     a:     1     2     2\n     b:     2     -     -\n\nsend        -    10     4\n     -:                  \n     a:     /    15    53\n     b:     /     -     -\n\nmethods\n"); }
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
       -:
       a:   0   1   1
       b:   1   -   -

    send    -   x   y
       -:
       a:   /  15  53
       b:   /   -   -

  */
  
  void test_comment( void ) 
  { assert_result("n=Turing(\"x = 10\ny=4 # this is a comment\n\na  x:15 -> b\n\n\n=begin\nmulti line\ncomment\n\n=end\na y:53 -> b\nb  ----> b\")\nn.tables\n",
                  "#<Turing:n 2, 2>\ntokens\n   1: x = 10\n   2: y = 4\n\ngoto        -     x     y\n     -:                  \n     a:     1     2     2\n     b:     2     -     -\n\nsend        -     x     y\n     -:                  \n     a:     /    15    53\n     b:     /     -     -\n\nmethods\n"); }
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
    y = 4
    
    a  x:bip(1) -> b
    a  y:send(1,{1,2,3}) -> b
    b  ----> a
    
    =begin lua
    function bip(v)
      print("lua",v)
    end
    =end
    
    tokens
       0: x = 10
       1: y = 4

    goto    -   x   y
       -:
       a:   0   1   1
       b:   1   -   -

    send    -   x   y
       -:
       a:   / (1) (2)
       b:   /   -   -
    
    lua calls
       1:  bip(1)
       2:  send(1,{1,2,3})

  */  
  void test_lua_calls( void )
  {
    assert_result("n=Turing(\"x = 10\ny=4\n\na  x:bip(1) -> b\na y:send(1,{1,2,3}) -> b\nb  ----> b\n\n=begin lua\nfunction bip(v)\nprint('lua',v)\nend\n=end\n\")\nn.tables\n",
                  "#<Turing:n 2, 2>\ntokens\n   1: x = 10\n   2: y = 4\n\ngoto        -     x     y\n     -:                  \n     a:     1     2     2\n     b:     2     -     -\n\nsend        -     x     y\n     -:                  \n     a:     / (  1) (  2)\n     b:     /     -     -\n\nmethods\n   1: bip(1)\n   2: send(1,{1,2,3})\n");
  }
  
  void test_lua_run( void )
  {
    setup_with_print("n=Turing(\"x = 10\ny=4\n\na  x:bip(1) -> b\na y:send(1,{1,2,3}) -> b\nb  ----> a\n\n=begin lua\nfunction bip(v)\nsend(1,1)\nend\n=end\n\")\nn.tables\n");
    assert_print("n.b(10)\n", "1.00\n"); // a  x:bip(1) -> b
    assert_print("n.b(4)\n", ""); // b --> a
    assert_print("n.b(4)\n", "<Matrix [  1.00  2.00  3.00 ], 1x3>\n"); // a y:send(1,{1,2,3}) -> b
  }
  
  void test_lua_raw( void )
  {
    setup_with_print("n=Turing(\"x = 10\ny=4\n\na  x:{n = n or 0; n = n + 1; send(1,n)} -> b\na y:send(1,{1,2,3}) -> b\nb  ----> a\n\n=begin lua\nfunction bip(v)\nsend(1,1)\nend\n=end\n\")\n");
    assert_print("n.b(10)\n", "1.00\n"); // a  x:bip(1) -> b
    assert_print("n.b(4)\n", ""); // b --> a
    assert_print("n.b(10)\n", "2.00\n"); // a  x:bip(1) -> b
    assert_print("n.b(4)\n", ""); // b --> a
    assert_print("n.b(10)\n", "3.00\n"); // a  x:bip(1) -> b
  }
  
  void test_lua_bang( void )
  {
    setup_with_print("n=Turing(\"x = 10\ny=4\n\na  x:bip(1) -> b\na y:send(1,{1,2,3}) -> b\nb  ----> a\n\n=begin lua\nfunction bip(v)\nsend(1,1)\nend\nfunction bang()\nif(in1 == 100) then\nreturn 10\nelse\nreturn in1\nend\nend\n=end\n\")\n");
    assert_print("n.b(100)\n", "1.00\n"); // a  x:bip(1) -> b
    assert_print("n.b(4)\n", ""); // b --> a
    assert_print("n.b(4)\n", "<Matrix [  1.00  2.00  3.00 ], 1x3>\n"); // a y:send(1,{1,2,3}) -> b
  }
  
  /*
    x = 10
    y = 4
    
    . y:33 -> a  # default action for token 'x'
    a  x:15 -> b
    a  y:53 -> b
    b  ----> b
    
    tokens
       0: x = 10
       1: y = 4

    goto    -   x   y
       -:       0
       a:   0   1   1
       b:   1   |   -

    send    -   x   y
       -:      33
       a:   /  15  53
       b:   /   |   -

  */
  
  void test_default_token_action( void ) 
  { assert_result("n=Turing(\"x = 10\ny=4\n. y:33 -> a\na  x:15 -> b\na y:53 -> b\nb  ----> b\")\nn.tables\n",
                  "#<Turing:n 2, 2>\ntokens\n   1: y = 4\n   2: x = 10\n\ngoto        -     y     x\n     -:           1      \n     a:     1     2     2\n     b:     2     |     -\n\nsend        -     y     x\n     -:          33      \n     a:     /    53    15\n     b:     /     |     -\n\nmethods\n"); }
  
  
  void test_default_token_sequence( void )
  {
    setup_with_print("n=Turing(\"x = 10\ny=4\n. y:33 -> a\na  x:15 -> b\na y:53 -> b\nb  ----> b\")\nn.tables\n");
    assert_print("n.b(13)\n", ""); // a  ---> a
    assert_print("n.b(4)\n", "53\n");  // a y:53 -> b
    assert_print("n.b(10)\n", ""); // b ---> b
    assert_print("n.b(4)\n", "33\n"); // . y:33 --> a
    assert_print("n.b(10)\n", "15\n");  // a x:15 -> b
  }
  
  void test_default_token_sequence_no_send( void )
  {
    setup_with_print("n=Turing(\"x = 10\ny=4\n. y -> a\na  x:15 -> b\na y:53 -> b\nb  ----> b\")\nn.tables\n");
    assert_print("n.b(13)\n", ""); // a  ---> a
    assert_print("n.b(4)\n", "53\n");  // a y:53 -> b
    assert_print("n.b(10)\n", ""); // b ---> b
    assert_print("n.b(4)\n", ""); // . y --> a
    assert_print("n.b(10)\n", "15\n");  // a x:15 -> b
  }
  
  void test_default_send( void )
  {
    setup_with_print("n=Turing(\"x = 10\ny=4\na :33->b\na  x:15 -> b\na y:53 -> b\nb---->a\")\nn.tables\n");
    assert_print("n.b(13)\n", "33\n"); // a  :33 --> b
    assert_print("n.b(13)\n", "");     // b  --> a
    assert_print("n.b(4)\n", "53\n");  // a y:53 -> b
    assert_print("n.b(10)\n", "");     // b ---> a
    assert_print("n.b(10)\n", "15\n"); // a x:15 --> b
  }
  
  void test_inputs( void ) 
  { 
    setup_with_print("n=Turing(\"p -> p\n=begin lua\nfunction bang()\nsend(1,{in1 or 0,in2 or 0,in3 or 0,in4 or 0,in5 or 0,in6 or 0,in7 or 0,in8 or 0,in9 or 0,in10 or 0})\nend\n=end\")\n");
    
    assert_print("n.b(11)\n",              "<Matrix [  11.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in2(22)\nn.b(11)\n",   "<Matrix [  11.00  22.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in3(33)\nn.b(11)\n",   "<Matrix [  11.00  22.00  33.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in4(44)\nn.b(11)\n",   "<Matrix [  11.00  22.00  33.00  44.00  0.00  0.00  0.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in5(55)\nn.b(11)\n",   "<Matrix [  11.00  22.00  33.00  44.00  55.00  0.00  0.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in6(66)\nn.b(11)\n",   "<Matrix [  11.00  22.00  33.00  44.00  55.00  66.00  0.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in7(77)\nn.b(11)\n",   "<Matrix [  11.00  22.00  33.00  44.00  55.00  66.00  77.00  0.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in8(88)\nn.b(11)\n",   "<Matrix [  11.00  22.00  33.00  44.00  55.00  66.00  77.00  88.00  0.00  0.00 ], 1x10>\n");
    assert_print("n.in9(9.9)\nn.b(11)\n",  "<Matrix [  11.00  22.00  33.00  44.00  55.00  66.00  77.00  88.00  9.90  0.00 ], 1x10>\n");
    assert_print("n.in10(1.1)\nn.b(11)\n", "<Matrix [  11.00  22.00  33.00  44.00  55.00  66.00  77.00  88.00  9.90  1.10 ], 1x10>\n");
  }
  
  void test_outputs( void ) 
  { 
    setup_with_print("n=Turing(\"p -> p\n=begin lua\nfunction bang()\nsend(6,in1)\nend\n=end\")\nn.6 => v\nv = Value()\nn // p\nv => p\n");
    
    assert_print("n.b(123.34)\n",              "123.34\n");
  }
};