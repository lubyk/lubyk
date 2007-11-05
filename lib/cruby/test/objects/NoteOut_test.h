#include "node.h"

class TestAdd : public CxxTest::TestSuite, public NodeTester
{
public:
  
  void testSendNoteOut( void )
  {
    mNode = Class::create(NULL, "NoteOut", "note:63 velocity:70 channel:2");
    assert_spy("Eb0(70,2), 500");
  }
};