#include "test_helper.h"
#include "oscit/root.h"
#include "mock/dummy_object.h"

class InfoMetaMethodTest : public TestHelper
{
public:
  void test_info( void ) {
    Root root(NoInput("This is the root node."));
    root.adopt(new Object("foo"));
    Value res;
    
    res = root.call("/.info", Value(""));
    assert_true(res.is_string());
    assert_equal("This is the root node.", res.str());

    res = root.call("/.info", Value("/foo"));
    assert_true(res.is_string());
    assert_equal(DEFAULT_TYPE[1].str(), res.str());

    res = root.call("/.info", Value("/blah"));
    assert_true(res.is_error());
    assert_equal(NOT_FOUND_ERROR, res.error_code());
  }
};