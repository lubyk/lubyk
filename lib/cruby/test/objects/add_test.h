#include "node.h"

class TestAdd : public CxxTest::TestSuite, public NodeTester
{
public:
  
  void testCreate( void )
  {
    mNode = Node::create(NULL, "Add", "");
    // ...
  }
};