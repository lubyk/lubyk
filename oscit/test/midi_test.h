#include "test_helper.h"
#include "oscit/midi_message.h"

class MidiTest : public TestHelper
{  
public:
  void test_defaults( void ) {
    MidiMessage m;
    assert_equal(60, m.note());
    assert_equal(80, m.velocity());
    assert_equal(500, m.length());
    assert_equal(1, m.channel());
    assert_equal(0, m.wait());
    assert_equal(NoteOn, m.type());
  }
  
  void test_create_note( void ) {
    MidiMessage m;
    m.set_as_note(63);
    assert_equal(63, m.note());
    assert_equal(80, m.velocity());
    assert_equal(500, m.length());
    assert_equal(1, m.channel());
    assert_equal(0, m.wait());
    
    m.set_as_note(68);
    assert_equal(68, m.note());
    assert_equal(80, m.velocity());
    assert_equal(500, m.length());
    assert_equal(1, m.channel());
    assert_equal(0, m.wait());
  }
  
  void test_create_ctrl( void ) {
    MidiMessage m;
    m.set_as_ctrl(19, 45);
    assert_equal(19, m.ctrl());
    assert_equal(45, m.value());
    assert_equal(1, m.channel());
    assert_equal(0, m.wait());
    
    m.set_as_note(12, 55);
    assert_equal(12, m.ctrl());
    assert_equal(55, m.value());
    assert_equal(1, m.channel());
    assert_equal(0, m.wait());
  }
  
  void test_set_note( void ) {
    MidiMessage m;
    m.set_as_note(212);
    assert_equal(127, m.note());
    m.set_note(-128);
    assert_equal(0, m.note());
    m.set_note(128);
    assert_equal(127, m.note());
    m.set_note(55);
    assert_equal(55, m.note());
  }
  
  void test_set_velocity( void ) {
    MidiMessage m;
    m.set_as_note(55, 215);
    assert_equal(127, m.velocity());
    m.set_velocity(-128);
    assert_equal(0, m.velocity());
    m.set_velocity(128);
    assert_equal(127, m.velocity());
    m.set_velocity(55);
    assert_equal(55, m.velocity());
  }
  
  void test_set_length( void ) {
    MidiMessage m;
    m.set_as_note(55, 110, 800);
    assert_equal(800, m.length());
    m.set_length(-10);
    assert_equal(0, m.length());
    m.set_length(125);
    assert_equal(125, m.length());
  }
  
  void test_set_channel( void ) {
    MidiMessage m;
    m.set_as_note(55, 110, 800, 18);
    assert_equal(16, m.channel());
    m.set_channel(-128);
    assert_equal(1, m.channel());
    m.set_channel(16);
    assert_equal(16, m.channel());
    m.set_channel(0);
    assert_equal(1, m.channel());
    m.set_channel(3);
    assert_equal(3, m.channel());
  }
};
