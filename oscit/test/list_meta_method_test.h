#include "test_helper.h"
#include "oscit/root.h"
#include "mock/dummy_object.h"

class ListMetaMethodTest : public TestHelper
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
  void test_list( void ) {
    Root root;
    Object * tmp = root.adopt(new Object("Nikolaus"));
    tmp->adopt(new Object("Jacob"));
    tmp->adopt(new Object("Nikolaus"));
    tmp = tmp->adopt(new Object("Johann"));
    tmp->adopt(new Object("Nicolaus"));
    tmp->adopt(new Object("Daniel"));
    tmp->adopt(new Object("Johann"));
    Value res;

    res = root.call("/.list", Value(""));
    assert_true(res.is_list());
    assert_equal(6, res.size());
    assert_equal(".error", res[0].str());
    assert_equal(".info", res[1].str());
    assert_equal(".list", res[2].str());
    assert_equal(".type", res[3].str());
    assert_equal(".tree", res[4].str());
    assert_equal("Nikolaus/", res[5].str());
    
    res = root.call("/.list", Value("/Nikolaus"));
    assert_true(res.is_list());
    assert_equal(3, res.size());
    assert_equal("Jacob",    res[0].str());
    assert_equal("Nikolaus", res[1].str());
    assert_equal("Johann/",  res[2].str());
    
    res = root.call("/.list", Value("/Nikolaus/Johann"));
    assert_true(res.is_list());
    assert_equal(3, res.size());
    assert_equal("Nicolaus", res[0].str());
    assert_equal("Daniel",   res[1].str());
    assert_equal("Johann",   res[2].str());
  }
  
  void test_list_on_empty( void ) {
    Root root;
    root.adopt(new Object("Nikolaus"));
    Value res;

    res = root.call("/.list", Value("/Nikolaus"));
    assert_true(res.is_list());
    assert_equal(0,  res.size());
    assert_equal("", res.type_tag());
  }
};