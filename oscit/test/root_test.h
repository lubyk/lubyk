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
    BaseObject * foo = root.adopt(new BaseObject("foo"));
    BaseObject * bar = foo->adopt(new BaseObject("bar"));
    BaseObject * res;
  
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
    BaseObject * res;
    
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
    
    assert_equal((BaseObject*)NULL, root.find_or_build_object_at("whatever", &error));
    assert_equal((BaseObject*)NULL, root.find_or_build_object_at("/whatever", &error));
    assert_equal((BaseObject*)NULL, root.find_or_build_object_at("/dummy/foo", &error));
    assert_true(error.is_error());
    assert_equal(NOT_FOUND_ERROR, error.error_code());
    assert_equal((BaseObject*)NULL, root.find_or_build_object_at("/dummy/error", &error));
    assert_true(error.is_error());
    assert_equal("You should not try to build errors !", error.error_message());
    assert_equal(INTERNAL_SERVER_ERROR, error.error_code());
    
    BaseObject * special = root.find_or_build_object_at("/dummy/special", &error);
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
  
  ////////////////////// OSCIT META METHODS TESTS ///////////////////////////////////////
  
  void test_info( void ) {
    Root root;
    root.adopt(new BaseObject("foo"));
    Value res;

    root.set_info("This is the root node.");
    res = root.call("/.info", Value(""));
    assert_true(res.is_string());
    assert_equal("This is the root node.", res.str());
    
    res = root.call("/.info", Value("/foo"));
    assert_true(res.is_string());
    assert_equal(DEFAULT_INFO, res.str());

    res = root.call("/.info", Value("/blah"));
    assert_true(res.is_error());
    assert_equal(NOT_FOUND_ERROR, res.error_code());
  }
  
  void test_slider_type( void ) {
    Root root;
    root.adopt(new DummyObject("foo", 4.25));
    Value res;

    root.set_info("This is the root node.");
    res = root.call("/.type", Value(""));
    assert_true(res.is_nil());
    
    res = root.call("/.type", Value("/foo"));
    assert_true(res.is_list());
    assert_equal("fffs", res.type_tag());
    assert_equal(4.25,  res[0].r); // current
    assert_equal(0.0,   res[1].r); // min
    assert_equal(127.0, res[2].r); // max
    assert_equal("lux", res[3].str()); // unit

    res = root.call("/.type", Value("/blah"));
    assert_true(res.is_error());
    assert_equal(NOT_FOUND_ERROR, res.error_code());
  }
  
  void test_menu_type( void ) {
    Root root;
    Dummy2BaseObject * foo = root.adopt(new Dummy2BaseObject("foo", "yuv"));
    Value res;
    
    // set wrong current to make sure current value is retrieved
    foo->set_type("", "rgb,rgba,yuv", "color mode");

    res = root.call("/.type", Value("/foo"));
    assert_true(res.is_list());
    assert_equal("sss", res.type_tag());
    assert_equal("yuv",  res[0].str()); // current
    assert_equal("rgb,rgba,yuv",  res[1].str()); // current
    assert_equal("color mode", res[2].str()); // unit
  }
  
  void test_any_real_type( void ) {
    Root root;
    DummyObject * foo = root.adopt(new DummyObject("foo", 1.23));
    Value res;
    
    // set wrong current to make sure current value is retrieved
    foo->set_type(1.99,"Hz");
    
    res = root.call("/.type", Value("/foo"));
    assert_true(res.is_list());
    assert_equal("fs", res.type_tag());
    assert_equal(1.23,  res[0].r); // current
    assert_equal("Hz", res[1].str()); // unit
  }
};