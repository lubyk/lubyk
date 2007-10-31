// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "command.h"
#include "rubyk.h"
#include <string>

class TestCommand : public CxxTest::TestSuite
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
  
  void testReadCommand( void ) 
  {
    Rubyk server;
    Command cmd(server);
    std::istringstream  input(std::istringstream::in);
    std::ostringstream output(std::ostringstream::out);  // allow output  operations
    cmd.listen(input, output);
    cmd.parse("v1=Value(3)\n");
    cmd.close();
    TS_ASSERT_EQUALS( output.str(), std::string("Welcome to rubyk !\n\n[Value(v1) 3]\n"));
  }
};