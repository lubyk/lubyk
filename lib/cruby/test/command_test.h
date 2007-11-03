// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "command.h"
#include "rubyk.h"
#include <string>

class TestCreateCommand : public CxxTest::TestSuite
{
public:
  void testCreate( void ) 
  {
    Rubyk server;
    Command cmd(&server);
    std::istringstream  input(std::istringstream::in);   // allow input operations
    std::ostringstream output(std::ostringstream::out);  // allow output  operations
    cmd.listen(input, output);
    cmd.close();
    TS_ASSERT_EQUALS( output.str(), std::string("Welcome to rubyk !\n\n"));
  }
};

class TestParseCommand : public CxxTest::TestSuite
{
public:
  TestParseCommand() : mServer(), mCmd(&mServer), mOutput(std::ostringstream::out), mInput(std::istringstream::in)  
    { mCmd.set_input(mInput); mCmd.set_output(mOutput); }
  
  void testParseCommand( void ) 
  { assert_result("v1=Value(1)\n","#<Value:v1 1.00>\n"); }
  
  void testParseCommandWithSpaces( void ) 
  { assert_result("v1 = Value(2)\n\n","#<Value:v1 2.00>\n"); }
  
  void testExecuteCommand( void ) 
  { assert_result("v1=Counter(3)\nv1.bang\n","#<Counter:v1 3>\n#<Counter:v1 4>\n"); }
  
  void testInspectCommand( void ) 
  { assert_result("i=Counter(14)\ni\n","#<Counter:i 14>\n#<Counter:i 14>\n"); }
  
  void testSyntaxError( void ) 
  { assert_result("i=Counter(1)\n4\ni\n","#<Counter:i 1>\nSyntax error !\n#<Counter:i 1>\n"); }

private:
  Rubyk mServer;
  Command mCmd;
  std::ostringstream mOutput;
  std::istringstream mInput;
  
  
  void assert_result(const char * pInput, const char * pOutput)
  {
    mOutput.str(std::string("")); // clear output
    mCmd.parse(pInput);
    TS_ASSERT_EQUALS( mOutput.str(), std::string(pOutput));
  }
};