#include <sstream>
#include "test_helper.h"
#include "oscit/mutex.h"
#include "mock/dummy_object.h"

class CallTest : public TestHelper
{  
public:
  void test_store_call( void ) {
    Root root;
    DummyObject * evil = root.adopt(new DummyObject("Evil", 1.23));
    
    Call call("/Evil", Value(6.66));

    assert_equal(1.23, evil->real());
    Value res = call.safe_trigger(&root, NULL);
    assert_equal(6.66, res.r);
    assert_equal(6.66, evil->real());
  }
};