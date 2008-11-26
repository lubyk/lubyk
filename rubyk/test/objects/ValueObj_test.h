#include "test_helper.h"

class ValueObjTest : public ParseTest
{
public:
  
  void test_empty( void ) 
  { parse("n=Value()\nn=>p\n");
    assert_print("n.b\n","Bang!\n"); }
  
  void test_real( void ) 
  { parse("n=Value(1.23)\nn=>p\n");
    assert_print("n.b\n","1.23\n"); }
  
  void test_integer( void ) 
  { parse("n=Value(15)\nn=>p\n");
    assert_print("n.b\n","15.00\n"); }
  
  void test_set_value( void ) 
  { 
    parse("n=Value(3)\nn2=Value(15)\nn => 2.n2\nn2 => p\n");
    assert_print("n2.b\n","15.00\n");
    
    assert_print("n.b\n","");
    assert_print("n2.b\n","3.00\n");
  }
};