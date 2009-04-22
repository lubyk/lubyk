#include <cxxtest/TestSuite.h>
#include "test_helper.h"
#include "mock/dummy_object.h"
#include "oscit/alias.h"

class AliasTest : public TestHelper {
 public:
  void test_trigger( void ) {
    Root root;
    Object   * one = root.adopt(new Object("one"));
    DummyObject  * sub = one->adopt(new DummyObject("sub", 3));
    Alias        * ali = root.adopt(new Alias("ali", sub));
    DummyObject  * two = root.adopt(new DummyObject("two", 2));
    
    Value res;
    
    assert_equal("/one" ,    one->url());
    assert_equal("/one/sub", sub->url());
    assert_equal("/ali",     ali->url());
    assert_equal("/two",     two->url());
    
    assert_equal(3.00, sub->real());
    res = root.call("/one/sub");
    assert_equal(3.00, res.r);
    res = root.call("/ali");
    assert_equal(3.00, res.r);
    
    res = root.call("/one/sub", Value(4.5));
    assert_equal(4.50, res.r);
    res = root.call("/one/sub");
    assert_equal(4.50, res.r);
    res = root.call("/ali");
    assert_equal(4.50, res.r);
    res = root.call("/two");
    assert_equal(2.00, res.r);
    
    res = root.call("/ali", Value(23.0));
    assert_equal(23.00, res.r);
    res = root.call("/one/sub");
    assert_equal(23.00, res.r);
    
    delete one; // delete original first
    res = root.call("/ali", Value(23.0));
    assert_equal("/ali", res.error_message());
    assert_equal(NOT_FOUND_ERROR, res.error_code());
  }
  
  void test_delete_alias_first( void )
  {
    Root root;
    Object       * one = root.adopt(new Object("one")); //   [/one]
    DummyObject  * sub = one->adopt(new DummyObject("sub", 3)); //   [/one/sub]
    Alias        * ali = root.adopt(new Alias("ali", sub));
    Value res;
    
    assert_equal("/one", one->url());
    assert_equal("/one/sub", sub->url());
    assert_equal("/ali", ali->url());
    
    assert_equal(3.00, sub->real());
    res = root.call("/one/sub");
    assert_equal(3.00, res.r);
    res = root.call("/ali");
    assert_equal(3.00, res.r);
    
    delete ali;
    res = root.call("/ali");
    assert_equal("/ali", res.error_message());
    assert_equal(NOT_FOUND_ERROR, res.error_code());
    res = root.call("/one/sub");
    assert_equal(3.00, res.r);
    
    delete one;
    res = root.call("/one/sub");
    assert_equal("/one/sub", res.error_message());
    assert_equal(NOT_FOUND_ERROR, res.error_code());
  }
  
};