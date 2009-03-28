#include "test_helper.h"
#include "oscit/values.h"

class ListValueTest : public TestHelper
{  
public:
  void test_create( void )
  {
    Value v((TypeTag)"fsf");
    
    assert_false(v.is_nil());
    assert_false(v.is_real());
    assert_false(v.is_string());
    assert_true (v.is_list());
    assert_false(v.is_error());
    
    // 0
    assert_true(v[0].is_real());
    assert_equal(0.0, v[0].r);
    
    // 1
    assert_true(v[1].is_string());
    assert_equal("", v[1].s);
    
    // 2
    assert_true(v[2].is_real());
    assert_equal(0.0, v[2].r);
    
    assert_equal("fsf", v.type_tag());
  }
  
  void test_copy( void ) {
    Value v(TypeTag("fs"));
    Value v2(v);
    Value v3;
    
    v[0].r = 1.2;
    v[1].set("super man");
    
    assert_true(v2.is_list());
    assert_equal(2, v2.size());
    
    // change in v did not change v2
    assert_equal(0.0, v2[0].r);
    assert_equal("",  v2[1].s);
    
    assert_true(v3.is_nil());
    
    v3 = v;
    
    assert_true(v3.is_list());
    
    assert_equal(1.2,         v3[0].r);
    assert_equal("super man", v3[1].s);
    
    v[1].set("super woman");
    
    // change in v did not change v3
    assert_equal("super man", v3[1].s);
  }
  
  void test_set( void ) {
    Value v;
    List l("fs");
    
    assert_true(v.is_nil());
    
    v.set(&l);
    
    assert_true(v.is_list());
  }
  
  void test_set_tag( void ) {
    Value v;
    
    v.set_type_tag("sff");
    assert_true(v.is_list());
    assert_equal("",  v[0].s);
    assert_equal(0.0, v[1].r);
    assert_equal(0.0, v[2].r);
  }
  
  void test_set_type( void ) {
    Value v;
    
    v.set_type(LIST_VALUE);
    assert_true(v.is_list());
    assert_equal(0, v.size());
  }
  
  void test_size( void ) {
    Value v;
    assert_equal(0, v.size());
    v.set_type_tag("ffsfsf");
    assert_equal(6, v.size());
    v.set_type_tag("f");
    assert_equal(0, v.size()); // not a list
    v.set(List());
    assert_equal(0, v.size()); // empty list
  }
  
  void test_read( void ) {
    Value v(TypeTag("ff"));
    List &l = *v.list_;
    
    assert_equal(0.0, l[0].f);
    assert_equal(0.0, l[1].f);
    l[0].f = 3.5;
    assert_equal(3.5, l[0].f);
    assert_equal(0.0, l[1].f);
  }
  
  void test_append_real( void ) {
    Value v;
    v.append(1.34);
    assert_true(v.is_list());
    assert_true(v[0].is_real());
    assert_equal(1.34, v[0].r);
    
    v.append(3.45);
    assert_equal(2, v.size());
    assert_true(v[1].is_real());
    assert_equal(3.45, v[1].r);
  }
  
  void test_append_list( void ) {
    Value v;
    Value l(TypeTag("fss"));
    l[0].r = 10.0;
    l[1].set("one");
    l[2].set("two");
    v.append(1.34);
    assert_true(v.is_list());
    assert_equal("f", v.type_tag());
    v.append(l);
    assert_equal("ffss", v.type_tag());
    assert_equal("two", v[3].s);
  }
};