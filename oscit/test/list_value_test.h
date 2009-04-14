#include "test_helper.h"
#include "oscit/values.h"

class ListValueTest : public TestHelper
{  
public:
  void test_create( void ) {
    Value v(TypeTag("fsf"));
    
    assert_false(v.is_nil());
    assert_false(v.is_real());
    assert_false(v.is_string());
    assert_true (v.is_list());
    assert_false(v.is_error());
    assert_false(v.is_hash());
    
    // 0
    assert_true(v[0].is_real());
    assert_equal(0.0, v[0].r);
    
    // 1
    assert_true(v[1].is_string());
    assert_equal("", v[1].str());
    
    // 2
    assert_true(v[2].is_real());
    assert_equal(0.0, v[2].r);
    
    assert_equal("fsf", v.type_tag());
  }
  
  void test_create_list_value( void ) {
    ListValue v("ffs");
    ListValue v2;
    
    assert_true(v.is_list());
    assert_true(v2.is_list());
    
    assert_equal("ffs", v.type_tag());
    assert_equal("", v2.type_tag());
  }
  
  void test_copy( void ) {
    Value v(TypeTag("fs"));
    assert_equal(1, v.list_->ref_count());
    v[0].r = 0.0;
    v[1].set("");
    
    Value * v2 = new Value(v);
    assert_true(v2->is_list());
    assert_equal(2, v2->size());
    assert_equal(2, v.list_->ref_count());
    assert_equal(v.list_, v2->list_);
    
    v[0].r = 1.2;
    v[1].set("super man");
    
    // change in v should change v2 (shared)
    assert_equal(1.2, v2->value_at(0).r);
    assert_equal("super man",  v2->value_at(1).str());
    
    Value v3;
    assert_true(v3.is_nil());
    
    v3 = v;
    
    assert_true(v3.is_list());
    assert_equal(3, v.list_->ref_count());
    assert_equal(v.list_, v3.list_);
    
    delete v2;
    assert_equal(2, v.list_->ref_count());
    
    assert_equal(1.2,         v3[0].r);
    assert_equal("super man", v3[1].str());
    
    v[1].set("super woman");
    
    // change in v should change v3
    assert_equal("super woman", v3[1].str());
  }
  
  void test_set( void ) {
    Value v;
    List l("fs");
    
    assert_true(v.is_nil());
    
    v.set(&l);
    
    assert_true(v.is_list());
  }
  
  void test_set_value_at( void ) {
    Value v(3.4);
    ListValue l("sss");
    l.set_value_at(1,Value(3.5));
    assert_equal("sfs", l.type_tag());
    assert_equal(3.5, l[1].r);
  }
  
  void test_set_tag_sub_list( void ) {
    Value v;
    
    v.set_type_tag("sff[fsf]");
    assert_true(v.is_list());
    assert_equal("",  v[0].str());
    assert_equal(0.0, v[1].r);
    assert_equal(0.0, v[2].r);
    assert_true(v[3].is_list());
    assert_equal("fsf", v[3].type_tag());
  }
  
  void test_set_tag( void ) {
    Value v;
    
    v.set_type_tag("sff");
    assert_true(v.is_list());
    assert_equal("",  v[0].str());
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
  
  void test_push_back_real( void ) {
    Value v;
    v.push_back(1.34);
    assert_true(v.is_real());
    assert_equal(1.34, v.r);
    
    v.push_back(3.45);
    assert_true(v.is_list());
    assert_true(v[0].is_real());
    assert_true(v[1].is_real());
    assert_equal(2, v.size());
    assert_equal(1.34, v[0].r);
    assert_equal(3.45, v[1].r);
  }
  
  void test_push_back_real_to_real( void ) {
    Value v(1.0);
    v.push_back(2.0);
    assert_true(v.is_list());
    assert_equal(2, v.size());
    assert_true(v[0].is_real());
    assert_true(v[1].is_real());
    assert_equal(1.0, v[0].r);
    assert_equal(2.0, v[1].r);
  }
  
  void test_push_back_list( void ) {
    Value v;
    Value l(TypeTag("fss"));
    l[0].r = 10.0;
    l[1].set("one");
    l[2].set("two");
    v.push_back(1.34);
    assert_true(v.is_real());
    assert_equal("f", v.type_tag());
    v.push_back(l);
    assert_true(v.is_list());
    assert_equal("f[fss]", v.type_tag());
    assert_equal("two", v[1][2].str());
  }
  
  void test_push_front_real( void ) {
    Value v;
    v.push_front(1.34);
    assert_true(v.is_real());
    assert_equal(1.34, v.r);
    
    v.push_front(3.45);
    assert_true(v.is_list());
    assert_equal(2, v.size());
    assert_true(v[0].is_real());
    assert_equal(3.45, v[0].r);
    assert_equal(1.34, v[1].r);
  }
  
  void test_push_front_real_to_real( void ) {
    Value v(1.0);
    v.push_front(2.0);
    assert_true(v.is_list());
    assert_equal(2, v.size());
    assert_true(v[0].is_real());
    assert_true(v[1].is_real());
    assert_equal(2.0, v[0].r);
    assert_equal(1.0, v[1].r);
  }
  
  void test_push_front_list( void ) {
    Value v;
    Value l(TypeTag("ss"));
    l[0].set("one");
    l[1].set("two");
    v.push_front(1.34);
    assert_true(v.is_real());
    assert_equal("f", v.type_tag());
    v.push_front(l);
    assert_true(v.is_list());
    assert_equal("[ss]f", v.type_tag());
    assert_equal("one", v[0][0].str());
    assert_equal("two", v[0][1].str());
    assert_equal(1.34,  v[1].r);
  }

  void test_to_json( void ) {
    Value v(TypeTag("fsff"));
    v[0].r = 1.234;
    v[1].set("Hello World!");
    v[2].r = 9.8;
    v[3].r = 2000;
    std::ostringstream os(std::ostringstream::out);
    os << v;
    assert_equal("[1.234, \"Hello World!\", 9.8, 2000]", os.str());
    assert_equal("[1.234, \"Hello World!\", 9.8, 2000]", v.to_json());
  }
  
  void test_from_json_strict_trailing( void ) {
    Value v;
    const char * json = "[2.0, 3.0], 4.0";
    const char * trailing = json + v.build_from_json(json);
    assert_true(v.is_list());
    assert_equal(", 4.0", trailing);
  }
  
  void test_from_json_strict( void ) {
    Value v(Json("[2.0, 3.0]"));
    assert_true(v.is_list());
    assert_equal(2.0, v[0].r);
    assert_equal(3.0, v[1].r);
  }
  
  void test_from_json_mixed_strict( void ) {
    Value v(Json("[1.0, \"two\"]"));
    assert_true(v.is_list());
    assert_equal(1.0, v[0].r);
    assert_equal("two", v[1].str());
  }
  
  void test_from_json_lazy( void ) {
    Value v(Json("1.0, \"two\""));
    assert_true(v.is_list());
    assert_equal(1.0, v[0].r);
    assert_equal("two", v[1].str());
  }
  
  void test_from_json_list_in_list_lazy( void ) {
    Value v(Json("1.0, [2.0, 3.0], [\"four\",\"five\"]"));
    assert_true(v.is_list());
    assert_equal(1.0, v[0].r);
    assert_equal(2.0, v[1][0].r);
    assert_equal(3.0, v[1][1].r);
    assert_equal("four", v[2][0].str());
    assert_equal("five", v[2][1].str());
  }
  
  void test_from_json_list_in_list_bracket( void ) {
    Value v(Json("[[1.0, 2.0], 3.0, [\"four\",\"five\"]]"));
    assert_true(v.is_list());
    assert_equal(1.0, v[0][0].r);
    assert_equal(2.0, v[0][1].r);
    assert_equal(3.0, v[1].r);
    assert_equal("four", v[2][0].str());
    assert_equal("five", v[2][1].str());
  }
  
  void test_from_json_hash_in_list( void ) {
    Value v(Json("[{one:\"one\" two:2.0}, 3.0]"));
    assert_true(v.is_list());
    assert_equal("Hf", v.type_tag());
    assert_equal(2.0, v[0]["two"].r);
    assert_equal("one", v[0]["one"].str());
    assert_equal(3.0, v[1].r);
  }
  
  void test_from_json_newline_in_list( void ) {
    Value v(Json("one:\"one\"\ntwo:2.0"));
    assert_true(v.is_hash());
    assert_equal("H", v.type_tag());
    assert_equal("one", v["one"].str());
    assert_equal(2.0, v["two"].r);
  }
};