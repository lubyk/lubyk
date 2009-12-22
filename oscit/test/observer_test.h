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
#include "oscit/thash.h"
#include "mock/observer_logger.h"

class ObserverTest : public TestHelper
{
public:
  void test_set_and_hold_should_set_value( void ) {
    Logger logger;
    ObserverLogger o("observer", &logger);
    ObserverLogger a("a", &logger);
    ObserverLogger b("b", &logger);
    assert_equal((ObserverLogger*)NULL, o.x_);
    assert_equal((ObserverLogger*)NULL, o.y_);

    o.set_x(&a);
    assert_equal(&a, o.x_);

    o.set_y(&b);
    assert_equal(&b, o.y_);
  }

  void test_destroy_observed_should_set_value_to_null( void ) {
    Logger logger;
    ObserverLogger o("observer", &logger, ObserverLogger::VarTest);
    ObserverLogger *a = new ObserverLogger("a", &logger);

    o.set_x(a);
    assert_equal(a, o.x_);
    assert_equal("", logger.str());

    delete a;
    // lock/unlock called twice:
    // - during the trigger of the 'on destroy' callback
    // - during the removal of the observer's "produced" callbacks
    assert_equal("[observer: lock a NULL][observer: unlock NULL NULL][observer: lock NULL NULL][observer: unlock NULL NULL]", logger.str());
    assert_equal((ObserverLogger*)NULL, o.x_);
  }

  void test_destroy_observer_before_observed_should_not_notify( void ) {
    Logger logger;
    ObserverLogger *o = new ObserverLogger("observer", &logger);
    ObserverLogger *a = new ObserverLogger("a", &logger);

    o->set_x(a);
    assert_equal(a, o->x_);
    logger.str("");

    delete o;
    assert_equal("[a: lock][a: unlock]", logger.str());
    logger.str("");
    delete a; // should not call o
    assert_equal("", logger.str());
  }

  void test_push_back_should_append_value( void ) {
    Logger logger;
    ObserverLogger o("observer", &logger);
    ObserverLogger a("a", &logger);
    ObserverLogger b("b", &logger);

    assert_equal((ObserverLogger*)NULL, o.x_);
    assert_equal((ObserverLogger*)NULL, o.y_);

    o.push_back_x(&a);
    assert_equal(&a, o.list_x_.front());

    o.push_back_y(&b);
    assert_equal(&b, o.list_y_.front());
  }

  void test_destroy_observed_should_remove_value_from_list( void ) {
    Logger logger;
    ObserverLogger o("observer", &logger, ObserverLogger::ListXTest);
    ObserverLogger *a = new ObserverLogger("a", &logger);

    o.push_back_x(a);
    logger.str("");

    delete a;
    // lock/unlock called twice:
    // - during the trigger of a's 'on destroy' callbacks in 'observer'
    // - during the removal of the observer's "produced" callbacks
    assert_equal("[observer: lock 1][observer: unlock 0][observer: lock 0][observer: unlock 0]", logger.str());
    assert_equal((ObserverLogger*)NULL, o.x_);
  }

  void test_destroy_observer_before_observed_should_not_notify_list_owner( void ) {
    Logger logger;
    ObserverLogger *o = new ObserverLogger("observer", &logger);
    ObserverLogger *a = new ObserverLogger("a", &logger);

    o->push_back_x(a);
    // we lock object 'a' when registering 'on_destroy' callbacks
    logger.str("[a: lock][a: unlock]");
    logger.str("");

    delete o;
    // we lock while removing 'on_destroy' callbacks in a
    assert_equal("[a: lock][a: unlock]", logger.str());
    logger.str("");
    delete a; // should not call o
    assert_equal("", logger.str());
  }
};