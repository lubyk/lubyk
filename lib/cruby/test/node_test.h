// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "Node.h"
#include "Inlet.h"
#include "Outlet.h"
#include <string>

class Dummy : public Node
{
public:
  // params = hash, 'port:450, baud:3200, xyz:450, title:"home of the braave"'
  bool init (Params& p)
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
  void testCreate( void ) {
    Node::declare<Dummy>("dummy");
    
    Node * d = Node::create("dummy", "counter: 5 name:\"foo\"");
    
    TS_ASSERT_EQUALS( std::string("foo: 5"), std::string(d->spy()) );
  }
  
  void testBang( void ) {
    Node::declare<Dummy>("dummy");
    
    Node * d = Node::create("dummy", "");
    
    TS_ASSERT_EQUALS( std::string("no-name: 0"), std::string(d->spy()) );
    d->bang();
    
    TS_ASSERT_EQUALS( std::string("no-name: 1"), std::string(d->spy()) );
  }
  
  void testConnection( void ) {
    Node::declare<Dummy>("dummy");
    
    Node   * d1   = Node::create("dummy", "name:first  counter:3");
    Outlet * out1 = d1->outlet(0);
    Node   * d2   = Node::create("dummy", "name:second counter:0");
    Inlet  * in2  = d2->inlet(0);
    
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
};