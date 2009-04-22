#include "test_helper.h"
#include "oscit/values.h"

class ListValueTest : public TestHelper
{  
public:
  void test_create( void ) {
    Value v(TypeTag("fsf"));

    assert_false(v.is_empty());
    assert_false(v.is_nil());
    assert_false(v.is_real());
    assert_false(v.is_string());
    assert_true (v.is_list());
    assert_false(v.is_error());
    assert_false(v.is_hash());
    assert_false(v.is_any());
    
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
    int i = H("fsf");
    assert_equal(i, v.type_id());
  }
  
  void test_empty_list_is_nil( void ) {
    ListValue v;
    assert_true(v.is_list());
    assert_equal("", v.type_tag());
    v.push_back(1.0);
    assert_equal("f", v.type_tag());
  }
  
  void test_create_list_value( void ) {
    ListValue v("ffs");
    ListValue v2;
    
    assert_true(v.is_list());
    assert_true(v2.is_list());
    
    assert_equal("ffs", v.type_tag());
    assert_equal("", v2.type_tag());
  }
  
  void test_share( void ) {
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
  
  void test_copy( void ) {
    Value v(TypeTag("fs"));
    assert_equal(1, v.list_->ref_count());
    v[0].r = 0.0;
    v[1].set("");
    
    Value v2;
    v2.copy(v);
    assert_true(v2.is_list());
    assert_equal(2, v2.size());
    assert_equal(1, v.list_->ref_count());
    assert_false(v.list_ == v2.list_);
    
    v[0].r = 1.2;
    v[1].set("super man");
    
    // change in v should not change v2
    assert_equal(0.0, v2.value_at(0).r);
    assert_equal("",  v2.value_at(1).str());
    
    // TODO: test deep copy
    // TODO: test copy for hash, error, matrix, ...
    
    // Value v3;
    // 
    // v3 = v;
    // 
    // assert_true(v3.is_list());
    // assert_equal(3, v.list_->ref_count());
    // assert_equal(v.list_, v3.list_);
    // 
    // delete v2;
    // assert_equal(2, v.list_->ref_count());
    // 
    // assert_equal(1.2,         v3[0].r);
    // assert_equal("super man", v3[1].str());
    // 
    // v[1].set("super woman");
    // 
    // // change in v should change v3
    // assert_equal("super woman", v3[1].str());
  }
  
  void test_set( void ) {
    Value v;
    List l("fs");
    
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
  
  void test_set_list_value_at( void ) {
    Value l(TypeTag("[fs]s"));
    Value v(3.4);
    assert_equal("[fs]s", l.type_tag());
    assert_equal("[[0, \"\"], \"\"]", l.to_json());
    v.push_back("hho");
    l.set_value_at(0,v);
    assert_equal("[fs]s", l.type_tag());
    assert_equal("[[3.4, \"hho\"], \"\"]", l.to_json());
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
    
    assert_equal(0.0, l[0]->r);
    assert_equal(0.0, l[1]->r);
    l[0]->r = 3.5;
    assert_equal(3.5, l[0]->r);
    assert_equal(0.0, l[1]->r);
  }
  
  void test_push_back_on_empty( void ) {
    Value v;
    v.push_back(Value('N'));
    assert_equal("N", v.type_tag());
    assert_true(v.is_nil());
    v.push_back(gEmptyValue);
    assert_equal("N", v.type_tag());
    v.push_back(gNilValue);
    assert_equal("NN", v.type_tag());
    assert_true(v.is_list());
    assert_equal(2, v.size());
    assert_true(v[0].is_nil());
    assert_true(v[1].is_nil());
  }
  
  void test_push_back_empty( void ) {
    Value v;
    v.push_back(gEmptyValue);
    assert_true(v.is_empty());
    assert_equal("", v.type_tag());
  }
  
  void test_push_back_nil( void ) {
    Value v;
    v.set_nil().push_back(gNilValue);
    assert_equal("NN", v.type_tag());
    assert_equal(2, v.size());
    assert_true(v[0].is_nil());
    assert_true(v[1].is_nil());
  }
  
  void test_push_back_real( void ) {
    Value v;
    v.set(1.34).push_back(3.45);
    
    assert_true(v.is_list());
    assert_true(v[0].is_real());
    assert_true(v[1].is_real());
    assert_equal(2, v.size());
    assert_equal(1.34, v[0].r);
    assert_equal(3.45, v[1].r);
  }
  
  void test_push_back_real_on_empty_list( void ) {
    // push_back on empty list is the same as set(...)
    Value v;
    v.push_back(1.34);
    
    assert_true(v.is_real());
    assert_equal(1.34, v.r);
  }
  
  void test_push_back_string( void ) {
    Value v;
    v.set("foo").push_back("bar");
    
    assert_true(v.is_list());
    assert_true(v[0].is_string());
    assert_true(v[1].is_string());
    assert_equal(2, v.size());
    assert_equal("foo", v[0].str());
    assert_equal("bar", v[1].str());
  }
  
  void test_push_back_error( void ) {
    Value v;
    v.set(BAD_REQUEST_ERROR, "blah").push_back(ErrorValue(INTERNAL_SERVER_ERROR, "bad, bad, bad"));
    
    assert_true(v.is_list());
    assert_true(v[0].is_error());
    assert_true(v[1].is_error());
    assert_equal(2, v.size());
    assert_equal(BAD_REQUEST_ERROR,     v[0].error_code());
    assert_equal(INTERNAL_SERVER_ERROR, v[1].error_code());
  }
  
  void test_push_back_hash( void ) {
    Value v;
    v.set(Json("one:1 two:2")).push_back(Json("un:1 deux:2"));
    
    assert_true(v.is_list());
    assert_true(v[0].is_hash());
    assert_true(v[1].is_hash());
    assert_equal(2, v.size());
    assert_equal("{\"one\":1 \"two\":2}", v[0].to_json());
    assert_equal("{\"un\":1 \"deux\":2}", v[1].to_json());
  }
  
  void test_push_back_matrix( void ) {
    Value v;
    v.set(MatrixValue(2,2)).push_back(MatrixValue(1,2));
    
    assert_true(v.is_list());
    assert_true(v[0].is_matrix());
    assert_true(v[1].is_matrix());
    assert_equal(2, v.size());
    assert_equal(2, v[0].matrix_->rows);
    assert_equal(2, v[0].matrix_->cols);
    assert_equal(1, v[1].matrix_->rows);
    assert_equal(2, v[1].matrix_->cols);
  }
  
  void test_push_back_any( void ) {
    Value v;
    v.set_any().push_back("bar");
    
    assert_true(v.is_list());
    assert_true(v[0].is_any());
    assert_true(v[1].is_string());
    assert_equal(2, v.size());
    assert_equal("bar", v[1].str());
  }
  
  void test_push_back_list_on_real( void ) {
    Value v;
    Value l(TypeTag("fss"));
    l[0].r = 10.0;
    l[1].set("one");
    l[2].set("two");
    v.set(1.34);
    assert_true(v.is_real());
    assert_equal("f", v.type_tag());
    v.push_back(l);
    assert_true(v.is_list());
    assert_equal("f[fss]", v.type_tag());
    assert_equal("two", v[1][2].str());
  }
  
  void test_push_back_list_on_nil( void ) {
    Value v('N');
    Value l(TypeTag("fss"));
    l[0].r = 10.0;
    l[1].set("one");
    l[2].set("two");
    assert_true(v.is_nil());
    assert_equal("N", v.type_tag());
    v.push_back(l);
    assert_true(v.is_list());
    assert_equal("N[fss]", v.type_tag());
    assert_equal("two", v[1][2].str());
  }
  
  void test_push_front_real( void ) {
    Value v;
    v.push_front(1.34);
    assert_true(v.is_real());
    
    v.set(1.34).push_front(3.45);
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
    v.set(1.34);
    assert_true(v.is_real());
    assert_equal("f", v.type_tag());
    v.push_front(l);
    assert_true(v.is_list());
    assert_equal("[ss]f", v.type_tag());
    assert_equal("one", v[0][0].str());
    assert_equal("two", v[0][1].str());
    assert_equal(1.34,  v[1].r);
  }
  
  void test_last( void ) {
    Value v;
    assert_true(v.last().is_empty());
    v.set(1.5);
    assert_equal(1.5, v.last().r);
    v.push_back("how");
    assert_equal("how", v.last().str());
  }
  
  void test_first( void ) {
    Value v;
    assert_true(v.first().is_empty());
    v.set(1.5);
    assert_equal(1.5, v.first().r);
    v.push_back("how");
    assert_equal(1.5, v.first().r);
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
  
  void test_lazy_json( void ) {
    Value v(TypeTag("fsff"));
    v[0].r = 1.234;
    v[1].set("Hello World!");
    v[2].r = 9.8;
    v[3].r = 2000;
    assert_equal("1.234, \"Hello World!\", 9.8, 2000", v.lazy_json());
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
    assert_equal("[[1, 2], 3, [\"four\", \"five\"]]", v.to_json());
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
  
  void test_can_receive( void ) {
    Object object("foo", JsonValue("[['',1,''], 'url', 'url', 'info']")); // list
    assert_false(object.can_receive(Value()));
    assert_true (object.can_receive(gNilValue));
    assert_false(object.can_receive(Value(1.23)));
    assert_false(object.can_receive(Value("foo")));
    assert_false(object.can_receive(Value(BAD_REQUEST_ERROR, "foo")));
    assert_true (object.can_receive(JsonValue("['asd',3,'']")));      // same arguments
    assert_true (object.can_receive(JsonValue("['',5,'', 1.0]"))); // extra arguments are allowed
    assert_false(object.can_receive(JsonValue("['','','']")));    // wrong arguments
    assert_false(object.can_receive(JsonValue("['',2]")));    // too short
    assert_false(object.can_receive(HashValue()));
    assert_false(object.can_receive(MatrixValue(1,1)));
    assert_false(object.can_receive(MidiValue()));
  }
};