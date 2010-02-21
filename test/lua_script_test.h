/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
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

#include <stdio.h>
#include <string>

#include "test_helper.h"

class LuaScriptTest : public TestHelper {
public:
  virtual void setUp() {
    planet_ = new Planet();
    planet_->call(LIB_URL, Value(TEST_LIB_PATH));
    script_ = planet_->adopt(new LuaScript);
    // all this is done by Class normally
    script_->set_name("lua");
    script_->adopt(new Object("in"));
    script_->adopt(new Object("out"));
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
    assert_true(planet_->get_object_at("/lua/in/tempo", &inlet));
    assert_equal("fss", inlet->type().type_tag());
    assert_equal(0.0, inlet->type()[0].r);
    assert_equal("bpm", inlet->type()[1].str());
    assert_equal("Main beat machine tempo.", inlet->type()[2].str());

    res = parse("inlet('tempo', {0.0, 'bpm', 'Main beat machine tempo.'})");
    assert_true(res.is_string()); // no error
  }

  void test_add_inlet_RealIO( void ) {
    // also tests loading of rubyk.lua
    Value res = parse("inlet('tempo', RealIO('bpm', 'Main beat machine tempo.'))");
    assert_true(res.is_string());
    ObjectHandle inlet;
    assert_true(planet_->get_object_at("/lua/in/tempo", &inlet));
    assert_equal("fss", inlet->type().type_tag());
    assert_equal(0.0, inlet->type()[0].r);
    assert_equal("bpm", inlet->type()[1].str());
    assert_equal("Main beat machine tempo.", inlet->type()[2].str());
  }

  void test_add_inlet_NilIO( void ) {
    // also tests loading of rubyk.lua
    Value res = parse("inlet('boom', NilIO('Ping pong.'))");
    assert_true(res.is_string());
    ObjectHandle inlet;
    assert_true(planet_->get_object_at("/lua/in/boom", &inlet));
    assert_equal("Ns", inlet->type().type_tag());
    assert_equal("Ping pong.", inlet->type()[1].str());
  }

  void test_add_outlet( void ) {
    Value res = parse("note = Outlet('note', RealIO('midi note', 'Sends note values.'))");
    assert_true(res.is_string());
    ObjectHandle outlet;
    assert_true(planet_->get_object_at("/lua/out/note", &outlet));
    assert_equal("fss", outlet->type().type_tag());
    assert_equal("midi note", outlet->type()[1].str()); // units
    assert_equal("Sends note values.", outlet->type()[2].str()); // info
  }

  // sending tested in LuaTest

private:
  const Value parse(const char *string) {
    return script_->script(Value(string));
  }

  Planet    *planet_;
  LuaScript *script_;
};
