// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "node.h"
#include <string>
#include "test_helper.h"

class Dummy : public Node
{
public:
  // params = hash, 'port:450, baud:3200, xyz:450, title:"home of the braave"'
  bool init (const Params& p)
  {
    mCounter = p.get("dummy", 0);
    mName    = p.get("name"   , std::string("no-name"));
    
    make_inlet<Dummy,&Dummy::set_counter>();
    make_outlet<Dummy,&Dummy::increment_counter>();
    
    return true;
  }

  void spy() 
  { spy_print("%s: %i", mName.c_str(), mCounter); }
  
  void set_counter(const Signal& sig)
  { SET_INTEGER(mCounter, sig) }

  void increment_counter(Signal& sig)
  { SEND_INTEGER(sig, ++mCounter) }
  
private:
  int mCounter;
  std::string mName;
};

class TestNode : public CxxTest::TestSuite, public NodeTester
{  
public:
  void testCreate( void )
  {  
    Node::declare<Dummy>("Dummy"); // we cannot move this into TestNode() because we get EXC_BAD_ACCESS ???
    mNode = Node::create(NULL, "Dummy", "dummy: 5 name:\"foo\"");
    
    assert_spy("foo: 5");
  }
  
  void testInspect( void )
  { 
    mNode = Node::create(NULL, "Dummy", "dummy: 5 name:\"foo\"");
    mNode->set_variable_name(std::string("d"));
    assert_inspect("#<Dummy:d foo: 5>");
  }
  
  void testBang( void )
  {
    mNode = Node::create(NULL, "Dummy", "");
    
    assert_spy("no-name: 0");
    mNode->bang();
    assert_spy("no-name: 1");
  }
  
  void testExecuteMethod( void )
  {
    mNode = Node::create(NULL, "Dummy", "dummy:5");
    
    assert_method_result("bang","","#<Dummy:_10 no-name: 6>\n");
  }
  
  void testConnection( void )
  {
    
    Node   * d1   = Node::create(NULL, "Dummy", "name:first  dummy:3");
    Outlet * out1 = d1->outlet(1); // oulets and inlets are indexed starting with '1', not '0'
    Node   * d2   = Node::create(NULL, "Dummy", "name:second dummy:0");
    Inlet  * in2  = d2->inlet(1);
    
    out1->connect(in2);
    
    TS_ASSERT_EQUALS( std::string("first: 3" ), std::string(d1->get_spy()) );
    TS_ASSERT_EQUALS( std::string("second: 0"), std::string(d2->get_spy()) );
    
    
    d1->bang(); /** ---> 1. bang --> increment d1  = 4
                  *      2. send new value to  d2
                  *      3. set d2                 = 4
                  *      4. bang --> increment d2  = 5  */
    TS_ASSERT_EQUALS( std::string("first: 4" ), std::string(d1->get_spy()) );
    TS_ASSERT_EQUALS( std::string("second: 5"), std::string(d2->get_spy()) );
  }
  
  void testConnectionOrder( void )
  {
    Node   * v1   = Node::create(NULL, "Value", "value:2");
    Node   * v2   = Node::create(NULL, "Value", "value:3");
    Node   * add  = Node::create(NULL, "Add", "");
    Node   * v3   = Node::create(NULL, "Value", "");
    
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
    
    TS_ASSERT_EQUALS( std::string("2.00"), std::string(v1->get_spy()) );
    TS_ASSERT_EQUALS( std::string("3.00"), std::string(v2->get_spy()) );
    TS_ASSERT_EQUALS( std::string("0.00"), std::string(add->get_spy()));
    TS_ASSERT_EQUALS( std::string("0.00"), std::string(v3->get_spy()) );
    
    v2->bang(); /** ---> 1. bang --> value         = 3
                  *      2. send new value to  add      */
    
    TS_ASSERT_EQUALS( std::string("2.00"), std::string(v1->get_spy()) );
    TS_ASSERT_EQUALS( std::string("3.00"), std::string(v2->get_spy()) );
    TS_ASSERT_EQUALS( std::string("3.00"), std::string(add->get_spy()));
    TS_ASSERT_EQUALS( std::string("0.00"), std::string(v3->get_spy()) );
    
    add->bang(); /** ---> 1. bang --> value         = 3
                   *      2. send new value to  v3  = 3  */
    
    TS_ASSERT_EQUALS( std::string("2.00"), std::string(v1->get_spy()) );
    TS_ASSERT_EQUALS( std::string("3.00"), std::string(v2->get_spy()) );
    TS_ASSERT_EQUALS( std::string("3.00"), std::string(add->get_spy()));
    TS_ASSERT_EQUALS( std::string("3.00"), std::string(v3->get_spy()) );
    
    v1->bang(); /** ---> 1. bang --> value         = 2
                  *      2. send new value to add  = 2
                  *      3. add.bang --> value     = 5
                  *      4. send to v3             = 5  */
    
    TS_ASSERT_EQUALS( std::string("2.00"), std::string(v1->get_spy()) );
    TS_ASSERT_EQUALS( std::string("3.00"), std::string(v2->get_spy()) );
    TS_ASSERT_EQUALS( std::string("5.00"), std::string(add->get_spy()));
    TS_ASSERT_EQUALS( std::string("5.00"), std::string(v3->get_spy()) );
    
  }
};