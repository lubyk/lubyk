// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "command.h"
#include "rubyk.h"
#include "test_helper.h"

class TestCreateCommand : public CxxTest::TestSuite
{
public:
  void testCreate( void ) 
  {
    Rubyk server;
    std::istringstream  input(std::istringstream::in);   // allow input operations
    std::ostringstream output(std::ostringstream::out);  // allow output  operations
    Command cmd(input, output);
    server.listen_to_command(cmd);
    server.run();
    TS_ASSERT_EQUALS( output.str(), std::string("Welcome to rubyk !\n\n"));
  }
};

class TestParseCommand : public CxxTest::TestSuite, public ParseTest
{
public:
  
  void testParseCommand( void ) 
  { assert_result("v1=Value(1)\n","#<Value:v1 1.00>\n"); }
  
  void testParseZero( void ) 
  { assert_result("v1=Value(0)\n","#<Value:v1 0.00>\n"); }
  
  void testParseCommandWithSpaces( void ) 
  { assert_result("v1 = Value(2)\n\n","#<Value:v1 2.00>\n"); }
  
  void testParseInteger( void ) 
  { assert_result("v1 = Value(2.35)\n\n","#<Value:v1 2.35>\n"); }
  
  void testParseFloat( void ) 
  { assert_result("v1 = Value(2.35)\n\n","#<Value:v1 2.35>\n"); }
  
  void testParseNegativeInteger( void ) 
  { assert_result("v1 = Value(-5)\n\n","#<Value:v1 -5.00>\n"); }
  
  void testParseNegativeFloat( void ) 
  { assert_result("v1 = Value(-2.35)\n\n","#<Value:v1 -2.35>\n"); }
  
  void testExecuteCommand( void ) 
  { assert_result("v1=Counter(3)\nv1.bang\n","#<Counter:v1 3>\n#<Counter:v1 4>\n"); }
  
  void testInspectCommand( void ) 
  { assert_result("i=Counter(14)\ni\n","#<Counter:i 14>\n#<Counter:i 14>\n"); }
  
  void testSyntaxError( void ) 
  { assert_result("i=Counter(1)\n4\ni\n","#<Counter:i 1>\nSyntax error !\n#<Counter:i 1>\n"); }
  
  void testExecuteMethodWithParams( void ) 
  { assert_result("i=Counter(1)\ni.set_increment(5)\ni.bang\n","#<Counter:i 1>\n#<Counter:i 6>\n"); }
  
  void testExecuteClassMethod( void ) 
  { assert_result("Test.hello\n","Hello World!\n"); }

  void testParseLink( void ) 
  { assert_result("n.1=>1.p\n",""); }

  void testParseFirstLinks( void ) 
  { assert_result("n=>p\n",""); }
  
  void testParseBadLinks( void ) 
  { assert_result("n=>\n","Syntax error !\n"); }
};