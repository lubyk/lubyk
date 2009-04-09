#include "test_helper.h"
#include "mock/dummy_object.h"

class BaseObjectTest : public TestHelper
{
public:
  void test_create( void ) {
    DummyObject a("a", 1);
    assert_equal("a", a.name());
  }
  
  void test_rename( void ) {
    BaseObject base;
    BaseObject * foo = base.adopt(new BaseObject("foo"));
    BaseObject * bar = foo->adopt(new BaseObject("bar"));
    
    assert_equal("/foo"    , foo->url() );
    assert_equal("/foo/bar", bar->url() );
    
    foo->set_name("super");
    
    assert_equal("/super"    , foo->url() );
    assert_equal("/super/bar", bar->url() );
  }
  
  void test_child( void ) {
    BaseObject base;
    BaseObject * one = base.adopt(new BaseObject("one")); // This is the prefered way of creating sub-objects.
    BaseObject * sub = one->adopt(new BaseObject("sub"));

    assert_equal("",         base.url());
    assert_equal("/one",     one->url());
    assert_equal("/one/sub", sub->url());

    assert_equal(one, base.child("one"));
    assert_equal((BaseObject*)NULL, base.child("foo"));

    one->set_name("foo");
    assert_equal((BaseObject*)NULL, base.child("one"));
    assert_equal(one, base.child("foo"));
  }
  
  void test_first_child( void ) {
    BaseObject base("base");
    BaseObject * one = base.adopt(new BaseObject("one"));
    BaseObject * two = base.adopt(new BaseObject("aaa"));
    
    assert_equal(one, base.first_child());
    
    delete one;
    assert_equal(two, base.first_child());
  }
  
  void test_set_parent_same_name_as_sibling( void ) {
    BaseObject base;
    BaseObject * child1 = new BaseObject("foo");
    BaseObject * child2 = new BaseObject("bar");
    BaseObject * child3 = new BaseObject("foo");
    
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
  
  void test_build_child( void ) {
    BaseObject base;
    BaseObject * carrot = base.adopt(new DummyObject("dummy", 0.0));
    BaseObject * foo    = carrot->build_child(std::string("something"));
    assert_equal((BaseObject*)NULL, foo);
    foo = carrot->build_child(std::string("special"));
    assert_true( foo != NULL );
    assert_equal("/dummy/special", foo->url());
  }
  
  void test_type_cast( void ) {
    BaseObject base;
    DummyObject dummy("doudou", 1200.0);
    BaseObject   * res1;
    DummyObject  * res2;
    res1 = TYPE_CAST(BaseObject, &base);
    assert_equal(&base, res1);
    res2 = TYPE_CAST(DummyObject, &base);
    assert_equal((DummyObject*)NULL, res2);
    
    res1 = TYPE_CAST(BaseObject, &dummy);
    assert_equal((BaseObject*)NULL, res1);
    res2 = TYPE_CAST(DummyObject, &dummy);
    assert_equal(&dummy, res2);
  }
};