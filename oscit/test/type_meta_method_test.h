#include "test_helper.h"
#include "oscit/root.h"
#include "mock/dummy_object.h"

class TypeMetatMethodTest : public TestHelper
{
public:
  void test_slider_type( void ) {
    Root root;
    root.adopt(new DummyObject("foo", 4.25));
    Value res;

    root.set_info("This is the root node.");
    res = root.call("/.type", Value(""));
    assert_true(res.is_nil());

    res = root.call("/.type", Value("/foo"));
    assert_true(res.is_list());
    assert_equal("fffs", res.type_tag());
    assert_equal(4.25,  res[0].r); // current
    assert_equal(0.0,   res[1].r); // min
    assert_equal(127.0, res[2].r); // max
    assert_equal("lux", res[3].str()); // unit

    res = root.call("/.type", Value("/blah"));
    assert_true(res.is_error());
    assert_equal(NOT_FOUND_ERROR, res.error_code());
  }

  void test_menu_type( void ) {
    Root root;
    DummyObject2 * foo = root.adopt(new DummyObject2("foo", "yuv"));
    Value res;

    // set wrong current to make sure current value is retrieved
    foo->set_type("", "rgb,rgba,yuv", "color mode");

    res = root.call("/.type", Value("/foo"));
    assert_true(res.is_list());
    assert_equal("sss", res.type_tag());
    assert_equal("yuv",  res[0].str()); // current
    assert_equal("rgb,rgba,yuv",  res[1].str()); // current
    assert_equal("color mode", res[2].str()); // unit
  }

  void test_any_real_type( void ) {
    Root root;
    DummyObject * foo = root.adopt(new DummyObject("foo", 1.23));
    Value res;

    // set wrong current to make sure current value is retrieved
    foo->set_type(1.99,"Hz");

    res = root.call("/.type", Value("/foo"));
    assert_true(res.is_list());
    assert_equal("fs", res.type_tag());
    assert_equal(1.23,  res[0].r); // current
    assert_equal("Hz", res[1].str()); // unit
  }
};