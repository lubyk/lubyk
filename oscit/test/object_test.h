#include "test_helper.h"
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
    
    child1->set_parent(&base);
    
    assert_equal(""    , base.url()    );
    assert_equal("/foo", child1->url() );
    assert_equal("bar" , child2->url() );
    assert_equal("foo" , child3->url() );
    
    child2->set_parent(child1);
    
    assert_equal(""        , base.url()    );
    assert_equal("/foo"    , child1->url() );
    assert_equal("/foo/bar", child2->url() );
    assert_equal("foo"     , child3->url() );
    
    child3->set_parent(&base);
    
    assert_equal(""        , base.url()    );
    assert_equal("/foo"    , child1->url() );
    assert_equal("/foo/bar", child2->url() );
    assert_equal("/foo-1"  , child3->url() );
  }
  
  void test_build_child( void ) {
    Value error;
    Object base;
    Object * carrot = base.adopt(new DummyObject("dummy", 0.0));
    Object * foo    = carrot->build_child(std::string("something"), &error);
    assert_equal((Object*)NULL, foo);
    foo = carrot->build_child(std::string("special"), &error);
    assert_true( foo != NULL );
    assert_equal("/dummy/special", foo->url());
  }
  
  void test_call_set_method( void ) {
    Object base;
    DummyObject * one = base.adopt(new DummyObject("one", 123.0));
    DummyObject * two = base.adopt(new DummyObject("two", 123.0));
    Value res = base.set(Value(Json("one:10.0 two:22.22")));
    assert_true(res.is_hash());
    assert_equal(10.0, res["one"].r);
    assert_equal(22.22, res["two"].r);
    assert_equal(10.0, one->real());
    assert_equal(22.22, two->real());
  }
  
  void test_call_set_method_return_bool( void ) {
    Object base;
    DummyObject * one = base.adopt(new DummyObject("one", 123.0));
    DummyObject * two = base.adopt(new DummyObject("two", 123.0));
    assert_true(base.set_all_ok(Value(Json("one:10.0 two:22.22"))));
    assert_false(base.set_all_ok(Value(Json("one:1.0 four:4.0"))));
    assert_equal(1.0,   one->real());
    assert_equal(22.22, two->real());
  }

  void test_hash_type_id( void ) {
    Object hash("foo", HashIO("bar"));
    Object matr("foo", MatrixIO(1,4,"bar"));
    assert_false(hash.type_id() == matr.type_id());
    assert_false(hash.type_id() == matr.type_id());
    assert_equal("Hs", hash.type().type_tag());
    assert_equal("Ms", matr.type().type_tag());
  }
  
  void test_list_with_type( void ) {
    Object base;
    base.adopt(new DummyObject("mode", 0.0, SelectIO("rgb, yuv", "color mode", "This is a menu.")));
    base.adopt(new DummyObject("tint", 45.0, RangeIO(1, 127, "tint", "This is a slider from 1 to 127.")));
    Value res = base.list_with_type();
    assert_equal(res.type_tag(), "[s[ssss]][s[fffss]]"); // [name, [current, values, unit, info]], [name, [current, min, max, unit, info]]
    assert_equal("mode", res[0][0].str());
    assert_equal("tint", res[1][0].str());
  }
  
  // set_type is not a good idea. It should be immutable (or maybe I'm wrong, so I leave the test here)
  //void test_set_type( void ) {
  //  DummyObject one("one", 123.0);
  //  assert_equal("fffss", one.type().type_tag()); // RangeIO
  //  assert_equal(0.0, one.type()[0].r); // current
  //  assert_equal(0.0, one.type()[1].r);   // min
  //  assert_equal(127.0, one.type()[2].r); // max
  //  assert_equal("lux", one.type()[3].str()); // unit
  //  assert_equal(DUMMY_OBJECT_INFO, one.type()[4].str()); // info
  //  
  //  Value type(TypeTag("fss"));
  //  assert_false(one.set_type(type));
  //  assert_equal("fffss", one.type().type_tag());
  //  assert_equal("lux", one.type()[3].str()); // unit
  //  type.set(0.0);
  //  type.push_back(0).push_back(5).push_back("eggs").push_back("I'm a hen !");
  //  assert_true(one.set_type(type));
  //  assert_equal("fffss", one.type().type_tag());
  //  assert_equal("eggs", one.type()[1].str());
  //}
};