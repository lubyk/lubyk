#include "test_helper.h"
#include "oscit/root.h"
#include "mock/dummy_object.h"

class TreeMetaMethodTest : public TestHelper
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
  void test_tree( void ) {
    Root root;
    Object * tmp = root.adopt(new Object("Nikolaus"));
    tmp->adopt(new Object("Jacob"));
    tmp->adopt(new Object("Nikolaus"));
    tmp = tmp->adopt(new Object("Johann"));
    tmp->adopt(new Object("Nicolaus"));
    tmp->adopt(new Object("Daniel"));
    tmp->adopt(new Object("Johann"));
    Value res;

    res = root.call("/.tree", Value(""));
    assert_true(res.is_list());
    assert_equal(13, res.size());
    assert_equal("[\".error\", \".info\", \".list\", \".list_with_type\", \".type\", \".tree\", \"Nikolaus\", \"Nikolaus/Jacob\", \"Nikolaus/Nikolaus\", \"Nikolaus/Johann\", \"Nikolaus/Johann/Nicolaus\", \"Nikolaus/Johann/Daniel\", \"Nikolaus/Johann/Johann\"]", res.to_json());
    
    res = root.call("/.tree", Value("/Nikolaus"));
    assert_true(res.is_list());
    assert_equal(6, res.size());
    assert_equal("[\"Jacob\", \"Nikolaus\", \"Johann\", \"Johann/Nicolaus\", \"Johann/Daniel\", \"Johann/Johann\"]", res.to_json());
    
    res = root.call("/.tree", Value("/Nikolaus/Johann"));
    assert_true(res.is_list());
    assert_equal(3, res.size());
    assert_equal("[\"Nicolaus\", \"Daniel\", \"Johann\"]", res.to_json());
    Value res2 = root.call("/.tree", Value("/Nikolaus/Johann"));
    assert_equal(res2.to_json(), res.to_json()); // should be the same as list if there are no children
  }
  
  void test_tree_on_empty( void ) {
    Root root;
    root.adopt(new Object("Zorglub"));
    Value res;

    res = root.call("/.tree", Value("/Zorglub"));
    assert_true(res.is_nil());
    assert_equal(0, res.size());
  }
  
  void test_tree_with_nil( void ) {
    Root root(NoIO("This is the root node."));
    Value res;
    
    res = root.call("/.tree", gNilValue);
    assert_true(res.is_nil());
  }
};