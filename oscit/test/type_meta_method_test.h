#include "test_helper.h"
#include "oscit/root.h"
#include "mock/dummy_object.h"

class TypeMetatMethodTest : public TestHelper
{
public:
  void test_range_type( void ) {
    Root root;
    root.adopt(new DummyObject("foo", 4.25));
    Value res;

    res = root.call("/.type", Value(""));
    assert_true(res.is_list());
    assert_equal("Ns", res.type_tag());

    res = root.call("/.type", Value("/foo"));
    assert_true(res.is_list());
    assert_equal("fffss", res.type_tag());
    assert_equal(4.25,  res[0].r); // current
    assert_equal(0.0,   res[1].r); // min
    assert_equal(127.0, res[2].r); // max
    assert_equal("lux", res[3].str()); // unit
    assert_equal(DUMMY_OBJECT_INFO, res[4].str()); // info

    res = root.call("/.type", Value("/blah"));
    assert_true(res.is_error());
    assert_equal(NOT_FOUND_ERROR, res.error_code());
  }

  void test_select_type( void ) {
    Root root;
    root.adopt(new DummyObject2("foo", "yuv"));
    Value res;
    
    res = root.call("/.type", Value("/foo"));
    assert_true(res.is_list());
    assert_equal("ssss", res.type_tag());
    assert_equal("yuv",  res[0].str()); // current
    assert_equal("rgb,rgba,yuv",  res[1].str()); // current
    assert_equal("color mode", res[2].str()); // unit
    assert_equal("Set color mode.", res[3].str()); // info
  }

  void test_any_type( void ) {
    Root root;
    root.adopt(new DummyObject("foo", 1.23, AnyInput("This is the info string.")));
    Value res;
    res = root.call("/.type", Value("/foo"));
    assert_true(res.is_list());
    assert_equal("*s", res.type_tag());
    assert_true(res[0].is_any());
    assert_equal("This is the info string.", res[1].str()); // info
  }

  void test_list_type( void ) {
    Root root;
    root.adopt(new DummyObject("Haddock", 42.0, Value(Json("[[\"\", 0.0], \"name\", \"years old\", \"Set captain with name and age.\"]"))));
    Value res;
    res = root.call("/.type", Value("/Haddock"));
    assert_true(res.is_list());
    assert_equal("[sf]sss", res.type_tag());
    assert_equal("[\"Haddock\", 42]", res[0].to_json()); // info
  }
};