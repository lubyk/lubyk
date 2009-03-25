#include "test_helper.h"
#include "oscit/root.h"

class ObjectTest : public TestHelper
{
public:
  void testCreate( void )
  {
    DummyObject a("a", 1);
    assertEqual("a", a.getName());
  }
  
  void testRoot( void )
  {
    oscit::Root root;
    DummyObject * a  = new DummyObject("a", 1);
    DummyObject * a2 = new DummyObject("a", 2);
    oscit::Object * res;
    
    assertTrue( root.get(&res, "") );
    assertEqual( &root, res );
    
    assertFalse( root.get(&res, "/a") );
    
    root.adopt(a);
    assertTrue( root.get(&res, "/a") );
    assertEqual( a, res );
    
    root.adopt(a2);
    assertTrue( root.get(&res, "/a") );
    assertEqual( a, res );
    
    assertTrue( root.get(&res, "/a-1") );
    assertEqual( a2, res );
    
    root.clear();
    assertTrue( !root.get(&res, "/a") );
    assertTrue( !root.get(&res, "/a-1") );
    assertTrue( root.get(&res, "") );
    assertEqual( &root, res );
  }
  
  void testAdopt( void )
  {
    oscit::Root root;
    oscit::Object * one = root.adopt(new oscit::Object("one")); // This is the prefered way of creating sub-objects.
    oscit::Object * sub = one->adopt(new oscit::Object("sub"));
  
    assertEqual( root.url(),     std::string("") );
    assertEqual( one->url(),     std::string("/one") );
    assertEqual( sub->url(),     std::string("/one/sub") );
    
    assertEqual( root.child("one"), one);
    assertEqual( root.child("foo"), (oscit::Object*)NULL);
    
    one->setName("foo");
    assertEqual( root.child("one"), (oscit::Object*)NULL);
    assertEqual( root.child("foo"), one);
    assertEqual( root.find("/one/sub"), (oscit::Object*)NULL);
    assertEqual( root.find("/foo/sub"), sub);
  }
  
  void testGetFirstChild( void )
  {
    oscit::Root root;
    oscit::Object * one = root.adopt(new oscit::Object("one"));
    oscit::Object * two = root.adopt(new oscit::Object("aaa"));
    
    assertEqual( one, root.getFirstChild());
    
    delete one;
    assertEqual( two, root.getFirstChild());
  }
  
  void testSetParent( void )
  {
    oscit::Root root;
    oscit::Object * child1 = new oscit::Object("foo");
    oscit::Object * child2 = new oscit::Object("bar");
    oscit::Object * child3 = new oscit::Object("foo");
    
    assertEqual( std::string("")   , root.url()    );
    assertEqual( std::string("foo"), child1->url() );
    assertEqual( std::string("bar"), child2->url() );
    assertEqual( std::string("foo"), child3->url() );
    
    child1->setParent(root);
    
    assertEqual( std::string("")    , root.url()    );
    assertEqual( std::string("/foo"), child1->url() );
    assertEqual( std::string("bar") , child2->url() );
    assertEqual( std::string("foo") , child3->url() );
    
    child2->setParent(child1);
    
    assertEqual( std::string("")        , root.url()    );
    assertEqual( std::string("/foo")    , child1->url() );
    assertEqual( std::string("/foo/bar"), child2->url() );
    assertEqual( std::string("foo")     , child3->url() );
    
    child3->setParent(root);
    
    assertEqual( std::string("")        , root.url()    );
    assertEqual( std::string("/foo")    , child1->url() );
    assertEqual( std::string("/foo/bar"), child2->url() );
    assertEqual( std::string("/foo-1")  , child3->url() );
  }
  
  void testRename( void )
  {
    oscit::Root root;
    oscit::Object * foo = root.adopt(new oscit::Object("foo"));
    oscit::Object * bar = foo->adopt(new oscit::Object("bar"));
    
    assertEqual( std::string("")        , root.url() );
    assertEqual( std::string("/foo")    , foo->url() );
    assertEqual( std::string("/foo/bar"), bar->url() );
    
    foo->setName("super");
    
    assertEqual( std::string("")          , root.url() );
    assertEqual( std::string("/super")    , foo->url() );
    assertEqual( std::string("/super/bar"), bar->url() );
  }
  
  void testGetObject( void )
  {
    oscit::Root root;
    oscit::Object * foo = root.adopt(new oscit::Object("foo"));
    oscit::Object * bar = foo->adopt(new oscit::Object("bar"));
    oscit::Object * res;
  
    assertEqual( std::string("")        , root.url() );
    assertEqual( std::string("/foo")    , foo->url() );
    assertEqual( std::string("/foo/bar"), bar->url() );
  
    assertTrue(root.get(&res, ""));
    assertEqual( &root, res );
    
    assertTrue(root.get(&res, "/foo"));
    assertEqual( foo, res );
    
    assertTrue(root.get(&res, "/foo/bar"));
    assertEqual( bar, res );
    
    
    assertFalse( root.get(&res, "/super"));
    assertFalse( root.get(&res, "/super/bar"));
    
    foo->setName("super");
    
    assertTrue(root.get(&res, "/super"));
    assertEqual( foo, res );
    
    assertTrue(root.get(&res, "/super/bar"));
    assertEqual( bar, res );
    
    assertFalse( root.get(&res, "/foo"));
    assertFalse( root.get(&res, "/foo/bar"));
  }
  
//  void testCall( void )
//  {
//    oscit::Root root;
//    oscit::Object * one = root.adopt(new oscit::Object("one"));
//    oscit::Object * two = root.adopt(new oscit::Object("two"));
//    oscit::Object * sub = two->adopt(new oscit::Object("sub"));
//    Value res, param;
//    String str;
//  
//    assertEqual( std::string("")        , root.url() );
//    assertEqual( std::string("/one")    , one->url() );
//    assertEqual( std::string("/two")    , two->url() );
//    assertEqual( std::string("/two/sub"), sub->url() );
//    
//    res = root.call("",param);
//    assertTrue(!res.is_error());
//    assertTrue(res.set(str));
//    assertTrue( str == "one,two/" );
//    
//    res = root.call("/one",param);
//    assertTrue(res.is_nil());
//    
//    res = root.call("/two",param);
//    assertFalse(res.is_error());
//    assertTrue(res.set(str));
//    assertTrue( str == "sub" );
//  }
  
//  
//  void test_child( void )
//  {
//    oscit::Root root;
//    oscit::Object * one = root.adopt(new oscit::Object("one"));
//    oscit::Object * sub = one->adopt(new oscit::Object("sub"));
//  
//    assertEqual( root.url(),    std::string("") );
//    assertEqual( one->url(),     std::string("/one") );
//    assertEqual( sub->url(),     std::string("/one/sub") );
//    
//    assertEqual( root.child("one"), one);
//    assertEqual( root.child("foo"), (oscit::Object*)NULL);
//    
//    one->setName("foo");
//    assertEqual( root.child("one"), (oscit::Object*)NULL);
//    assertEqual( root.child("foo"), one);
//    assertEqual( root.find("/one/sub"), (oscit::Object*)NULL);
//    assertEqual( root.find("/foo/sub"), sub);
//  }
//  
//  void test_call_bad_object( void )
//  {
//    oscit::Root root;
//    Value res, param;
//    
//    Number n1(4);
//    assert_id(n1, 1);
//    assert_id(res, 0);
//    
//    res = root.call("/foo",param);
//    assert_id(res, 2);
//    
//    assertTrue(res.is_error());
//    assertEqual( Error(res).message(), std::string("Object '/foo' not found.") );
//  }
//  
//  void test_get_info( void )
//  {
//    oscit::Root root;
//    oscit::Object no_info(root,"foo");
//    Value res, param;
//    
//    root.setInfo("This is the root node.");
//    res = root.call("/.info","");
//    assertEqual( res.to_string(), "[1] This is the root node.");
//    res = root.call("/foo/#info",param);
//    assertEqual( res.to_string(), "[2] ");
//    
//    res = root.call("/blah/#info",param);
//    assertEqual( Error(res).message(), std::string("Object '/blah/#info' not found.") );
//  }
//  
//  void test_inspect( void )
//  {
//    oscit::Root root;
//    root.adopt(new DummyObject("foo", 23));
//    Value res;
//    
//    root.setInfo("This is the root node.");
//    res = root.call("/#inspect");
//    assertEqual( res.to_string(), "[1] This is the root node.");
//    res = root.call("/foo/#inspect");
//    assertEqual( res.to_string(), "[2] foo: 23");
//    
//    res = root.call("/blah/#inspect");
//    assertEqual( Error(res).message(), std::string("Object '/blah/#inspect' not found.") );
//  }
};