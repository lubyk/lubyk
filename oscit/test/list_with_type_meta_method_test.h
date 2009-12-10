#include "test_helper.h"
#include "oscit/root.h"
#include "mock/dummy_object.h"

class ListWithTypeMetaMethodTest : public TestHelper
{
public:
  /* Bernoulli family
  /Nikolaus
  /Nikolaus/Jacob
  /Nikolaus/Nikolaus
  /Nikolaus/Johann
  /Nikolaus/Johann/Nicolaus
  /Nikolaus/Johann/Daniel
  /Nikolaus/Johann/Johann
   */
  
  void test_list_with_type_on_root( void ) {
    Root root;
    root.adopt(new DummyObject("mode", "rgb", SelectIO("rgb, yuv", "color mode", "This is a menu.")));
    root.adopt(new DummyObject("tint", 45.0, RangeIO(1, 127, "tint", "This is a slider from 1 to 127.")));
    Value res = root.list_with_type();
    // .error, .info, etc
    assert_equal("[s[*s]][s[sss]][s[sss]][s[sss]][s[sss]][s[sss]][s[ssss]][s[fffss]]", res.type_tag());
    assert_equal(".error", res[0][0].str());
    assert_equal(".info", res[1][0].str());
  }
  
  void test_list_with_type( void ) {
    Root root;
    Object * tmp = root.adopt(new Object("monitor"));
    tmp->adopt(new DummyObject("mode", "rgb", SelectIO("rgb, yuv", "color mode", "This is a menu.")));
    tmp->adopt(new DummyObject("tint", 45.0, RangeIO(1, 127, "tint", "This is a slider from 1 to 127.")));
    Value reply, res;

    reply = root.call(LIST_WITH_TYPE_PATH, Value(""));
    assert_equal("", reply[0].str());
    res = reply[1];
    assert_true(res.is_list());
    assert_equal(7, res.size());
    
    assert_equal(Url(ERROR_PATH).name(), res[0][0].str());
    assert_equal(Url(INFO_PATH).name(), res[1][0].str());
    assert_equal(Url(LIST_PATH).name(), res[2][0].str());
    assert_equal(Url(LIST_WITH_TYPE_PATH).name(), res[3][0].str());
    assert_equal(Url(TYPE_PATH).name(), res[4][0].str());
    assert_equal(Url(TREE_PATH).name(), res[5][0].str());
    assert_equal("monitor/", res[6][0].str());
    
    reply = root.call(LIST_WITH_TYPE_PATH, Value("/monitor"));
    assert_equal("/monitor", reply[0].str());
    res = reply[1];
    assert_true(res.is_list());
    assert_equal(2, res.size());
    assert_equal("mode",    res[0][0].str());
    assert_equal("tint",    res[1][0].str());
  }
  
  void test_list_with_type_on_empty( void ) {
    Root root;
    root.adopt(new Object("Nikolaus"));
    Value res;

    res = root.call(LIST_WITH_TYPE_PATH, Value("/Nikolaus"));
    assert_equal("/Nikolaus", res[0].str());
    res = res[1];
    assert_true(res.is_list());
    assert_equal(0,  res.size());
  }
  
  void test_list_with_type_with_nil( void ) {
    Root root(NoIO("This is the root node."));
    Value res;
    
    res = root.call(LIST_WITH_TYPE_PATH, gNilValue);
    assert_true(res.is_nil());
  }
};