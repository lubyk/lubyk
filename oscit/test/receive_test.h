#include "test_helper.h"
#include "oscit/root.h"
#include "mock/dummy_object.h"

class ReceiveTest : public TestHelper
{
 public:
  ReceiveTest() : sender_("localhost", 7000) {
    root_.open_port(7000);
  }
  
  void setUp() {
    root_.clear();
  }
  
  void test_value_from_osc( void ) {
    
  }
  void test_send_receive( void ) {
    DummyObject * foo = root_.adopt(new DummyObject("foo", 1.0));
    
    send("/foo", 3.0);
    assert_equal(3.0, foo->real());
  }
  
 private:
  void send(const char *url, Real real) {
    send(url, Value(real));
  }
  
  void send(const char *url, const Value &val) {
    sender_.send(url, val);
    microsleep(1);
  }
  
  Root root_;
  OscSend sender_;
};
