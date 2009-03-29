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
    assert_equal(DUMMY_OBJECT_INFO, res.error_message());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());
  }
  
 void test_call_bad_url_should_return_missing_error( void ) {
   Root root;
   Value res = root.call("/foo");
   
   assert_true(res.is_error());
   assert_equal("/foo", res.error_message());
   assert_equal(NOT_FOUND_ERROR, res.error_code());
 }
 
//  void test_get_info( void )
//  {
//    Root root;
//    Object no_info(root,"foo");
//    Value res, param;
//    
//    root.set_info("This is the root node.");
//    res = root.call("/.info","");
//    assert_equal(res.to_string(), "[1] This is the root node.");
//    res = root.call("/foo/#info",param);
//    assert_equal(res.to_string(), "[2] ");
//    
//    res = root.call("/blah/#info",param);
//    assert_equal(Error(res).message(), std::string("Object '/blah/#info' not found.") );
//  }
//  
//  void test_inspect( void )
//  {
//    Root root;
//    root.adopt(new DummyObject("foo", 23));
//    Value res;
//    
//    root.set_info("This is the root node.");
//    res = root.call("/#inspect");
//    assert_equal(res.to_string(), "[1] This is the root node.");
//    res = root.call("/foo/#inspect");
//    assert_equal(res.to_string(), "[2] foo: 23");
//    
//    res = root.call("/blah/#inspect");
//    assert_equal(Error(res).message(), std::string("Object '/blah/#inspect' not found.") );
//  }
};