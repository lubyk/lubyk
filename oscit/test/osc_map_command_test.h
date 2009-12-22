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
#include "oscit/root.h"
#include "mock/dummy_object.h"
#include "oscit/log_object.h"
#include "oscit/osc_map_command.h"
#include "mock/osc_command_logger.h"

#define APP1_PORT 7016
#define APP2_PORT 7017

/** In these tests, we have the following setup:
 *  app1_  : contains /foo  + /map     (OscMapCommand)
 *  app2_  : contains /slider/1 + app2_sender_  (OscCommand)
 *  /map receives messages on port APP1_PORT sent by app2_
 *  app1_ notifies changes to app2_ on port APP2_PORT through /map command.
 */
class OscMapCommandTest : public TestHelper
{
 public:
  OscMapCommandTest() : app1_end_point_("oscmap", Location::LOOPBACK, APP1_PORT) {
                                                  // IN      // OUT
    map_cmd_ = app1_.adopt_command(new OscMapCommand(APP1_PORT, APP2_PORT));

    app2_sender_ = app2_.adopt_command(new OscCommandLogger(APP2_PORT, "sender", &logger_));
    // we need to register in order to get return values
    send("/.register", gNilValue);
  }

  void setUp() {
    app2_sender_->clear_replies();
    app1_.Object::clear(); // empty root but keep commands
  }

  void test_received_should_map( void ) {
    DummyObject * foo = app1_.adopt(new DummyObject("foo", 1.0));
    Value res = map_cmd_->script(Value("/slider/1 [0,1] --> /foo [10,20]"));
    assert_false(res.is_error());
    send("/slider/1", 0.5);
    assert_equal(15.0, foo->real());
    assert_equal("[\"/slider/1\", 0.5]\n", reply());
  }

  void test_notifications_should_reverse_map( void ) {
    app1_.adopt(new DummyObject("foo", 1.0));
    Value res = map_cmd_->script(Value("/slider/1 [0,1] --> /foo [10,20]"));
    assert_false(res.is_error());
    app1_.notify_observers(REPLY_PATH, Value("/foo").push_back(17.5));
    // notification should be sent to /slider/1 0.75
    assert_equal("[\"/slider/1\", 0.75]\n", reply());
  }

 private:
  void send(const char *url, Real real) {
    send(url, Value(real));
  }

  void send(const char *url, const Value &val) {
    app2_sender_->send(app1_end_point_, url, val);
    millisleep(50);
  }

  std::string reply() {
    millisleep(50);
    return app2_sender_->replies();
  }

  Mutex context_;
  Location app1_end_point_;
  Root app1_;
  Root app2_;
  OscCommandLogger *app2_sender_;
  OscMapCommand *map_cmd_;
  Logger logger_;
};
