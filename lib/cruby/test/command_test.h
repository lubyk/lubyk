// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "command.h"
#include <string>

class TestCommand : public CxxTest::TestSuite
{
public:
  void testCreate( void ) 
  {
    Command cmd;
    std::istringstream  input(std::istringstream::in); // allow output operations
    std::ostringstream output(std::ostringstream::out);  // allow input  operations
    cmd.listen(input, output);
    TS_ASSERT_EQUALS( output.str(), std::string("Start\n"));
  }
};