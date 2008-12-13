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
  { assert_result("v1=Number(value:1)\n","[8] \"#<Number:/v1 [3] 1.00>\"\n"); }
  
//FIX  void test_parse_zero( void ) 
//FIX  { assert_result("v1=Value(0)\n","#<Value:v1 0.00>\n"); }
//FIX  
//FIX  void test_parse_zero_dot_int( void ) 
//FIX  { assert_result("v1=Value(0.1)\n","#<Value:v1 0.10>\n"); }
//FIX  
//FIX  void test_parse_command_with_spaces( void ) 
//FIX  { assert_result("v1 = Value(2)\n\n","#<Value:v1 2.00>\n"); }
//FIX  
//FIX  void test_parse_multiline_string( void ) 
//FIX  { assert_result("t=Test(\"I love you (a lot)\nwhen you're naked...\")\n","#<Test:t \'I love you (a lot)\nwhen you\'re naked...\' 0>\n"); }
//FIX  
//FIX  void test_parse_integer( void ) 
//FIX  { assert_result("v1 = Value(2.35)\n\n","#<Value:v1 2.35>\n"); }
//FIX  
//FIX  void test_parse_real( void ) 
//FIX  { assert_result("v1 = Value(2.35)\n\n","#<Value:v1 2.35>\n"); }
//FIX  
//FIX  void test_parse_negative_integer( void ) 
//FIX  { assert_result("v1 = Value(-5)\n\n","#<Value:v1 -5.00>\n"); }
//FIX  
//FIX  void test_parse_negative_real( void ) 
//FIX  { assert_result("v1 = Value(-2.35)\n\n","#<Value:v1 -2.35>\n"); }
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
//FIX  void test_parse_link( void ) 
//FIX  { assert_result("n.1=>1.p\n",""); }
//FIX
//FIX  void test_parse_unLink( void ) 
//FIX  { assert_print("n=Value(1)\nn // p\nn.bang()",""); }
//FIX
//FIX  void test_parse_first_links( void ) 
//FIX  { assert_result("n=>p\n",""); }
//FIX  
//FIX  void test_parse_bad_links( void ) 
//FIX  { assert_result("n=>\n","Syntax error near '>\n'.\n"); }
//FIX  
//FIX  void test_create_destroy( void ) 
//FIX  { assert_result("v=>p\nv=Value()\np=Print()\nv=Print()\np=Value()\n",
//FIX                  "#<Value:v Bang!>\n#<Print:p >\n#<Print:v >\n#<Value:p Bang!>\n");
//FIX
//FIX  }
//FIX  
//FIX  void test_comment( void )
//FIX  { assert_result("v1=Value()    # this is a comment\n",     "#<Value:v1 Bang!>\n");
//FIX    assert_result("v2=Value()    #this is a comment\n",      "#<Value:v2 Bang!>\n");
//FIX    assert_result("v3=Value()#this is a comment\n# bobob\n", "#<Value:v3 Bang!>\n");
//FIX  }
};