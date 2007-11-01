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
    Command cmd(server);
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
  TestParseCommand() : mServer(), mCmd(mServer), mOutput(std::ostringstream::out), mInput(std::istringstream::in)  
  { mCmd.listen(mInput, mOutput); }
  
  void testParseCommand( void ) 
  { assert_result("v1=Value(3)","[Value(v1) 3]\n"); }
  
  void testParseCommandWithSpaces( void ) 
  { assert_result("v1 = Value(3)\n\n","[Value(v1) 3]\n"); }
  
  void testExecuteCommand( void ) 
  { assert_result("v1=Counter(7)\nv1.bang\n","[Counter(v1) 7]\n[Counter(v1) 8]\n"); }
  
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