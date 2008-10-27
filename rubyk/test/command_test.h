#include "test_helper.h"

class testCreateCommand : public CxxTest::TestSuite
{
public:
  void test_create( void ) 
  {
    Rubyk server;
    std::istringstream  input(std::istringstream::in);   // allow input operations
    std::ostringstream output(std::ostringstream::out);  // allow output  operations
    Command cmd(input, output);
    server.listen_to_command(cmd);
    time_t start = server.mCurrentTime;
    while(server.mCurrentTime <= start + 50 && server.run()) {
      ;
    }
    TS_ASSERT_EQUALS( output.str(), std::string("Welcome to rubyk !\n\n"));
  }
};

class testParseCommand : public CxxTest::TestSuite, public ParseTest
{
public:
  
  void test_parse_command( void ) 
  { assert_result("v1=Value(1)\n","#<Value:v1 1.00>\n"); }
  
  void test_parse_zero( void ) 
  { assert_result("v1=Value(0)\n","#<Value:v1 0.00>\n"); }
  
  void test_parse_zero_dot_int( void ) 
  { assert_result("v1=Value(0.1)\n","#<Value:v1 0.10>\n"); }
  
  void test_parse_command_with_spaces( void ) 
  { assert_result("v1 = Value(2)\n\n","#<Value:v1 2.00>\n"); }
  
  void test_parse_multiline_string( void ) 
  { assert_result("t=Test(\"I love you (a lot)\nwhen you're naked...\")\n","#<Test:t \'I love you (a lot)\nwhen you\'re naked...\' 0>\n"); }
  
  void test_parse_integer( void ) 
  { assert_result("v1 = Value(2.35)\n\n","#<Value:v1 2.35>\n"); }
  
  void test_parse_double( void ) 
  { assert_result("v1 = Value(2.35)\n\n","#<Value:v1 2.35>\n"); }
  
  void test_parse_negative_integer( void ) 
  { assert_result("v1 = Value(-5)\n\n","#<Value:v1 -5.00>\n"); }
  
  void test_parse_negative_double( void ) 
  { assert_result("v1 = Value(-2.35)\n\n","#<Value:v1 -2.35>\n"); }
  
  void test_execute_command( void ) 
  { assert_result("v1=Counter(3)\nv1.bang\nv1\n","#<Counter:v1 3 (+1)>\n#<Counter:v1 4 (+1)>\n"); }
  
  void test_inspect_command( void ) 
  { assert_print("i=Counter(14)\ni\n","#<Counter:i 14 (+1)>\n"); }
  
  void test_syntax_error( void ) 
  { assert_result("i=Counter(1)\n4\ni\n","#<Counter:i 1 (+1)>\nSyntax error near '\n4\ni\n'.\n#<Counter:i 1 (+1)>\n"); }
  
  void test_execute_inlet_with_params( void ) 
  { assert_result("i=Counter(1)\ni.set_increment(5)\ni.bang\ni\n","#<Counter:i 1 (+1)>\n#<Counter:i 6 (+5)>\n"); }
  
  void test_execute_set( void ) 
  { assert_result("i=Counter(1)\ni.set(counter:4 increment:5)\ni.bang\ni\n","#<Counter:i 1 (+1)>\n#<Counter:i 9 (+5)>\n"); }
  
  void test_execute_set_value( void ) 
  { assert_result("i=Counter(1)\ni.set(5)\ni.bang\ni\n","#<Counter:i 1 (+1)>\n#<Counter:i 6 (+1)>\n"); }
  
  void test_bang_shortcut( void ) 
  { assert_result("i=Counter(1)\ni.b\ni\n","#<Counter:i 1 (+1)>\n#<Counter:i 2 (+1)>\n"); }
  
  void test_execute_class_method( void ) 
  { assert_result("Test.hello\n","Hello World!\n"); }

  void test_parse_link( void ) 
  { assert_result("n.1=>1.p\n",""); }

  void test_parse_unLink( void ) 
  { assert_print("n=Value(1)\nn // p\nn.bang()",""); }

  void test_parse_first_links( void ) 
  { clean_assert_result("n=>p\n",""); }
  
  void test_parse_bad_links( void ) 
  { assert_result("n=>\n","Syntax error near '>\n'.\n"); }
  
  void test_create_destroy( void ) 
  { assert_result("v=>p\nv=Value()\np=Print()\nv=Print()\np=Value()\n",
                  "#<Value:v Bang!>\n#<Print:p >\n#<Print:v >\n#<Value:p Bang!>\n");

  }
  
  void test_comment( void )
  { assert_result("v1=Value()    # this is a comment\n",     "#<Value:v1 Bang!>\n");
    assert_result("v2=Value()    #this is a comment\n",      "#<Value:v2 Bang!>\n");
    assert_result("v3=Value()#this is a comment\n# bobob\n", "#<Value:v3 Bang!>\n");
  }
};