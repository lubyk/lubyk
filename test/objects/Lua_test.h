#include "test_helper.h"

class LuaTest : public CxxTest::TestSuite, public ParseTest
{
public:

  void test_return( void ) 
  { assert_print("n=Lua(\"function bang()\nreturn 77\nend\")\nn=>p\np=Print()\nn.bang\n","77.00\n"); }
  
  void test_send( void ) 
  { assert_print("n=Lua(\"function bang()\nsend(1,{1,2,3})\nend\")\np=Print()\nn=>p\nn.bang\n","<Matrix [  1.00  2.00  3.00 ], 3>\n"); }

  void test_create_load( void ) 
  { assert_print("n=Lua(load:\"test/fixtures/test.lua\")\nn.script\n", "n: script loaded.\nfunction bang()\n\treturn in1+12\nend\n\n"); }

  void test_load( void ) 
  { assert_print("n=Lua()\nn.load(\"test/fixtures/test.lua\")\nn.script\n", "n: script loaded.\nfunction bang()\n\treturn in1+12\nend\n\n"); }

};