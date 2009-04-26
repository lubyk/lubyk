#include "test_helper.h"

class NodeTest : public TestHelper {
public:
  void test_outlet_for_value( void ) {
    Root base;
    Real value = 0;
    DummyNode *sender = base.adopt(new DummyNode(&value));
    Object *out  = sender->adopt(new Object("out"));
    Outlet *ping = out->adopt(new Outlet(sender, "ping", RealIO("any", "Receive real values.")));
    
    Outlet *outlet = sender->outlet_for_value(1, Value(1.0));
    assert_equal(ping, outlet);
    outlet = sender->outlet_for_value(1, gNilValue);
    assert_equal(ping, outlet);
    outlet = sender->outlet_for_value(1, Value("hello"));
    assert_equal((Outlet*)NULL, outlet);
  }
};