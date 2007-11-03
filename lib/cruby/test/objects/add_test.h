#include "node.h"

class TestAdd : public CxxTest::TestSuite, public NodeTester
{
public:
  
  void testCreate( void )
  {
    mNode = Class::create(NULL, "Add", "");
    // ...
  }
};