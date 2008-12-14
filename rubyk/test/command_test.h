#include "test_helper.h"

//FIX class CreateCommandTest : public CxxTest::TestSuite
//FIX {
//FIX public:
//FIX   void test_create( void ) 
//FIX   {
//FIX     Root server;
//FIX     std::istringstream  input(std::istringstream::in);   // allow input operations
//FIX     std::ostringstream output(std::ostringstream::out);  // allow output  operations
//FIX     Command cmd(input, output);
//FIX     server.listen_to_command(cmd);
//FIX     time_t start = server.mCurrentTime;
//FIX     while(server.mCurrentTime <= start + 50 && server.run()) {
//FIX       ;
//FIX     }
//FIX     TS_ASSERT_EQUALS( output.str(), std::string("Welcome to rubyk !\n\n"));
//FIX   }
//FIX };

class ParseCommandTest : public ParseHelper
{
public:
  
  void test_parse_command( void ) 
  { assert_result("v1=Number(value:1)\n","<Number:/v1 1.00>\n"); }
  
//FIX  void test_parse_zero( void ) 
//FIX  { assert_result("v1=Number(0)\n","#<Number:v1 0.00>\n"); }
//FIX  
//FIX  void test_parse_zero_dot_int( void ) 
//FIX  { assert_result("v1=Number(0.1)\n","#<Number:v1 0.10>\n"); }
//FIX  
//FIX  void test_parse_command_with_spaces( void ) 
//FIX  { assert_result("v1 = Number(2)\n\n","#<Number:v1 2.00>\n"); }
//FIX  
//FIX  void test_parse_multiline_string( void ) 
//FIX  { assert_result("t=Test(\"I love you (a lot)\nwhen you're naked...\")\n","#<Test:t \'I love you (a lot)\nwhen you\'re naked...\' 0>\n"); }
//FIX  
//FIX  void test_parse_integer( void ) 
//FIX  { assert_result("v1 = Number(2.35)\n\n","#<Number:v1 2.35>\n"); }
//FIX  
//FIX  void test_parse_real( void ) 
//FIX  { assert_result("v1 = Number(2.35)\n\n","#<Number:v1 2.35>\n"); }
//FIX  
//FIX  void test_parse_negative_integer( void ) 
//FIX  { assert_result("v1 = Number(-5)\n\n","#<Number:v1 -5.00>\n"); }
//FIX  
//FIX  void test_parse_negative_real( void ) 
//FIX  { assert_result("v1 = Number(-2.35)\n\n","#<Number:v1 -2.35>\n"); }
//FIX  
//FIX  void test_execute_command( void ) 
//FIX  { assert_result("v1=Counter(3)\nv1.bang\nv1\n","#<Counter:v1 3 (+1)>\n#<Counter:v1 4 (+1)>\n"); }
//FIX  
//FIX  void test_inspect_command( void ) 
//FIX  { assert_print("i=Counter(14)\ni\n","#<Counter:i 14 (+1)>\n"); }
//FIX  
//FIX  void test_syntax_error( void ) 
//FIX  { assert_result("i=Counter(1)\n4\ni\n","#<Counter:i 1 (+1)>\nSyntax error near '\n4\ni\n'.\n#<Counter:i 1 (+1)>\n"); }
//FIX  
//FIX  void test_execute_inlet_with_params( void ) 
//FIX  { assert_result("i=Counter(1)\ni.set_increment(5)\ni.bang\ni\n","#<Counter:i 1 (+1)>\n#<Counter:i 6 (+5)>\n"); }
//FIX  
//FIX  void test_execute_set( void ) 
//FIX  { assert_result("i=Counter(1)\ni.set(counter:4 increment:5)\ni.bang\ni\n","#<Counter:i 1 (+1)>\n#<Counter:i 9 (+5)>\n"); }
//FIX  
//FIX  void test_execute_set_value( void ) 
//FIX  { assert_result("i=Counter(1)\ni.set(5)\ni.bang\ni\n","#<Counter:i 1 (+1)>\n#<Counter:i 6 (+1)>\n"); }
//FIX  
//FIX  void test_bang_shortcut( void ) 
//FIX  { assert_result("i=Counter(1)\ni.b\ni\n","#<Counter:i 1 (+1)>\n#<Counter:i 2 (+1)>\n"); }
//FIX  
//FIX  void test_execute_class_method( void ) 
//FIX  { assert_result("Test.hello\n","Hello World!\n"); }
//FIX
  void test_parse_link( void ) 
  { 
    assert_result("n.out=>in.p\n",""); // just make sure there is no parse error
  }

  void test_parse_unLink( void ) 
  { 
    setup_with_print("n=Number(1)\n");
    assert_print("n // p\nn.bang()\n","");
  }
  
  void test_print( void )
  {
    setup_with_print("n=Number(7)\n");
    assert_print("n.bang()\n","7.00\n");
  }

//FIX  void test_parse_first_links( void ) 
//FIX  { assert_result("n=>p\n",""); }
//FIX  
//FIX  void test_parse_bad_links( void ) 
//FIX  { assert_result("n=>\n","Syntax error near '>\n'.\n"); }
//FIX  
//FIX  void test_create_destroy( void ) 
//FIX  { assert_result("v=>p\nv=Number()\np=Print()\nv=Print()\np=Number()\n",
//FIX                  "#<Number:v Bang!>\n#<Print:p >\n#<Print:v >\n#<Number:p Bang!>\n");
//FIX
//FIX  }
//FIX  
//FIX  void test_comment( void )
//FIX  { assert_result("v1=Number()    # this is a comment\n",     "#<Number:v1 Bang!>\n");
//FIX    assert_result("v2=Number()    # this is a comment\n",      "#<Number:v2 Bang!>\n");
//FIX    assert_result("v3=Number()#this is a comment\n# bobob\n", "#<Number:v3 Bang!>\n");
//FIX  }
};