// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "class.h"
#include "test_helper.h"


class TestTest : public CxxTest::TestSuite, public NodeTester
{    
public:
  TestTest() : NodeTester("Test") {}
  
  void testCreate( void )
  {  
    create("counter: 5 name:\"foo\"");
    assert_spy("foo: 5");
  }
  
  void testInitMessage( void )
  {
    create("test:\"is output ok?\"");
    TS_ASSERT_EQUALS( mOutput.str(), std::string("Output set\n"));
  }
  
  void testInspect( void )
  { 
    create("counter: 5 name:\"foo\"");
    mNode->set_name(std::string("d"));
    assert_inspect("#<Test:d foo: 5>");
  }
  
  void testBang( void )
  {
    create("");
    
    assert_spy("no-name: 0");
    mNode->bang(gNilSignal);
    assert_spy("no-name: 1");
  }
  
  void testExecuteMethod( void )
  {
    create("counter:5");
    mNode->set_name("d");
    assert_method_result("help","","Don't hit me!\n");
  }
  
  void testConnection( void )
  {
    Node   * d1   = Class::create(NULL, "Test", "name:first  counter:3", &mOutput);
    Outlet * out1 = d1->outlet(1); // oulets and inlets are indexed starting with '1', not '0'
    Node   * d2   = Class::create(NULL, "Test", "name:second counter:0", &mOutput);
    Inlet  * in2  = d2->inlet(1);
    
    out1->connect(in2);
    
    TS_ASSERT_EQUALS( std::string("first: 3" ), std::string(d1->get_spy()) );
    TS_ASSERT_EQUALS( std::string("second: 0"), std::string(d2->get_spy()) );
    
    
    d1->bang(gNilSignal); /** ---> 1. bang --> increment d1  = 4
                  *      2. send new value to  d2
                  *      3. set d2                 = 4
                  *      4. bang --> increment d2  = 5  */
    TS_ASSERT_EQUALS( std::string("first: 4" ), std::string(d1->get_spy()) );
    TS_ASSERT_EQUALS( std::string("second: 5"), std::string(d2->get_spy()) );
  }
  
  void BadConnection( void )
  {
    
    Node   * d1   = Class::create(NULL, "Test", "name:first  counter:3", &mOutput);
    Outlet * out1 = d1->outlet(2); // oulets and inlets are indexed starting with '1', not '0'
    Node   * d2   = Class::create(NULL, "Test", "name:second counter:0", &mOutput);
    Inlet  * in2  = d2->inlet(2);
    
    TS_ASSERT_EQUALS( out1, (void*)NULL);
    TS_ASSERT_EQUALS( in2,  (void*)NULL);
    
    out1 = d1->outlet(1);
    TS_ASSERT(!out1->connect(in2)); // should fail
    
    TS_ASSERT_EQUALS( std::string(d1->get_spy()), std::string("first: 3" ));
    TS_ASSERT_EQUALS( std::string(d2->get_spy()), std::string("second: 0"));
    
    
    d1->bang(gNilSignal); /** ---> 1. bang --> increment d1  = 4
                  *      2. send new value to  d2
                  *      3. set d2                 = 4
                  *      4. bang --> increment d2  = 5  */
    TS_ASSERT_EQUALS( std::string(d1->get_spy()), std::string("first: 4" ));
    TS_ASSERT_EQUALS( std::string(d2->get_spy()), std::string("second: 0"));
  }
  
  void testConnectionOrder( void )
  {
    Node   * v1   = Class::create(NULL, "Value", "value:2", &mOutput);
    Node   * v2   = Class::create(NULL, "Value", "value:3", &mOutput);
    Node   * add  = Class::create(NULL, "Add", "", &mOutput);
    Node   * v3   = Class::create(NULL, "Value", "value:0", &mOutput);
    
    /**    v1   v2
      *    |    |
      *   +------+
      *   | add  |
      *   +------+
      *    |
      *    v3             */
    v1->outlet(1)->connect(add->inlet(1));
    v2->outlet(1)->connect(add->inlet(2));
    
    add->outlet(1)->connect(v3->inlet(1));
    
    TS_ASSERT_EQUALS( std::string(v1->get_spy()) , std::string("2.00"));
    TS_ASSERT_EQUALS( std::string(v2->get_spy()) , std::string("3.00"));
    TS_ASSERT_EQUALS( std::string(add->get_spy()), std::string("0.00"));
    TS_ASSERT_EQUALS( std::string(v3->get_spy()) , std::string("0.00"));
    
    v2->bang(gNilSignal); /** ---> 1. bang --> value         = 3
                  *      2. send new value to  add      */
    
    TS_ASSERT_EQUALS( std::string(v1->get_spy()) , std::string("2.00"));
    TS_ASSERT_EQUALS( std::string(v2->get_spy()) , std::string("3.00"));
    TS_ASSERT_EQUALS( std::string(add->get_spy()), std::string("3.00"));
    TS_ASSERT_EQUALS( std::string(v3->get_spy()) , std::string("0.00"));
    
    add->bang(gNilSignal); /** ---> 1. bang --> value         = 3
                   *      2. send new value to  v3  = 3  */
    
    TS_ASSERT_EQUALS( std::string(v1->get_spy()) , std::string("2.00"));
    TS_ASSERT_EQUALS( std::string(v2->get_spy()) , std::string("3.00"));
    TS_ASSERT_EQUALS( std::string(add->get_spy()), std::string("3.00"));
    TS_ASSERT_EQUALS( std::string(v3->get_spy()) , std::string("3.00"));
    
    v1->bang(gNilSignal); /** ---> 1. bang --> value         = 2
                  *      2. send new value to add  = 2
                  *      3. add.bang --> value     = 5
                  *      4. send to v3             = 5  */
    
    TS_ASSERT_EQUALS( std::string(v1->get_spy()) , std::string("2.00"));
    TS_ASSERT_EQUALS( std::string(v2->get_spy()) , std::string("3.00"));
    TS_ASSERT_EQUALS( std::string(add->get_spy()), std::string("5.00"));
    TS_ASSERT_EQUALS( std::string(v3->get_spy()) , std::string("5.00"));
    
  }
};

class TestParseTest : public CxxTest::TestSuite, public ParseTest
{
public:

  void test_send_nil( void ) 
    { clean_assert_print("n=Test()\nn.2=>p\np=Print()\nn.bang\n","sending nil=><=done.\n"); }

};