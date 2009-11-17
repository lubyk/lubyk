#include "test_helper.h"
#include "oscit/root.h"
#include "mock/dummy_object.h"
#include "mock/dummy_command.h"
#include "mock/object_logger.h"
#include "mock/command_logger.h"

class RootTest : public TestHelper
{
public:
  void test_create( void ) {
    Root r;
    assert_equal("", r.name());
    assert_equal("", r.url());
  }

  void test_url( void ) {
    Root r("funky");
    assert_equal("funky", r.name());
    assert_equal("", r.url());
    Object *lala = r.adopt(new Object("lala"));
    assert_equal("/lala", lala->url());
  }

  void test_get_object_at( void ) {
    Root root;
    Object * foo = root.adopt(new Object("foo"));
    Object * bar = foo->adopt(new Object("bar"));
    Object * res;

    assert_equal(""        , root.url() );
    assert_equal("/foo"    , foo->url() );
    assert_equal("/foo/bar", bar->url() );

    assert_true(root.get_object_at("", &res));
    assert_equal(&root, res );

    assert_true(root.get_object_at("/foo", &res));
    assert_equal(foo, res );

    assert_true(root.get_object_at("/foo/bar", &res));
    assert_equal(bar, res );


    assert_false( root.get_object_at("/super", &res));
    assert_false( root.get_object_at("/super/bar", &res));

    foo->set_name("super");

    assert_true(root.get_object_at("/super", &res));
    assert_equal(foo, res );

    assert_true(root.get_object_at("/super/bar", &res));
    assert_equal(bar, res );

    assert_false( root.get_object_at("/foo", &res));
    assert_false( root.get_object_at("/foo/bar", &res));
  }

  void test_get_object_at_same_name_as_sibling( void ) {
    Root root;
    DummyObject * a  = new DummyObject("a", 1);
    DummyObject * a2 = new DummyObject("a", 2);
    Object * res;

    assert_true( root.get_object_at("", &res) );
    assert_equal(&root, res );

    assert_false( root.get_object_at("/a", &res) );

    root.adopt(a);

    assert_true( root.get_object_at("/a", &res) );
    assert_equal(a, res );

    root.adopt(a2);
    assert_true( root.get_object_at("/a", &res) );
    assert_equal(a, res );

    assert_true( root.get_object_at("/a-1", &res) );
    assert_equal(a2, res );

    root.clear();
    assert_true( !root.get_object_at("/a", &res) );
    assert_true( !root.get_object_at("/a-1", &res) );
    assert_true( root.get_object_at("", &res) );
    assert_equal(&root, res );
  }

  void test_call_without_arguments_should_return_current_value( void ) {
    Root root;
    root.adopt(new DummyObject("zorglub", 9.87));
    Value res = root.call("/zorglub");
    assert_true(res.is_real());
    assert_equal(9.87, res.r);
  }

  void test_call_with_nil_should_return_current_value( void ) {
    Root root;
    root.adopt(new DummyObject("zorglub", 9.87));
    Value res = root.call("/zorglub", gNilValue);
    assert_true(res.is_real());
    assert_equal(9.87, res.r);
  }

  void test_call_with_empty_should_return_current_value( void ) {
    Root root;
    root.adopt(new DummyObject("zorglub", 9.87));
    Value res = root.call("/zorglub", Value());
    assert_true(res.is_real());
    assert_equal(9.87, res.r);
  }

  void test_call_with_bad_arguments_should_return_bad_request_error( void ) {
    Root root;
    root.adopt(new DummyObject("zorglub", 9.87));
    Value param(TypeTag("ff"));
    Value res = root.call("/zorglub", param);
    assert_true(res.is_error());
    assert_equal(std::string("'/zorglub' (").append(DUMMY_OBJECT_INFO).append(")."), res.error_message());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());
  }

  void test_call_bad_url_should_return_missing_error( void ) {
    Root root;
    Value res = root.call("/foo");

    assert_true(res.is_error());
    assert_equal("/foo", res.error_message());
    assert_equal(NOT_FOUND_ERROR, res.error_code());
  }

  void test_call_should_route_message_to_object( void ) {
    Root root;
    DummyObject *dummy = root.adopt(new DummyObject("dummy", 0.0));
    Value res = root.call("/dummy", Value(4.5));

    assert_false(res.is_error());
    assert_equal(4.5, dummy->real());
  }

  void test_find_or_build_object_at_should_call_build_child( void ) {
    Root root;
    root.adopt(new DummyObject("dummy", 0.0));
    Value error;

    assert_equal((Object*)NULL, root.find_or_build_object_at("whatever", &error));
    assert_equal((Object*)NULL, root.find_or_build_object_at("/whatever", &error));
    assert_equal((Object*)NULL, root.find_or_build_object_at("/dummy/foo", &error));
    assert_true(error.is_error());
    assert_equal(NOT_FOUND_ERROR, error.error_code());
    assert_equal((Object*)NULL, root.find_or_build_object_at("/dummy/error", &error));
    assert_true(error.is_error());
    assert_equal("You should not try to build errors !", error.error_message());
    assert_equal(INTERNAL_SERVER_ERROR, error.error_code());

    Object * special = root.find_or_build_object_at("/dummy/special", &error);
    assert_true( special != NULL );
    assert_equal("/dummy/special", special->url());
  }

  void test_call_should_build_child( void ) {
    Root root;
    root.adopt(new DummyObject("builder", 0.0));
    Value res = root.call("/builder/AgeOf/Capitain"); // dummy builds 'AgeOf' and 'Capitain'
    assert_true(res.is_real());
    assert_equal(78.0, res.r);
  }

  void test_adopt_command_should_start_listeners( void ) {
    Root root;
    Logger logger;
    root.adopt_command(new CommandLogger("osc", &logger));
    assert_equal("[osc: listen]", logger.str());
  }

  void test_adopt_command_with_false_should_not_start_listeners( void ) {
    Root root;
    Logger logger;
    root.adopt_command(new CommandLogger("osc", &logger), false);
    assert_equal("", logger.str());
  }

  void test_remote_object_at( void ) {
    Root root;
    std::string string;
    root.adopt_command(new DummyCommand(&string));
    DummyObject  *foo = root.adopt(new DummyObject("foo", 3));
    Object   *res;
    Value error;
    res = root.object_at(Url("/foo"), &error);
    assert_equal((Object*)foo, res);
    assert_true(error.is_empty());
  }

  void test_object_at_bad_protocol( void ) {
    Root root;
    Value error;
    Object * res = root.object_at(Url("some://example.com/foo"), &error);
    assert_equal((Object*)NULL, res);
    assert_equal(BAD_REQUEST_ERROR, error.error_code());
    assert_equal("No command to handle \'some\' protocol.", error.error_message());
  }

  void test_object_at_bad_url( void ) {
    Root root;
    Value error;
    Object *obj = root.object_at(Url("some://example.com /foo"), &error);
    assert_equal((Object*)NULL, obj);
    assert_equal(BAD_REQUEST_ERROR, error.error_code());
    assert_equal("Could not parse url \'some://example.com /foo\'.", error.error_message());
  }

  void test_send_should_route_messages_depending_on_protocol( void ) {
    Root root;
    Logger logger;
    root.adopt_command(new CommandLogger("http", &logger),false);
    root.adopt_command(new CommandLogger("osc", &logger),false);
    logger.str("");
    root.send(Url("http://example.com/foo/bar"), gNilValue);
    assert_equal("[http: http://example.com/foo/bar]", logger.str());
    root.send(Url("osc://\"funky thing\"/one/two"), gNilValue);
    assert_equal("[http: http://example.com/foo/bar][osc: osc://\"funky thing\"/one/two]", logger.str());
  }

  void test_should_only_register_one_command_per_protocol( void ) {
    Root root;
    Logger logger;
    root.adopt_command(new CommandLogger("one", "osc", &logger),false);
    CommandLogger *two = root.adopt_command(new CommandLogger("two", "osc", &logger),false);
    assert_equal(NULL, (void*)two);
    assert_equal("", logger.str());
    logger.str("");
    root.send(Url("osc://example.com/foo/bar"), gNilValue);
    assert_equal("[one: osc://example.com/foo/bar]", logger.str());
  }

  void test_named_root( void ) {
    Root root("gaia");
    Object *obj = root.object_at("");
    assert_equal((Object*)&root, obj);
  }

  // Is this really what we want ? Shouldn't we notify always ?
  void test_call_without_context_should_not_notify( void ) {
    Root root;
    Logger logger;
    root.adopt_command(new CommandLogger("osc", &logger));
    root.adopt(new DummyObject("foo", 4.5));
    logger.str("");
    // call without context should not notify
    Value res = root.call("/foo", Value(5.2));
    assert_equal("", logger.str());
  }

  void test_call_with_context_should_notify_observers( void ) {
    Root root;
    Mutex context;
    root.adopt(new DummyObject("foo", 4.5));
    Logger logger;
    root.adopt_command(new CommandLogger("http", &logger));
    root.adopt_command(new CommandLogger("osc", &logger));
    logger.str("");
    root.call("/foo", Value(5.2), &context);
    assert_equal("[http: /foo 5.2][osc: /foo 5.2]", logger.str());
  }

  void test_delete_command_should_unregister_it_from_observers( void ) {
    Root root;
    Mutex context;
    root.adopt(new DummyObject("foo", 4.5));
    Logger logger;
    CommandLogger *cmd = root.adopt_command(new CommandLogger("osc", &logger));
    delete cmd;
    logger.str("");
    root.call("/foo", Value(5.2), &context);
  }

  void test_list_with_type_on_root( void ) {
    Root root;
    root.adopt(new DummyObject("mode", 0.0, SelectIO("rgb, yuv", "color mode", "This is a menu.")));
    root.adopt(new DummyObject("tint", 45.0, RangeIO(1, 127, "tint", "This is a slider from 1 to 127.")));
    Value res = root.list_with_type();
    // .error, .info, etc are ignored (current value -- type mismatch)
    assert_equal("[s[*s]][s[sss]][s[sss]][s[sss]][s[sss]][s[sss]][s[ssss]][s[fffss]]", res.type_tag());
    assert_equal(".error", res[0][0].str());
    assert_equal(".info", res[1][0].str());
  }

  void test_create_root_without_meta( void ) {
    Root root(false);
    Value res = root.list();
    assert_true(res.size() == 0);
  }

  void test_should_destroy_all_tree_on_delete( void ) {
    Root *root = new Root;
    Logger logger;
    root->adopt(new ObjectLogger("one", &logger));
    assert_equal("", logger.str());
    delete root;
    assert_equal("[one: destroyed]", logger.str());
  }
  // remote objects and 'send' testing is done in command_test.h
};
