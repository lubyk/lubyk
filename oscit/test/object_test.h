#include "test_helper.h"
#include "oscit/root.h"
#include "mock/dummy_object.h"

class ObjectTest : public TestHelper
{
public:
  void test_create( void ) {
    DummyObject a("a", 1);
    assert_equal("a", a.name());
  }
  
  void test_rename( void ) {
    Object base;
    Object * foo = base.adopt(new Object("foo"));
    Object * bar = foo->adopt(new Object("bar"));
    
    assert_equal("/foo"    , foo->url() );
    assert_equal("/foo/bar", bar->url() );
    
    foo->set_name("super");
    
    assert_equal("/super"    , foo->url() );
    assert_equal("/super/bar", bar->url() );
  }
  
  void test_child( void ) {
    Object base;
    Object * one = base.adopt(new Object("one")); // This is the prefered way of creating sub-objects.
    Object * sub = one->adopt(new Object("sub"));

    assert_equal("",         base.url());
    assert_equal("/one",     one->url());
    assert_equal("/one/sub", sub->url());

    assert_equal(one, base.child("one"));
    assert_equal((Object*)NULL, base.child("foo"));

    one->set_name("foo");
    assert_equal((Object*)NULL, base.child("one"));
    assert_equal(one, base.child("foo"));
  }
  
  void test_first_child( void ) {
    Object base("base");
    Object * one = base.adopt(new Object("one"));
    Object * two = base.adopt(new Object("aaa"));
    
    assert_equal(one, base.first_child());
    
    delete one;
    assert_equal(two, base.first_child());
  }
  
  void test_set_parent_same_name_as_sibling( void ) {
    Object base;
    Object * child1 = new Object("foo");
    Object * child2 = new Object("bar");
    Object * child3 = new Object("foo");
    
    assert_equal(""   , base.url()    );
    assert_equal("foo", child1->url() );
    assert_equal("bar", child2->url() );
    assert_equal("foo", child3->url() );
    
    child1->set_parent(base);
    
    assert_equal(""    , base.url()    );
    assert_equal("/foo", child1->url() );
    assert_equal("bar" , child2->url() );
    assert_equal("foo" , child3->url() );
    
    child2->set_parent(child1);
    
    assert_equal(""        , base.url()    );
    assert_equal("/foo"    , child1->url() );
    assert_equal("/foo/bar", child2->url() );
    assert_equal("foo"     , child3->url() );
    
    child3->set_parent(base);
    
    assert_equal(""        , base.url()    );
    assert_equal("/foo"    , child1->url() );
    assert_equal("/foo/bar", child2->url() );
    assert_equal("/foo-1"  , child3->url() );
  }
  
  void test_not_found( void ) {
    Object base;
    Object * carrot = base.adopt(new Object("carrot"));
    Value res = carrot->not_found(std::string("/carrot/something"), gNilValue);
    assert_true(res.is_error());
    assert_equal(NOT_FOUND_ERROR, res.error_code());
    assert_equal("/carrot/something", res.error_message());
  }
};