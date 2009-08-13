#include "test_helper.h"
#include "oscit/root.h"
#include "mock/dummy_object.h"

class InfoMetaMethodTest : public TestHelper
{
public:
  void test_info( void ) {
    Root root(NoIO("This is the root node."));
    root.adopt(new Object("foo"));
    Value res;
    
    res = root.call(INFO_PATH, Value(""));
    assert_true(res.is_list());
    assert_equal("", res[0].str());
    assert_equal("This is the root node.", res[1].str());

    res = root.call(INFO_PATH, Value("/foo"));
    assert_true(res.is_list());
    assert_equal("/foo", res[0].str());
    assert_equal(DEFAULT_TYPE.str(), res[1].str());

    res = root.call(INFO_PATH, Value("/blah"));
    assert_equal("/blah", res[0].str());
    assert_true(res[1].is_error());
    assert_equal(NOT_FOUND_ERROR, res[1].error_code());
  }
  
  void test_info_with_nil( void ) {
    Root root(NoIO("This is the root node."));
    Value res;
    
    res = root.call(INFO_PATH, gNilValue);
    assert_true(res.is_nil());
  }
};