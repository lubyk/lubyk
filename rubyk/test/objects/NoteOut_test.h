#include "test_helper.h"

class NoteOutTest : public ParseHelper
{
public:
  
  void test_defaults( void ) {
    assert_result("# <NoteOut:/n note:\"MidiMessage +1:C3(80), 0/500\">\n", "n=NoteOut()\n");
  }

  void test_note_out( void ) {
    setup_with_print("n=NoteOut()\n");
    assert_print("p: \"MidiMessage +1:C3(80), 0/500\"\n", "n/note\n");
  }
  
  void test_set_note_no_out( void ) {
    setup_with_print("n=NoteOut()\n");
    assert_print("", "n/set_note(55)\n");
    assert_result("# 45\n", "n/set_note(45)\n");
    assert_result("# 127\n", "n/set_note(128)\n");
    assert_result("# 0\n", "n/set_note(-12)\n");
    assert_result("# 55\n", "n/set_note(55)\n");
    assert_result("# <NoteOut:/n note:\"MidiMessage +1:G2(80), 0/500\">\n", "n\n");
  }
  
  void test_set_velocity( void ) {
    setup("n=NoteOut(velocity:85)\n");
    assert_result("# 85\n", "n/velocity\n");
    assert_result("# 127\n", "n/velocity(250)\n");
    assert_result("# 127\n", "n/velocity\n");
    assert_result("# 110\n", "n/velocity(110)\n");
    assert_result("# 110\n", "n/velocity\n");
    assert_result("# <NoteOut:/n note:\"MidiMessage +1:C3(110), 0/500\">\n", "n\n");
  }
  
  void test_set_length( void ) {
    setup("n=NoteOut(length:250)\n");
    assert_result("# 250\n", "n/length\n");
    assert_result("# 125\n", "n/length(125)\n");
    assert_result("# 125\n", "n/length\n");
    assert_result("# <NoteOut:/n note:\"MidiMessage +1:C3(80), 0/125\">\n", "n\n");
  }
  
  void test_set_channel( void ) {
    setup("n=NoteOut(channel:7)\n");
    assert_result("# 7\n", "n/channel\n");
    assert_result("# 16\n", "n/channel(32)\n");
    assert_result("# 16\n", "n/channel\n");
    assert_result("# 1\n", "n/channel(0)\n");
    assert_result("# 1\n", "n/channel\n");
    assert_result("# 5\n", "n/channel(5)\n");
    assert_result("# 5\n", "n/channel\n");
    assert_result("# <NoteOut:/n note:\"MidiMessage +5:C3(80), 0/500\">\n", "n\n");
  }

};