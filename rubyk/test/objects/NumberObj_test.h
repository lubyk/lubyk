#include "test_helper.h"

class NumberObjTest : public NodeTestHelper
{
public:
  
  void test_connection_order( void )
  {
    create("/v1",  "Number", "value:2",  "[5] \"/v1\"");
    create("/v2",  "Number", "value:3",  "[11] \"/v2\"");
    create("/add", "Add",   "",          "[15] \"/add\""); 
    create("/v3",  "Number", "value:0",  "[24] \"/v3\"");
    
    /**    v1   v2
      *    |    |
      *   +------+
      *   | add  |
      *   +------+
      *    |
      *    v3             */
    assert_call("/v1/out/value/link", "/add/in/bang",  "/add/in/bang");
    assert_call("/v2/out/value/link", "/add/in/adder", "/add/in/adder");
    assert_call("/add/out/sum/link",  "/v3/in/bang", "/v3/in/bang");
    
    
    assert_call("/v1", "[1] 2.00");
    assert_call("/v2", "[7] 3.00");
    assert_call("/add","[19] 0.00");
    assert_call("/v3", "[20] 0.00");
    
    /** ---> 1. bang --> value         = 3
      *      2. send new value to add      */
    assert_call("/v2/in/bang", "Bang!", "Nil");
    
    
    assert_inspect("/v1", "[1] 2.00");
    assert_inspect("/v2", "[7] 3.00");
    assert_inspect("/add","[19] 3.00");
    assert_inspect("/v3", "[20] 0.00");
    
    /** ---> 1. bang --> value         = 3
      *      2. send new value to  v3  = 3  */
    assert_call("/add/in/bang", "Bang!", "Nil");
    
    assert_inspect("/v1", "[1] 2.00");
    assert_inspect("/v2", "[7] 3.00");
    assert_inspect("/add","[19] 3.00");
    assert_inspect("/v3", "[20] 3.00");
    
    /** ---> 1. bang --> value         = 2
      *      2. send new value to add  = 2
      *      3. add.bang --> value     = 5
      *      4. send to v3             = 5  */
    assert_call("/v1/in/bang", "Bang!", "Nil");
    
    assert_inspect("/v1", "[1] 2.00");
    assert_inspect("/v2", "[7] 3.00");
    assert_inspect("/add","[19] 5.00");
    assert_inspect("/v3", "[20] 5.00");
  }
//  
//  void test_empty( void ) 
//  { parse("n=Value()\nn=>p\n");
//    assert_print("n.b\n","Bang!\n"); }
//  
//  void test_real( void ) 
//  { parse("n=Value(1.23)\nn=>p\n");
//    assert_print("n.b\n","1.23\n"); }
//  
//  void test_integer( void ) 
//  { parse("n=Value(15)\nn=>p\n");
//    assert_print("n.b\n","15.00\n"); }
//  
//  void test_set_value( void ) 
//  { 
//    parse("n=Value(3)\nn2=Value(15)\nn => 2.n2\nn2 => p\n");
//    assert_print("n2.b\n","15.00\n");
//    
//    assert_print("n.b\n","");
//    assert_print("n2.b\n","3.00\n");
//  }
};