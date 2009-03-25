#include "test_helper.h"
#include "oscit/root.h"
#include "mock/dummy_object.h"

class ObjectTest : public TestHelper
{
public:
  void test_create( void )
  {
    DummyObject a("a", 1);
    assert_equal("a", a.name());
  }
  
  void test_root( void )
  {
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
  
  void test_adopt( void )
  {
    Root root;
    Object * one = root.adopt(new Object("one")); // This is the prefered way of creating sub-objects.
    Object * sub = one->adopt(new Object("sub"));
  
    assert_equal(root.url(),     std::string("") );
    assert_equal(one->url(),     std::string("/one") );
    assert_equal(sub->url(),     std::string("/one/sub") );
    
    assert_equal(root.child("one"), one);
    assert_equal(root.child("foo"), (Object*)NULL);
    
    one->set_name("foo");
    assert_equal(root.child("one"), (Object*)NULL);
    assert_equal(root.child("foo"), one);
    assert_equal(root.object_at("/one/sub"), (Object*)NULL);
    assert_equal(root.object_at("/foo/sub"), sub);
  }
  
  void test_first_child( void )
  {
    Root root;
    Object * one = root.adopt(new Object("one"));
    Object * two = root.adopt(new Object("aaa"));
    
    assert_equal(one, root.first_child());
    
    delete one;
    assert_equal(two, root.first_child());
  }
  
  void test_set_parent( void )
  {
    Root root;
    Object * child1 = new Object("foo");
    Object * child2 = new Object("bar");
    Object * child3 = new Object("foo");
    
    assert_equal(std::string("")   , root.url()    );
    assert_equal(std::string("foo"), child1->url() );
    assert_equal(std::string("bar"), child2->url() );
    assert_equal(std::string("foo"), child3->url() );
    
    child1->set_parent(root);
    
    assert_equal(std::string("")    , root.url()    );
    assert_equal(std::string("/foo"), child1->url() );
    assert_equal(std::string("bar") , child2->url() );
    assert_equal(std::string("foo") , child3->url() );
    
    child2->set_parent(child1);
    
    assert_equal(std::string("")        , root.url()    );
    assert_equal(std::string("/foo")    , child1->url() );
    assert_equal(std::string("/foo/bar"), child2->url() );
    assert_equal(std::string("foo")     , child3->url() );
    
    child3->set_parent(root);
    
    assert_equal(std::string("")        , root.url()    );
    assert_equal(std::string("/foo")    , child1->url() );
    assert_equal(std::string("/foo/bar"), child2->url() );
    assert_equal(std::string("/foo-1")  , child3->url() );
  }
  
  void test_rename( void )
  {
    Root root;
    Object * foo = root.adopt(new Object("foo"));
    Object * bar = foo->adopt(new Object("bar"));
    
    assert_equal(std::string("")        , root.url() );
    assert_equal(std::string("/foo")    , foo->url() );
    assert_equal(std::string("/foo/bar"), bar->url() );
    
    foo->set_name("super");
    
    assert_equal(std::string("")          , root.url() );
    assert_equal(std::string("/super")    , foo->url() );
    assert_equal(std::string("/super/bar"), bar->url() );
  }
  
  void test_get( void )
  {
    Root root;
    Object * foo = root.adopt(new Object("foo"));
    Object * bar = foo->adopt(new Object("bar"));
    Object * res;
  
    assert_equal(std::string("")        , root.url() );
    assert_equal(std::string("/foo")    , foo->url() );
    assert_equal(std::string("/foo/bar"), bar->url() );
  
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
  
//  void testCall( void )
//  {
//    Root root;
//    Object * one = root.adopt(new Object("one"));
//    Object * two = root.adopt(new Object("two"));
//    Object * sub = two->adopt(new Object("sub"));
//    Value res, param;
//    String str;
//  
//    assert_equal(std::string("")        , root.url() );
//    assert_equal(std::string("/one")    , one->url() );
//    assert_equal(std::string("/two")    , two->url() );
//    assert_equal(std::string("/two/sub"), sub->url() );
//    
//    res = root.call("",param);
//    assert_true(!res.is_error());
//    assert_true(res.set(str));
//    assert_true( str == "one,two/" );
//    
//    res = root.call("/one",param);
//    assert_true(res.is_nil());
//    
//    res = root.call("/two",param);
//    assert_false(res.is_error());
//    assert_true(res.set(str));
//    assert_true( str == "sub" );
//  }
  
//  
//  void test_child( void )
//  {
//    Root root;
//    Object * one = root.adopt(new Object("one"));
//    Object * sub = one->adopt(new Object("sub"));
//  
//    assert_equal(root.url(),    std::string("") );
//    assert_equal(one->url(),     std::string("/one") );
//    assert_equal(sub->url(),     std::string("/one/sub") );
//    
//    assert_equal(root.child("one"), one);
//    assert_equal(root.child("foo"), (Object*)NULL);
//    
//    one->set_name("foo");
//    assert_equal(root.child("one"), (Object*)NULL);
//    assert_equal(root.child("foo"), one);
//    assert_equal(root.object_at("/one/sub"), (Object*)NULL);
//    assert_equal(root.object_at("/foo/sub"), sub);
//  }
//  
//  void test_call_bad_object( void )
//  {
//    Root root;
//    Value res, param;
//    
//    Number n1(4);
//    assert_id(n1, 1);
//    assert_id(res, 0);
//    
//    res = root.call("/foo",param);
//    assert_id(res, 2);
//    
//    assert_true(res.is_error());
//    assert_equal(Error(res).message(), std::string("Object '/foo' not found.") );
//  }
//  
//  void test_get_info( void )
//  {
//    Root root;
//    Object no_info(root,"foo");
//    Value res, param;
//    
//    root.setInfo("This is the root node.");
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
//    root.setInfo("This is the root node.");
//    res = root.call("/#inspect");
//    assert_equal(res.to_string(), "[1] This is the root node.");
//    res = root.call("/foo/#inspect");
//    assert_equal(res.to_string(), "[2] foo: 23");
//    
//    res = root.call("/blah/#inspect");
//    assert_equal(Error(res).message(), std::string("Object '/blah/#inspect' not found.") );
//  }
};