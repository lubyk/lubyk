// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "object.h"
#include "test_helper.h"

class DummyObject : public Object
{
public:
  DummyObject(const char * pName, int pCounter) : Object(pName), mCounter(pCounter) {}
  
  virtual const Value inspect(const Value& val)
  {
    std::ostringstream os(std::ostringstream::out);
    os << mName << ": " << mCounter;
    return String(os.str());
  }
private:
  int mCounter;
};

class ObjectTest : public ValueTestHelper
{
public:
  void test_root( void )
  {
    Root root;
    Object * a  = new Object("a");
    Object * a2 = new Object("a");
    Object * res;
    
    TS_ASSERT( root.get(&res, "") );
    TS_ASSERT_EQUALS( res, &root );
    
    TS_ASSERT( !root.get(&res, "/a") );
    
    root.adopt(a);
    TS_ASSERT( root.get(&res, "/a") );
    TS_ASSERT_EQUALS( res, a );
    
    root.adopt(a2);
    TS_ASSERT( root.get(&res, "/a") );
    TS_ASSERT_EQUALS( res, a );
    
    TS_ASSERT( root.get(&res, "/a-1") );
    TS_ASSERT_EQUALS( res, a2 );
    
    root.clear();
    TS_ASSERT( !root.get(&res, "/a") );
    TS_ASSERT( !root.get(&res, "/a-1") );
    TS_ASSERT( root.get(&res, "") );
    TS_ASSERT_EQUALS( res, &root );
  }
  
  void test_adopt( void )
  {
    Root root;
    Object * one = root.adopt(new Object("one")); // This is the prefered way of creating sub-objects.
    Object * sub = one->adopt(new Object("sub"));
  
    TS_ASSERT_EQUALS( root.url(),     std::string("") );
    TS_ASSERT_EQUALS( one->url(),     std::string("/one") );
    TS_ASSERT_EQUALS( sub->url(),     std::string("/one/sub") );
    
    TS_ASSERT_EQUALS( root.child("one"), one);
    TS_ASSERT_EQUALS( root.child("foo"), (Object*)NULL);
    
    one->set_name("foo");
    TS_ASSERT_EQUALS( root.child("one"), (Object*)NULL);
    TS_ASSERT_EQUALS( root.child("foo"), one);
    TS_ASSERT_EQUALS( root.find("/one/sub"), (Object*)NULL);
    TS_ASSERT_EQUALS( root.find("/foo/sub"), sub);
  }
  
  void test_first_child( void )
  {
    Root root;
    Object * one = root.adopt(new Object("one"));
    Object * two = root.adopt(new Object("aaa"));
    
    TS_ASSERT_EQUALS( root.first_child(), one);
    
    delete one;
    TS_ASSERT_EQUALS( root.first_child(), two);
  }
  
  void test_set_parent( void )
  {
    Root root;
    Object * child1 = new Object("foo");
    Object * child2 = new Object("bar");
    Object * child3 = new Object("foo");
    
    TS_ASSERT_EQUALS( root.url(),     std::string("") );
    TS_ASSERT_EQUALS( child1->url(),  std::string("foo") );
    TS_ASSERT_EQUALS( child2->url(),  std::string("bar") );
    TS_ASSERT_EQUALS( child3->url(),  std::string("foo") );
    
    child1->set_parent(root);
    
    TS_ASSERT_EQUALS( root.url(),     std::string("") );
    TS_ASSERT_EQUALS( child1->url(),  std::string("/foo") );
    TS_ASSERT_EQUALS( child2->url(),  std::string("bar") );
    TS_ASSERT_EQUALS( child3->url(),  std::string("foo") );
    
    child2->set_parent(child1);
    
    TS_ASSERT_EQUALS( root.url(),     std::string("") );
    TS_ASSERT_EQUALS( child1->url(),  std::string("/foo") );
    TS_ASSERT_EQUALS( child2->url(),  std::string("/foo/bar") );
    TS_ASSERT_EQUALS( child3->url(),  std::string("foo") );
    
    child3->set_parent(root);
    
    TS_ASSERT_EQUALS( root.url(),     std::string("") );
    TS_ASSERT_EQUALS( child1->url(),  std::string("/foo") );
    TS_ASSERT_EQUALS( child2->url(),  std::string("/foo/bar") );
    TS_ASSERT_EQUALS( child3->url(),  std::string("/foo-1") );
  }
  
  void test_rename( void )
  {
    Root root;
    Object * foo = root.adopt(new Object("foo"));
    Object * bar = foo->adopt(new Object("bar"));
    
    TS_ASSERT_EQUALS( root.url(),    std::string("") );
    TS_ASSERT_EQUALS( foo->url(),     std::string("/foo") );
    TS_ASSERT_EQUALS( bar->url(),     std::string("/foo/bar") );
    
    foo->set_name("super");
    
    TS_ASSERT_EQUALS( root.url(),    std::string("") );
    TS_ASSERT_EQUALS( foo->url(),     std::string("/super") );
    TS_ASSERT_EQUALS( bar->url(),     std::string("/super/bar") );
  }
  
  void test_get_object( void )
  {
    Root root;
    Object * foo = root.adopt(new Object("foo"));
    Object * bar = foo->adopt(new Object("bar"));
    Object * res;
  
    TS_ASSERT_EQUALS( root.url(),     std::string("") );
    TS_ASSERT_EQUALS( foo->url(),     std::string("/foo") );
    TS_ASSERT_EQUALS( bar->url(),     std::string("/foo/bar") );
  
    TS_ASSERT(root.get(&res, ""));
    TS_ASSERT_EQUALS( res, &root );
    
    TS_ASSERT(root.get(&res, "/foo"));
    TS_ASSERT_EQUALS( res, foo );
    
    TS_ASSERT(root.get(&res, "/foo/bar"));
    TS_ASSERT_EQUALS( res, bar );
    
    
    TS_ASSERT( ! root.get(&res, "/super"));
    TS_ASSERT( ! root.get(&res, "/super/bar"));
    
    foo->set_name("super");
    
    TS_ASSERT(root.get(&res, "/super"));
    TS_ASSERT_EQUALS( res, foo );
    
    TS_ASSERT(root.get(&res, "/super/bar"));
    TS_ASSERT_EQUALS( res, bar );
    
    TS_ASSERT( ! root.get(&res, "/foo"));
    TS_ASSERT( ! root.get(&res, "/foo/bar"));
  }
  
  void test_call( void )
  {
    Root root;
    Object * one = root.adopt(new Object("one"));
    Object * two = root.adopt(new Object("two"));
    Object * sub = two->adopt(new Object("sub"));
    Value res, param;
    String str;
  
    TS_ASSERT_EQUALS( root.url(),    std::string("") );
    TS_ASSERT_EQUALS( one->url(),     std::string("/one") );
    TS_ASSERT_EQUALS( two->url(),     std::string("/two") );
    TS_ASSERT_EQUALS( sub->url(),     std::string("/two/sub") );
    
    res = root.call("",param);
    TS_ASSERT(!res.is_error());
    TS_ASSERT(res.set(str));
    TS_ASSERT( str == "one,two/" );
    
    res = root.call("/one",param);
    TS_ASSERT(res.is_nil());
    
    res = root.call("/two",param);
    TS_ASSERT(!res.is_error());
    TS_ASSERT(res.set(str));
    TS_ASSERT( str == "sub" );
  }
  
  void test_child( void )
  {
    Root root;
    Object * one = root.adopt(new Object("one"));
    Object * sub = one->adopt(new Object("sub"));
  
    TS_ASSERT_EQUALS( root.url(),    std::string("") );
    TS_ASSERT_EQUALS( one->url(),     std::string("/one") );
    TS_ASSERT_EQUALS( sub->url(),     std::string("/one/sub") );
    
    TS_ASSERT_EQUALS( root.child("one"), one);
    TS_ASSERT_EQUALS( root.child("foo"), (Object*)NULL);
    
    one->set_name("foo");
    TS_ASSERT_EQUALS( root.child("one"), (Object*)NULL);
    TS_ASSERT_EQUALS( root.child("foo"), one);
    TS_ASSERT_EQUALS( root.find("/one/sub"), (Object*)NULL);
    TS_ASSERT_EQUALS( root.find("/foo/sub"), sub);
  }
  
  void test_call_bad_object( void )
  {
    Root root;
    Value res, param;
    
    Number n1(4);
    assert_id(n1, 1);
    assert_id(res, 0);
    
    res = root.call("/foo",param);
    assert_id(res, 2);
    
    TS_ASSERT(res.is_error());
    TS_ASSERT_EQUALS( Error(res).message(), std::string("Object '/foo' not found.") );
  }
  
  void test_get_info( void )
  {
    Root root;
    Object no_info(root,"foo");
    Value res, param;
    
    root.set_info("This is the root node.");
    res = root.call("/#info",param);
    TS_ASSERT_EQUALS( res.to_string(), "[1] This is the root node.");
    res = root.call("/foo/#info",param);
    TS_ASSERT_EQUALS( res.to_string(), "[2] ");
    
    res = root.call("/blah/#info",param);
    TS_ASSERT_EQUALS( Error(res).message(), std::string("Object '/blah/#info' not found.") );
  }
  
  void test_inspect( void )
  {
    Root root;
    root.adopt(new DummyObject("foo", 23));
    Value res;
    
    root.set_info("This is the root node.");
    res = root.call("/#inspect");
    TS_ASSERT_EQUALS( res.to_string(), "[1] This is the root node.");
    res = root.call("/foo/#inspect");
    TS_ASSERT_EQUALS( res.to_string(), "[2] foo: 23");
    
    res = root.call("/blah/#inspect");
    TS_ASSERT_EQUALS( Error(res).message(), std::string("Object '/blah/#inspect' not found.") );
  }
};