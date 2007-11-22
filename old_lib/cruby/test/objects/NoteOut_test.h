#include "test_helper.h"

class NoteOutTest : public CxxTest::TestSuite, public ParseTest
{
public:
  
  void test_defaults( void ) 
  { clean_assert_result("n=NoteOut()\n","#<NoteOut:n  1:C0(70), 500>\n"); }

  void test_note_out( void ) 
  { clean_assert_bang("n=NoteOut()\n","<note  1:C0(70)>\n"); }

  void test_note_on_off( void ) 
  { clean_assert_run(100,"n=NoteOut(length:50 velocity:40)\nn => p\np=Print()\nn.bang\n","<note  1:C0(40)>\n<note -1:C0(40)>\n"); }
  
  void test_clear( void )
  { clean_assert_print("n=NoteOut()\nn=>p\np=Print()\nn.bang\nn.clear\n", "<note  1:C0(70)>\n<note -1:C0(70)>\n");}
};