#include "test_helper.h"
#include "oscit/root.h"
#include "mock/dummy_object.h"

class RootTest : public TestHelper
{
public:
  void test_create( void ) {
    Root r;
    assert_equal("", r.name());
    assert_equal("", r.url());
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
  
  void test_adopt_command( void ) {
    Root root;
    std::string string;
    DummyCommand * cmd = root.adopt_command(new DummyCommand(&string));
    microsleep(15);
    assert_equal("..", string);
    delete cmd;
    string = "";
    cmd = root.adopt_command(new DummyCommand(&string), false); // do not start
    microsleep(15);
    assert_equal("", string);
    delete cmd; // should not lock
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
    assert_true(error.is_nil());
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
    Object * res = root.object_at(Url("some://example.com /foo"), &error);
    assert_equal((Object*)NULL, res);
    assert_equal(BAD_REQUEST_ERROR, error.error_code());
    assert_equal("Could not parse url \'some://example.com /foo\'.", error.error_message());
  }
  
  // remote objects and 'send' testing is done in command_test.h
};