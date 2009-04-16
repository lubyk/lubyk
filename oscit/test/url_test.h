#include "test_helper.h"
#include "oscit/url.h"

class UrlTest : public TestHelper
{  
public:
  void test_create( void ) {
    Url url("osc://example.com:7010/one/two");
    assert_equal("osc", url.protocol());
    assert_equal("example.com", url.host());
    assert_equal(7010, url.port());
    assert_equal("/one/two", url.path());
  }
  
  void test_create_local( void ) {
    Url url("/one/two");
    assert_equal("", url.protocol());
    assert_equal("", url.host());
    assert_equal(0, url.port());
    assert_equal("/one/two", url.path());
  }
  
  void test_create_relative( void ) {
    Url url("one/two");
    assert_equal("", url.protocol());
    assert_equal("", url.host());
    assert_equal(0, url.port());
    assert_equal("one/two", url.path());
  }
  
  void test_create_bad_url( void ) {
    Url url("one/two://foo/bar");
    assert_equal("", url.path());
  }
  
  void test_create_bad_path( void ) {
    Url url("http://www.example.com /foo/bar");
    assert_equal("", url.path());
  }
};
