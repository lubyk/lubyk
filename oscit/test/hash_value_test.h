#include "test_helper.h"
#include "oscit/values.h"

class HashValueTest : public TestHelper
{
public:
  void test_is_hash( void ) {
    Value v(TypeTag("H"));
    Value res;

    assert_false(v.is_empty());
    assert_false(v.is_nil());
    assert_false(v.is_real());
    assert_false(v.is_string());
    assert_false(v.is_list());
    assert_false(v.is_error());
    assert_true (v.is_hash());
    assert_false(v.is_matrix());
    assert_false(v.is_midi());

    assert_false(v.get("foo", &res));
    v.set("foo",3.5);
    assert_true(v.get("foo", &res));
    assert_true(res.is_real());
    assert_equal(3.5, res.r);

    assert_equal("H", v.type_tag());
    int i = H("H");
    assert_equal(i, v.type_id());
  }

  void test_create_set( void ) {
    HashValue v;

    assert_true(v.is_hash());

    assert_equal("H", v.type_tag());
  }

  void test_share( void ) {
    Value v(TypeTag("H"));
    v.set("1","one");
    v.set("2","two");

    Value v2(v);
    Value v3;

    Value res;

    assert_true(v2.is_hash());
    assert_true(v2.get("1", &res));
    assert_true(res.is_string());
    assert_equal("one", res.str());

    v.set("1","un");
    v.set("2","deux");
    assert_true(v.get("1", &res));
    assert_equal("un", res.str());

    // change in v changes v2
    assert_true(v2.get("1", &res));
    assert_equal("un", res.str());

    assert_true(v3.is_empty());

    v3 = v;

    assert_true(v3.is_hash());

    assert_true(v3.get("1", &res));
    assert_equal("un", res.str());
    assert_true(v3.get("2", &res));
    assert_equal("deux", res.str());

    v.set("1", "uno");

    // change in v changes v3
    assert_true(v3.get("1", &res));
    assert_equal("uno", res.str());

  }

  void test_set( void ) {
    Value v;

    v.set("nice", "friends");

    assert_true(v.is_hash());
  }

  void test_set_tag( void ) {
    Value v;

    v.set_type_tag("H");
    assert_true(v.is_hash());
  }

  void test_set_type( void ) {
    Value v;

    v.set_type(HASH_VALUE);
    assert_true(v.is_hash());
  }

  void test_key_iterator( void ) {
    Value v;
    Value res;

    HashIterator it  = v.begin();
    HashIterator end = v.end();

    assert_true(it == end);

    v.set("a", 1);
    v.set("c", 3);
    v.set("b", 2);

    it  = v.begin();
    end = v.end();

    while( it != end ) {
      std::string key(*it++);
      assert_true(v.get(key, &res));
      assert_true(res.is_real());
      if (key == "a") {
        assert_equal(1.0, res.r);
      } else if (key == "b") {
        assert_equal(2.0, res.r);
      } else if (key == "c") {
        assert_equal(3.0, res.r);
      } else {
        assert_equal("wrong key !", key);
      }
    }
  }

  void test_read( void ) {
    Value v('H');
    Hash &l = *v.hash_;
    Value res;

    assert_false(v.get("one", &res));

    l.set("one", Value(1.0));

    assert_true(v.get("one", &res));
    assert_equal(1.0, res.r);
  }

  void test_set_real( void ) {
    Value v;
    v.set("one", 1.34);
    assert_true(v.is_hash());
    assert_equal(1.34, v["one"].r); // this is a dangerous syntax: v["real"] can return gNilValue...

    v.set("two", 3.45);
    assert_true(v["two"].is_real());
  }

  void test_set_string( void ) {
    Value v(1.0);
    v.set("one", "first");
    assert_true(v.is_hash());
    assert_true(v["one"].is_string());
    assert_equal("first", v["one"].str());
  }

  void test_set_list( void ) {
    Value v;
    Value l;
    l.set("one").push_back(2.0);
    v.set("list", l);
    assert_true(v.is_hash());

    assert_true(v["list"].is_list());
    assert_equal(2, v["list"].size());

    assert_true(v["list"][0].is_string());
    assert_equal("one", v["list"][0].str());

    assert_true(v["list"][1].is_real());
    assert_equal(2.0, v["list"][1].r);
  }

  void test_set_hash( void ) {
    Value v;
    Hash h(20);
    h.set("one", Value(1.0));
    v.set(h);
    assert_true(v.is_hash());
    assert_equal(1.0, v["one"].r);
    h.set("one", Value(2.0));
    // copy, not sharing
    assert_equal(1.0, v["one"].r);
  }

  void test_to_json( void ) {
    Value v;
    Value jobs;
    v.set("name", "Joe");
    v.set("age", 34.0);
    jobs.set("dad").push_back("husband").push_back("lover").push_back(-666);
    v.set("job", jobs);

    std::ostringstream os(std::ostringstream::out);
    os << v;
    assert_equal("{\"name\":\"Joe\", \"age\":34, \"job\":[\"dad\", \"husband\", \"lover\", -666]}", os.str());
    assert_equal("{\"name\":\"Joe\", \"age\":34, \"job\":[\"dad\", \"husband\", \"lover\", -666]}", v.to_json());
  }

  void test_lazy_json( void ) {
    Value v;
    Value jobs;
    v.set("name", "Joe");
    v.set("age", 34.0);
    jobs.set("dad").push_back("husband").push_back("lover").push_back(-666);
    v.set("job", jobs);

    assert_equal("name:\"Joe\" age:34 job:[\"dad\", \"husband\", \"lover\", -666]", v.lazy_json());
  }

  void test_from_json_strict( void ) {
    Value v(Json("{\"week_start_on\":1.0, monday:\"Lundi\"}"));
    assert_true(v.is_hash());
    assert_equal(1.0, v["week_start_on"].r);
    assert_equal("Lundi", v["monday"].str());
  }

  void test_from_json_lazy( void ) {
    Value v(Json("week_start_on:1.0 monday:\"Lundi\""));
    assert_equal("{\"week_start_on\":1, \"monday\":\"Lundi\"}", v.to_json());
    assert_true(v.is_hash());
    assert_equal(1.0, v["week_start_on"].r);
    assert_equal("Lundi", v["monday"].str());
  }

  void test_from_json_hash_in_hash( void ) {
    Value v(Json("week_start_on:1.0 weeks:{monday:\"Lundi\" friday:\"free\"}"));
    assert_true(v.is_hash());
    assert_equal(1.0, v["week_start_on"].r);
    assert_equal("Lundi", v["weeks"]["monday"].str());
  }

  void test_from_json_hash_in_hash_curly( void ) {
    Value v(Json("{week_start_on:1.0 weeks:{monday:\"Lundi\" friday:\"free\"}}"));
    assert_true(v.is_hash());
    assert_equal(1.0, v["week_start_on"].r);
    assert_true(v["weeks"].is_hash());
    assert_equal("free", v["weeks"]["friday"].str());
  }

  void test_can_receive( void ) {
    Object object("foo", HashIO("bar"));
    assert_false(object.can_receive(Value()));
    assert_true (object.can_receive(gNilValue));
    assert_false(object.can_receive(Value(1.23)));
    assert_false(object.can_receive(Value("foo")));
    assert_false(object.can_receive(Value(BAD_REQUEST_ERROR, "foo")));
    assert_false(object.can_receive(JsonValue("['','']")));
    assert_true (object.can_receive(HashValue()));
    assert_false(object.can_receive(MatrixValue(1,1)));
    assert_false(object.can_receive(MidiValue()));
  }

  void test_equal( void ) {
    Value a(Json("one: 1 two: 2"));
    Value b(Json("two: 2 one: 1"));
    assert_equal(a, b);
    a.set(Json("one: 1 two: 3"));
    assert_false(a == b);
    a.set_nil();
    assert_false(a == b);
  }

  void test_deep_merge( void ) {
    Value a(Json("{one:{x:4 y:10} two:{x:5 y:8}}"));
    Value b(Json("{one:{x:7} two:null}"));
    a.deep_merge(b);
    assert_equal("{\"one\":{\"x\":7, \"y\":10}}", a.to_json());
  }

  void test_integer_key( void ) {
    Value a(Json("1:\"one\" two: 2"));
    assert_equal("{\"1\":\"one\", \"two\":2}", a.to_json());
  }
};
