// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "node.h"
#include <string>

class Dummy : public Node
{
public:
  // params = hash, 'port:450, baud:3200, xyz:450, title:"home of the braave"'
  bool init (const Params& p)
  {
    mCounter = p.get("counter", 0);
    mName    = p.get("name"   , std::string("no-name"));
    
    make_inlet<Dummy,&Dummy::set_counter>();
    make_outlet<Dummy,&Dummy::increment_counter>();
    
    return true;
  }

  virtual const char * spy() 
  { 
    sprintf(mSpy, "%s: %i", mName.c_str(), mCounter);
    return mSpy;
  }
  
  void set_counter(float value)
  { mCounter = (int)value; }

  float increment_counter()
  { return (float)++mCounter; }
  
private:
  int mCounter;
  std::string mName;
};

class TestNode : public CxxTest::TestSuite
{
public:
  void testCreate( void )
  {
    Node::declare<Dummy>("Dummy");
    
    Node * d = Node::create("Dummy", "counter: 5 name:\"foo\"");
    
    TS_ASSERT_EQUALS( std::string(d->spy()), std::string("foo: 5") );
  }
  
  void testInspect( void )
  {
    Node::declare<Dummy>("Dummy");
    
    Node * d = Node::create("Dummy", "counter: 5 name:\"foo\"");
    d->set_variable_name(std::string("d"));
    
    TS_ASSERT_EQUALS( std::string(d->inspect()), std::string("[Dummy(d) foo: 5]") );
  }
  
  void testBang( void )
  {
    Node::declare<Dummy>("Dummy");
    
    Node * d = Node::create("Dummy", "");
    
    TS_ASSERT_EQUALS( std::string(d->spy()), std::string("no-name: 0") );
    d->bang();
    
    TS_ASSERT_EQUALS( std::string(d->spy()), std::string("no-name: 1") );
  }
  
  void testConnection( void )
  {
    Node::declare<Dummy>("Dummy");
    
    Node   * d1   = Node::create("Dummy", "name:first  counter:3");
    Outlet * out1 = d1->outlet(1); // oulets and inlets are indexed starting with '1', not '0'
    Node   * d2   = Node::create("Dummy", "name:second counter:0");
    Inlet  * in2  = d2->inlet(1);
    
    out1->connect(in2);
    
    TS_ASSERT_EQUALS( std::string("first: 3" ), std::string(d1->spy()) );
    TS_ASSERT_EQUALS( std::string("second: 0"), std::string(d2->spy()) );
    
    
    d1->bang(); /** ---> 1. bang --> increment d1  = 4
                  *      2. send new value to  d2
                  *      3. set d2                 = 4
                  *      4. bang --> increment d2  = 5  */
    TS_ASSERT_EQUALS( std::string("first: 4" ), std::string(d1->spy()) );
    TS_ASSERT_EQUALS( std::string("second: 5"), std::string(d2->spy()) );
  }
  
  void testConnectionOrder( void )
  {
    
    Node   * v1   = Node::create("Value", "value:2");
    Node   * v2   = Node::create("Value", "value:3");
    Node   * add  = Node::create("Add", "");
    Node   * v3   = Node::create("Value", "");
    
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
    
    TS_ASSERT_EQUALS( std::string("2"), std::string(v1->spy()) );
    TS_ASSERT_EQUALS( std::string("3"), std::string(v2->spy()) );
    TS_ASSERT_EQUALS( std::string("0"), std::string(add->spy()));
    TS_ASSERT_EQUALS( std::string("0"), std::string(v3->spy()) );
    
    v2->bang(); /** ---> 1. bang --> value         = 3
                  *      2. send new value to  add      */
    
    TS_ASSERT_EQUALS( std::string("2"), std::string(v1->spy()) );
    TS_ASSERT_EQUALS( std::string("3"), std::string(v2->spy()) );
    TS_ASSERT_EQUALS( std::string("3"), std::string(add->spy()));
    TS_ASSERT_EQUALS( std::string("0"), std::string(v3->spy()) );
    
    add->bang(); /** ---> 1. bang --> value         = 3
                   *      2. send new value to  v3  = 3  */
    
    TS_ASSERT_EQUALS( std::string("2"), std::string(v1->spy()) );
    TS_ASSERT_EQUALS( std::string("3"), std::string(v2->spy()) );
    TS_ASSERT_EQUALS( std::string("3"), std::string(add->spy()));
    TS_ASSERT_EQUALS( std::string("3"), std::string(v3->spy()) );
    
    v1->bang(); /** ---> 1. bang --> value         = 2
                  *      2. send new value to add  = 2
                  *      3. add.bang --> value     = 5
                  *      4. send to v3             = 5  */
    
    TS_ASSERT_EQUALS( std::string("2"), std::string(v1->spy()) );
    TS_ASSERT_EQUALS( std::string("3"), std::string(v2->spy()) );
    TS_ASSERT_EQUALS( std::string("5"), std::string(add->spy()));
    TS_ASSERT_EQUALS( std::string("5"), std::string(v3->spy()) );
    
  }
};