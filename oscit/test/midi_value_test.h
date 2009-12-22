/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#include "test_helper.h"
#include "oscit/values.h"

class MidiValueTest : public TestHelper
{
public:
  void test_is_midi( void ) {
    Value v(TypeTag("m"));

    assert_false(v.is_empty());
    assert_false(v.is_nil());
    assert_false(v.is_real());
    assert_false(v.is_string());
    assert_false(v.is_list());
    assert_false(v.is_error());
    assert_false(v.is_hash());
    assert_false(v.is_matrix());
    assert_true (v.is_midi());

    MidiMessage *msg = v.midi_message_;

    assert_equal(NoteOn, msg->type());

    assert_equal("m", v.type_tag());
    int i = H("m");
    assert_equal(i, v.type_id());
  }

  void test_create( void ) {
    MidiValue v;

    assert_true(v.is_midi());

    assert_equal("m", v.type_tag());
  }

  void test_create_with_char( void ) {
    Value v('m');

    assert_true(v.is_midi());
    assert_equal(NoteOn, v.midi_message_->type());
  }

  void test_create_with_TypeTag( void ) {
    Value v(TypeTag("m"));

    assert_true(v.is_midi());
    assert_equal(NoteOn, v.midi_message_->type());
  }


  void test_share( void ) {
    Value v(TypeTag("m"));
    v.set_as_note(45, 78, 1000);

    Value v2(v);
    Value v3;

    assert_true(v2.is_midi());
    assert_equal(45, v2.midi_message_->note());

    v.midi_message_->set_note(60);

    // change in v changes v2
    assert_equal(60, v2.midi_message_->note());

    assert_true(v3.is_empty());

    v3 = v;

    assert_true(v3.is_midi());

    assert_equal(60, v3.midi_message_->note());

    v.midi_message_->set_note(80);

    // change in v changes v3
    assert_equal(80, v3.midi_message_->note());
  }

  void test_set( void ) {
    Value v;
    MidiMessage m;
    m.set_as_note(32);

    assert_true(v.is_empty());
    v.set(m);
    assert_true(v.is_midi());
    assert_equal(32, v.midi_message_->note());
    m.set_note(60);
    // was copied
    assert_equal(32, v.midi_message_->note());
  }

  void test_set_type_tag( void ) {
    Value v;
    v.set_type_tag("m");
    assert_true(v.is_midi());
    assert_equal(NoteOn, v.midi_message_->type());
  }

  void test_set_type( void ) {
    Value v;

    v.set_type(MIDI_VALUE);
    assert_true(v.is_midi());
    assert_equal(NoteOn, v.midi_message_->type());
  }

  void test_to_json( void ) {
    Value v;
    v.set_as_note(60, 80, 400, 4, 50);
    std::ostringstream os(std::ostringstream::out);
    os << v;
    assert_equal("\"MidiMessage +4:C3(80), 50/400\"", os.str());
    assert_equal("\"MidiMessage +4:C3(80), 50/400\"", v.to_json());
  }

  // to and from_json not possible yet.

  void test_can_receive( void ) {
    Object object("foo", MidiIO("notes and stuff"));
    assert_false(object.can_receive(Value()));
    assert_true (object.can_receive(gNilValue));
    assert_false(object.can_receive(Value(1.23)));
    assert_false(object.can_receive(Value("foo")));
    assert_false(object.can_receive(Value(BAD_REQUEST_ERROR, "foo")));
    assert_false(object.can_receive(JsonValue("['','']")));
    assert_false(object.can_receive(HashValue()));
    assert_false(object.can_receive(MatrixValue(1,1)));
    assert_true (object.can_receive(MidiValue()));
  }

  void test_equal( void ) {
    Value a;
    a.set_as_note(50, 80, 400, 4, 50);
    Value b;
    b.set_as_note(50, 80, 400, 4, 50);
    assert_equal(a, b);
    a.set_as_note(50, 81, 400, 4, 50);
    assert_false(a == b);
    a.set(4);
    assert_false(a == b);
    a.set_nil();
    assert_false(a == b);
  }
};
