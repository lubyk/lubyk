#include "test_helper.h"
#include "oscit/root.h"
#include "mock/dummy_object.h"
#include "mock/log_object.h"
#include "ip/UdpSocket.h"

#define ROOT1_PORT 7014
#define ROOT2_PORT 7015

class OscCommandTest : public TestHelper
{
 public:
  OscCommandTest() : root_end_point_("localhost", ROOT1_PORT) {
    root_.adopt_command(new OscCommand(ROOT1_PORT));
    
    sender_ = root2_.adopt_command(new OscCommand(ROOT2_PORT));
    reply_  = root2_.adopt(new LogObject(".reply"));
  }
  
  void setUp() {
    reply_->clear();
    root_.Object::clear(); // empty root but keep commands
  }
  
  void test_send_receive( void ) {
    DummyObject * foo = root_.adopt(new DummyObject("foo", 1.0));
    
    send("/foo", 3.0);
    assert_equal(3.0, foo->real());
    assert_equal("[\"/foo\", 3]\n", reply());
  }
  
  void test_send_empty_should_receive_value( void ) {
    DummyObject * foo = root_.adopt(new DummyObject("foo", 1.25));
    
    send("/foo", Value());
    assert_equal(1.25, foo->real());
    assert_equal("[\"/foo\", 1.25]\n", reply());
  }
  
  void test_send_nil_should_receive_value( void ) {
    DummyObject * foo = root_.adopt(new DummyObject("foo", 1.25));

    send("/foo", gNilValue);
    assert_equal(1.25, foo->real());
    assert_equal("[\"/foo\", 1.25]\n", reply());
  }
  
 private:
  void send(const char *url, Real real) {
    send(url, Value(real));
  }
  
  void send(const char *url, const Value &val) {
    sender_->send(root_end_point_, url, val);
    millisleep(1);
  }
  
  std::string reply() {
    millisleep(1);
    return reply_->str();
  }
  
  IpEndpointName root_end_point_;
  Root root_;
  Root root2_;
  OscCommand * sender_;
  LogObject * reply_;
};
