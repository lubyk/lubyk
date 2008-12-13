#include "test_helper.h"

class NoteOutTest : public ParseHelper
{
public:
  
  void test_defaults( void ) 
  { assert_result("n=NoteOut()\n","#<NoteOut:n +1:C0(70), 0/500>\n"); }

  void test_note_out( void ) 
  { assert_print("n=NoteOut()\np=Print()\nn=>p\nn.b\n","<Midi +1:C0(70), 0/500>\n"); }

};