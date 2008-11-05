// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "object.h"


class ObjectTest : public CxxTest::TestSuite
{
public:
  void test_set_parent( void )
  {
    Object root("root");
    Object child1("foo");
    Object child2("bar");
    Object child3("foo");
    
    TS_ASSERT_EQUALS( root.url(),    std::string("/root") );
    TS_ASSERT_EQUALS( child1.url(),  std::string("/foo") );
    TS_ASSERT_EQUALS( child2.url(),  std::string("/bar") );
    TS_ASSERT_EQUALS( child3.url(),  std::string("/foo") );
    
    child1.set_parent(root);
    
    TS_ASSERT_EQUALS( root.url(),    std::string("/root") );
    TS_ASSERT_EQUALS( child1.url(),  std::string("/root/foo") );
    TS_ASSERT_EQUALS( child2.url(),  std::string("/bar") );
    TS_ASSERT_EQUALS( child3.url(),  std::string("/foo") );
    
    child2.set_parent(child1);
    
    TS_ASSERT_EQUALS( root.url(),    std::string("/root") );
    TS_ASSERT_EQUALS( child1.url(),  std::string("/root/foo") );
    TS_ASSERT_EQUALS( child2.url(),  std::string("/root/foo/bar") );
    TS_ASSERT_EQUALS( child3.url(),  std::string("/foo") );
    
    child3.set_parent(root);
    
    TS_ASSERT_EQUALS( root.url(),    std::string("/root") );
    TS_ASSERT_EQUALS( child1.url(),  std::string("/root/foo") );
    TS_ASSERT_EQUALS( child2.url(),  std::string("/root/foo/bar") );
    TS_ASSERT_EQUALS( child3.url(),  std::string("/root/foo-1") );
    
  }
  
  void test_rename( void )
  {
    Object root("root");
    Object foo(root,"foo");
    Object bar(foo,"bar");
    
    TS_ASSERT_EQUALS( root.url(),    std::string("/root") );
    TS_ASSERT_EQUALS( foo.url(),     std::string("/root/foo") );
    TS_ASSERT_EQUALS( bar.url(),     std::string("/root/foo/bar") );
    
    foo.set_name("super");
    
    TS_ASSERT_EQUALS( root.url(),    std::string("/root") );
    TS_ASSERT_EQUALS( foo.url(),     std::string("/root/super") );
    TS_ASSERT_EQUALS( bar.url(),     std::string("/root/super/bar") );
  }
  
  void test_get_object( void )
  {
    Object root("root");
    Object foo(root,"foo");
    Object bar(foo,"bar");
    Object * res;

    TS_ASSERT_EQUALS( root.url(),    std::string("/root") );
    TS_ASSERT_EQUALS( foo.url(),     std::string("/root/foo") );
    TS_ASSERT_EQUALS( bar.url(),     std::string("/root/foo/bar") );

    TS_ASSERT(Object::get(&res, "/root"));
    TS_ASSERT_EQUALS( res, &root );
    
    TS_ASSERT(Object::get(&res, "/root/foo"));
    TS_ASSERT_EQUALS( res, &foo );
    
    TS_ASSERT(Object::get(&res, "/root/foo/bar"));
    TS_ASSERT_EQUALS( res, &bar );
    
    
    TS_ASSERT( ! Object::get(&res, "/root/super"));
    TS_ASSERT( ! Object::get(&res, "/root/super/bar"));
    
    foo.set_name("super");
    
    TS_ASSERT(Object::get(&res, "/root/super"));
    TS_ASSERT_EQUALS( res, &foo );
    
    TS_ASSERT(Object::get(&res, "/root/super/bar"));
    TS_ASSERT_EQUALS( res, &bar );
    
    TS_ASSERT( ! Object::get(&res, "/root/foo"));
    TS_ASSERT( ! Object::get(&res, "/root/foo/bar"));
  }
  
  void test_call( void )
  {
    Object root("root");
    Object one(root,"one");
    Object two(root,"two");
    Object sub(two,"sub");
    Signal res, param;
    std::string str;

    TS_ASSERT_EQUALS( root.url(),    std::string("/root") );
    TS_ASSERT_EQUALS( one.url(),     std::string("/root/one") );
    TS_ASSERT_EQUALS( two.url(),     std::string("/root/two") );
    TS_ASSERT_EQUALS( sub.url(),     std::string("/root/two/sub") );
    
    TS_ASSERT(Object::call("/root",&res,param));
    TS_ASSERT(res.get(&str));
    TS_ASSERT_EQUALS( str, std::string("one,two/"));
    
    TS_ASSERT(Object::call("/root/one",&res,param));
    TS_ASSERT(res.get(&str));
    TS_ASSERT_EQUALS( str, std::string(""));
    
    TS_ASSERT(Object::call("/root/two",&res,param));
    TS_ASSERT(res.get(&str));
    TS_ASSERT_EQUALS( str, std::string("sub"));
  }
};