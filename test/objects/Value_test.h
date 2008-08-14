#include "test_helper.h"

class ValueTest : public CxxTest::TestSuite, public ParseTest
{
public:
  
  void test_empty( void ) 
  { clean_assert_print("n=Value()\nn=>p\np=Print()\nn.bang\n","Bang!\n"); }
  
  void test_double( void ) 
  { clean_assert_print("n=Value(1.23)\nn=>p\np=Print()\nn.bang\n","1.23\n"); }
  
  void test_integer( void ) 
  { clean_assert_print("n=Value(15)\nn=>p\np=Print()\nn.bang\n","15.00\n"); }
};