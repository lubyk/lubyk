#include "test_helper.h"

class NumberObjTest : public NodeTestHelper
{
public:
  
  void test_connection_order( void )
  {
    create("v1",  "Number", "value:2",  "[5] \"/v1\"");
    create("v2",  "Number", "value:3",  "[11] \"/v2\"");
    create("add", "Add",   "",          "[15] \"/add\""); 
    create("v3",  "Number", "value:0",  "[24] \"/v3\"");
    
    /**    v1   v2
      *    |    |
      *   +------+
      *   | add  |
      *   +------+
      *    |
      *    v3             */
    assert_call("/v1/outlets/value/link", "/add/inlets/bang",  "/add/inlets/bang");
    assert_call("/v2/outlets/value/link", "/add/inlets/adder", "/add/inlets/adder");
    assert_call("/add/outlets/sum/link",  "/v3/inlets/bang", "/v3/inlets/bang");
    
    
    assert_inspect("/v1", "[3] 2.00");
    assert_inspect("/v2", "[9] 3.00");
    assert_inspect("/add","[19] 0.00");
    assert_inspect("/v3", "[22] 0.00");
    
    /** ---> 1. bang --> value         = 3
      *      2. send new value to add      */
    assert_call("/v2/inlets/bang", "Bang!", "Nil");
    
    
    assert_inspect("/v1", "[3] 2.00");
    assert_inspect("/v2", "[9] 3.00");
    assert_inspect("/add","[19] 3.00");
    assert_inspect("/v3", "[22] 0.00");
    
    /** ---> 1. bang --> value         = 3
      *      2. send new value to  v3  = 3  */
    assert_call("/add/inlets/bang", "Bang!", "Nil");
    
    assert_inspect("/v1", "[3] 2.00");
    assert_inspect("/v2", "[9] 3.00");
    assert_inspect("/add","[19] 3.00");
    assert_inspect("/v3", "[22] 3.00");
    
    /** ---> 1. bang --> value         = 2
      *      2. send new value to add  = 2
      *      3. add.bang --> value     = 5
      *      4. send to v3             = 5  */
    assert_call("/v1/inlets/bang", "Bang!", "Nil");
    
    assert_inspect("/v1", "[3] 2.00");
    assert_inspect("/v2", "[9] 3.00");
    assert_inspect("/add","[19] 5.00");
    assert_inspect("/v3", "[22] 5.00");
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