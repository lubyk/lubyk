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
#include "oscit/thread.h"
#include "oscit/root_proxy.h"

#include "mock/command_logger.h"
#include "mock/object_proxy_logger.h"
#include "mock/dummy_object.h"

#include <sstream>

namespace oscit {
class CommandTest : public TestHelper
{
public:
  void test_create_delete( void ) {
    Root * root = new Root;
    Logger logger;
    root->adopt_command(new CommandLogger(&logger));
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. quit [end]
    millisleep(30);
    delete root;
    // should join here
    assert_equal("[dummy: listen][dummy: .][dummy: .]", logger.str());
  }

  void test_many_root_many_commands( void ) {
    Root * root1 = new Root;
    Root * root2 = new Root;
    Logger logger1;
    Logger logger2;
    Logger logger3;
    Logger logger4;

    CommandLogger *d1 = root1->adopt_command(new CommandLogger(&logger1)); // first registered
    CommandLogger *d2 = root1->adopt_command(new CommandLogger(&logger2)); // not registered (same protocol)
    CommandLogger *d3 = root1->adopt_command(new CommandLogger("doom", &logger3));
    CommandLogger *d4 = root2->adopt_command(new CommandLogger(&logger4)); // registered (same protocol, other root)

    assert_true(d1 != NULL);
    assert_true(d2 == NULL);
    assert_true(d3 != NULL);
    assert_true(d4 != NULL);

    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. quit [end]
    millisleep(30);
    delete root1;
    delete root2;

    // should join here
    assert_equal("[dummy: listen][dummy: .][dummy: .]", logger1.str());
    assert_equal("", logger2.str()); // never started (same protocol)
    assert_equal("[doom: listen][doom: .][doom: .]", logger3.str());
    assert_equal("[dummy: listen][dummy: .][dummy: .]", logger4.str());
  }

  void test_root_remote_object_at( void ) {
    Root root;
    Logger logger;
    CommandLogger *cmd = root.adopt_command(new CommandLogger(&logger));
    Value error;
    Object *obj  = root.object_at(Url("dummy://dummy.host:2009/one/two/testing"), &error); // builds reference

    DummyObject *object = (DummyObject*) cmd->remote_object_no_build("dummy://dummy.host:2009/one/two/testing");
    assert_equal(2009.0, object->real());
    assert_equal((Object*)object, obj);
  }

  void test_root_send( void ) {
    Root root;
    Logger logger;
    CommandLogger *cmd = root.adopt_command(new CommandLogger(&logger));
    Value res = root.send(Url("dummy://dummy.host:2009/one/two/testing"), gNilValue);
    DummyObject *object = (DummyObject*) cmd->remote_object_no_build("dummy://dummy.host:2009/one/two/testing");
    // object created (the 2009 port thing is a hack for testing)
    assert_equal(2009.0, object->real());
    root.send(Url("dummy://dummy.host:2009/one/two/testing"), Value(120));
    // normally, the remote object should send data out. In this case we just trigger a DummyObject.
    assert_equal(120.0, object->real());

    res = root.send(Url("dummy://unknown.host/foo/bar"), Value(45));
    assert_true(res.is_error());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());
  }

  void test_should_find_proxy_from_location( void ) {
    Logger logger;
    CommandLogger cmd(&logger);
    Location remote("oscit", "my place");
    Location remote2("oscit", "my place2");
    RootProxy *proxy = cmd.adopt_proxy(new RootProxy(remote));
    RootProxy *found = cmd.find_proxy(remote2);
    assert_equal((RootProxy *)NULL, found);
    found = cmd.find_proxy(remote);
    assert_equal(proxy, found);
  }

  void test_should_find_proxy_from_position( void ) {
    Logger logger;
    CommandLogger cmd(&logger);
    Location remote("oscit", "my place");
    Location remote2("oscit", "my place2");
    RootProxy *proxy = cmd.adopt_proxy(new RootProxy(remote));
    assert_equal(1, cmd.root_proxies_count());
    assert_equal(proxy, cmd.root_proxy_at_index(0));
    assert_equal((RootProxy *)NULL, cmd.root_proxy_at_index(1));
    delete proxy;
    assert_equal(0, cmd.root_proxies_count());
    assert_equal((RootProxy *)NULL, cmd.root_proxy_at_index(0));
  }

  void test_should_not_find_proxy_from_location_after_proxy_deletion( void ) {
    Logger logger;
    CommandLogger cmd(&logger);
    Location remote("oscit", "my place");
    RootProxy *proxy = cmd.adopt_proxy(new RootProxy(remote));
    RootProxy *found;

    delete proxy;
    found = cmd.find_proxy(remote);
    assert_equal((RootProxy *)NULL, found);
  }

  void test_should_handle_register_messages( void ) {
    Logger logger;
    CommandLogger cmd(&logger);
    assert_equal(0, cmd.observers().size());
    // receive is protected, we need to be friend...
    cmd.receive(Url("dummy://unknown.host:4560/.register"), gNilValue);
    assert_equal(1, cmd.observers().size());
    assert_equal("unknown.host", cmd.observers().front().name());
  }

  void test_should_handle_reply_messages( void ) {
    Logger logger;
    CommandLogger cmd("dummy", &logger);
    assert_equal(0, cmd.observers().size());
    RootProxy *root_proxy = cmd.adopt_proxy(new RootProxy(Location("dummy", "some place")));
    root_proxy->adopt(new ObjectProxyLogger("foo", RangeIO(1, 127, "tint", "This is a slider from 1 to 127."), &logger));
    // receive is protected, we need to be friend...
    logger.str("");
    cmd.receive(Url("dummy://\"some place\"/.reply"), Value(Json("[\"/foo\", 5]")));
    assert_equal("[foo: value_changed 5]", logger.str());
  }

  void test_should_notify_observers( void ) {
    Logger logger;
    Root root;
    root.adopt_command(new CommandLogger(&logger));
    logger.str("");
    root.notify_observers("/flop", Value(5));
    assert_equal("[dummy: notify /flop 5]", logger.str());
  }

  void test_receive_meta_should_not_notify_observers( void ) {
    Logger logger;
    Root root;
    CommandLogger *cmd = root.adopt_command(new CommandLogger(&logger));
    root.adopt_command(new CommandLogger("http", &logger));
    root.adopt_command(new CommandLogger("osc", &logger));
    root.adopt(new DummyObject("foo", 4.5));
    logger.str("");

    // receive is protected, we need to be friend...
    cmd->receive(Url("dummy://unknown.host:4560/.type"), Value(""));
    assert_equal("[dummy: send dummy://unknown.host:4560 /.reply [\"/.type\", [\"\", \"No information on this node.\"]]]", logger.str());
  }

  void test_receive_should_notify_observers( void ) {
    Logger logger;
    Root root;
    CommandLogger *cmd = root.adopt_command(new CommandLogger(&logger));
    root.adopt_command(new CommandLogger("http", &logger));
    root.adopt_command(new CommandLogger("osc", &logger));
    root.adopt(new DummyObject("foo", 4.5));
    logger.str("");

    // receive is protected, we need to be friend...
    cmd->receive(Url("dummy://unknown.host:4560/foo"), Value(5.2));
    assert_equal("[dummy: notify /.reply [\"/foo\", 5.2]][http: notify /.reply [\"/foo\", 5.2]][osc: notify /.reply [\"/foo\", 5.2]]", logger.str());
  }

  void test_register_proxy( void ) {
    Logger logger;
    CommandLogger cmd(&logger);
    Location location("oscit", 167772167, 45);
    RootProxy proxy(location);
    assert_true(NULL  == cmd.find_proxy(location));
    cmd.register_proxy(&proxy);
    assert_true(&proxy == cmd.find_proxy(location));
    cmd.unregister_proxy(&proxy);
    assert_true(NULL  == cmd.find_proxy(location));
  }

};

} // oscit