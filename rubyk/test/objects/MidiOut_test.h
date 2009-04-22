#include "test_helper.h"

class MidiOutTest : public ParseHelper
{
public:
  
  void test_open_virtual_port( void ) { 
    assert_result("# <MidiOut:/zoom port:\"zoom\">\n", "zoom=MidiOut()\n");
  }
};