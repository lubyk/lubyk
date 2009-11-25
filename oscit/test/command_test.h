#include "test_helper.h"
#include "oscit/thread.h"
#include "mock/dummy_command.h"
#include "oscit/root_proxy.h"

#include <sstream>

class CommandTest : public TestHelper
{
public:
  void test_create_delete( void ) {
    Root * root = new Root;
    std::string string;
    root->adopt_command(new DummyCommand(&string));
    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. quit [end]
    millisleep(30);
    delete root;
    // should join here
    assert_equal("..", string);
  }

  void test_many_root_many_commands( void ) {
    Root * root1 = new Root;
    Root * root2 = new Root;
    std::string string1;
    std::string string2;
    std::string string3;
    DummyCommand *d1 = root1->adopt_command(new DummyCommand(&string1)); // first registered
    DummyCommand *d2 = root1->adopt_command(new DummyCommand(&string2)); // not registered (same protocol)
    DummyCommand *d3 = root1->adopt_command(new DummyCommand(&string2, "doom"));
    DummyCommand *d4 = root2->adopt_command(new DummyCommand(&string3));

    assert_true(d1 != NULL);
    assert_true(d2 == NULL);
    assert_true(d3 != NULL);
    assert_true(d4 != NULL);

    // let it run 2 times: +1 ... 10ms ... +1 ... 5ms .. quit [end]
    millisleep(30);
    delete root1;
    delete root2;

    // should join here
    assert_equal("..", string1);
    assert_equal("..", string2);
    assert_equal("..", string3);
  }

  void test_root_remote_object_at( void ) {
    Root root;
    std::string string;
    DummyCommand *cmd = root.adopt_command(new DummyCommand(&string));
    Value error;
    Object *obj  = root.object_at(Url("dummy://dummy.host:2009/one/two/testing"), &error); // builds reference

    DummyObject *object = (DummyObject*) cmd->remote_object_no_build("dummy://dummy.host:2009/one/two/testing");
    assert_equal(2009.0, object->real());
    assert_equal((Object*)object, obj);
  }

  void test_root_send( void ) {
    Root root;
    std::string string;
    DummyCommand *cmd = root.adopt_command(new DummyCommand(&string));
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
    std::string logger;
    DummyCommand cmd(&logger);
    Location remote("oscit", "my place");
    Location remote2("oscit", "my place2");
    RootProxy *proxy = cmd.adopt_proxy(new RootProxy(remote));
    RootProxy *found = cmd.find_proxy(remote2);
    assert_equal((RootProxy *)NULL, found);
    found = cmd.find_proxy(remote);
    assert_equal(proxy, found);
  }

  void test_should_not_find_proxy_from_location_after_proxy_deletion( void ) {
    std::string logger;
    DummyCommand cmd(&logger);
    Location remote("oscit", "my place");
    RootProxy *proxy = cmd.adopt_proxy(new RootProxy(remote));
    RootProxy *found;

    delete proxy;
    found = cmd.find_proxy(remote);
    assert_equal((RootProxy *)NULL, found);
  }
};
