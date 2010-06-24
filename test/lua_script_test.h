/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

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

#include <stdio.h>
#include <string>

#include "test_helper.h"
#include "oscit/matrix.h"

class LuaScriptTest : public TestHelper {
public:
  virtual void setUp() {
    planet_ = new Planet();
    script_ = planet_->adopt(new LuaScript);
    // all this is done by Class normally
    script_->set_name("lua");
    script_->adopt(new Object("in"));
    script_->adopt(new Object(NODE_OUT_KEY));
    Value res = script_->init();
    if (res.is_error()) {
      std::cout << "Could not init LuaScript !\n" << res << "\n";
    }
  }

  void tearDown() {
    if (planet_) delete planet_;
    planet_ = NULL;
  }

  void test_compile( void ) {
    Value res = parse("function foo()\nend");
    assert_true(res.is_string());
    assert_equal("function foo()\nend", res.str());
    assert_true(script_->is_ok());
  }

  void test_compilation_error( void ) {
    Value res = parse("function foo\nend");
    assert_true(res.is_error());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());
    assert_equal("[string \"lua\"]:2: \'(\' expected near \'end\'.", res.error_message());
    assert_false(script_->is_ok());
  }

  void test_runtime_error( void ) {
    Value res = parse("x = y + 3"); // y undefined
    assert_true(res.is_error());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());
    assert_equal("[string \"lua\"]:1: attempt to perform arithmetic on global 'y' (a nil value).", res.error_message());
    assert_false(script_->is_ok());
  }

  void test_add_inlet( void ) {
    Value res = parse("inlet('tempo', {0.0, 'bpm', 'Main beat machine tempo.'})");
    assert_true(res.is_string());
    ObjectHandle inlet;
    assert_true(planet_->get_object_at("/lua/tempo", &inlet));
    assert_equal("fss", inlet->type().type_tag());
    assert_equal(0.0, inlet->type()[0].r);
    assert_equal("bpm", inlet->type()[1].str());
    assert_equal("Main beat machine tempo.", inlet->type()[2].str());

    res = parse("inlet('tempo', {0.0, 'bpm', 'Main beat machine tempo.'})");
    assert_true(res.is_string()); // no error
  }

  void test_add_inlet_RealIO( void ) {
    // also tests loading of rubyk.lua
    Value res = parse("inlet('tempo', RealIO('Main beat machine tempo [bpm].'))");
    assert_true(res.is_string());
    ObjectHandle inlet;
    assert_true(planet_->get_object_at("/lua/tempo", &inlet));
    assert_equal("fs", inlet->type().type_tag());
    assert_equal(0.0, inlet->type()[0].r);
    assert_equal("Main beat machine tempo [bpm].", inlet->type()[1].str());
  }

  // ------------------------------------------------------------------------  False

  void test_return_false( void ) {
    Value res = parse("function foo()\n return false\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo");
    assert_true(res.is_false());
  }

  // ------------------------------------------------------------------------  True (Bang)

  void test_return_true( void ) {
    Value res = parse("function foo()\n return true\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo");
    assert_true(res.is_true());
  }

  void test_receive_true( void ) {
    Value res = parse("function foo(v)\n return v == true\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo", gTrueValue);
    assert_true(res.is_true());
  }

  void test_add_inlet_BangIO( void ) {
    // also tests loading of rubyk.lua
    Value res = parse("inlet('boom', BangIO('Ping pong.'))");
    assert_true(res.is_string());
    ObjectHandle inlet;
    assert_true(planet_->get_object_at("/lua/boom", &inlet));
    assert_equal("Ts", inlet->type().type_tag());
    assert_equal("Ping pong.", inlet->type()[1].str());
  }

  // ------------------------------------------------------------------------  Matrix

  void test_create_matrix( void ) {
    Value matrix_value(Matrix(2,3,CV_32FC1));
    Value res = parse("require('cv'); function foo()\n return cv.Mat(2,3,cv.CV_32FC1)\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo");
    assert_true(res.is_matrix());
    assert_equal(matrix_value.matrix_->type(), res.matrix_->type());
  }

  void test_call_lua_with_matrix( void ) {
    Value matrix_value(Matrix(2,3,CV_32FC1));
    Value res = parse("require('cv'); function foo(m)\n return m\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo", matrix_value);
    assert_true(res.is_matrix());
    assert_equal(matrix_value.matrix_->data, res.matrix_->data);
  }

  void test_add_inlet_MatrixIO( void ) {
    // also tests loading of rubyk.lua
    Value res = parse("require('cv'); inlet('boom', MatrixIO('Ping pong.'))");
    assert_true(res.is_string());
    ObjectHandle inlet;
    assert_true(planet_->get_object_at("/lua/boom", &inlet));
    assert_equal("Ms", inlet->type().type_tag());
    assert_equal("Ping pong.", inlet->type()[1].str());
  }

  // ------------------------------------------------------------------------  Midi

  void test_create_midi( void ) {
    Value res = parse("function foo()\n return oscit.MidiMessage_Note(65,68)\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo");
    assert_true(res.is_midi());
    std::ostringstream oss;
    oss << res;
    assert_equal("\"MidiMessage +1:F3(68), 0/500\"", oss.str());
  }

  void test_call_lua_with_MidiValue( void ) {
    MidiValue midi_value;
    midi_value.set_as_note(45, 78, 1000);
    Value res = parse("function foo(m)\n return m\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo", midi_value);
    // moving the message into Lua makes a copy because we loose constness
    // if this is an important performance hit, we could discard const checking but
    // this could lead to other problems such as kept values in lua that keep changing
    // without proper mutex protections.
    assert_false(midi_value.midi_message_ == res.midi_message_);
  }

  void test_add_inlet_MidiIO( void ) {
    // also tests loading of rubyk.lua
    Value res = parse("inlet('boom', MidiIO('Ping pong.'))");
    assert_true(res.is_string());
    ObjectHandle inlet;
    assert_true(planet_->get_object_at("/lua/boom", &inlet));
    assert_equal("ms", inlet->type().type_tag());
    assert_equal("Ping pong.", inlet->type()[1].str());
  }

  void test_add_outlet_MidiIO( void ) {
    // also tests loading of rubyk.lua
    Value res = parse("boom = Outlet('boom', MidiIO('Ping pong.'))");
    assert_true(res.is_string());
    ObjectHandle outlet;
    assert_true(planet_->get_object_at("/lua/out/boom", &outlet));
    assert_equal("ms", outlet->type().type_tag());
    assert_equal("Ping pong.", outlet->type()[1].str()); // info
  }

  void test_add_outlet( void ) {
    Value res = parse("force = Outlet('force', RealIO('Dark Force.'))");
    assert_true(res.is_string());
    ObjectHandle outlet;
    assert_true(planet_->get_object_at("/lua/out/force", &outlet));
    assert_equal("fs", outlet->type().type_tag());
    assert_equal("Dark Force.", outlet->type()[1].str()); // info
  }

  void test_call_lua( void ) {
    Value res = parse("function foo(x,y)\n return x+y\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo", Value(5).push_back(9));
    assert_equal(14.0, res.r);
  }

  void test_call_lua_with_nil( void ) {
    Value res = parse("function foo()\n return 45\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo", gNilValue);
    assert_equal(45.0, res.r);
  }

  void test_exception_thrown_in_lua( void ) {
    Value res = parse("require('cv'); function foo()\n cv.subtract(cv.Mat(3,3,cv.CV_32FC1), cv.Mat(3,3,cv.CV_32FC2), cv.Mat(3,3,cv.CV_32FC2))\nend");
    assert_true(res.is_string());
    res = script_->call_lua("foo", gNilValue);
    assert_true(res.is_error());
    assert_equal("Could not call foo(null): cv.subtract: failed (-215) size == src2.size() && type == src2.type() && func != 0 in function subtract\n", res.error_message());
  }

  void test_rk_call( void ) {
    // also tests loading of rubyk.lua
    Value res = parse("function foo() return rk.call('/class/lib'); end");
    assert_true(res.is_string());
    res = script_->call_lua("foo", gNilValue);
    assert_equal("\"lib:~/rubyk/lib:/usr/local/lib/rubyk\"", res.to_json());
  }

  // sending tested in LuaTest

private:
  const Value parse(const char *string) {
    return script_->script(Value(string));
  }

  Planet    *planet_;
  LuaScript *script_;
};
