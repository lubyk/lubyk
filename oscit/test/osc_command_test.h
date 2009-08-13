#include "test_helper.h"
#include "oscit/root.h"
#include "mock/dummy_object.h"
#include "oscit/log_object.h"
#include "oscit/list_meta_method.h"
#include "oscit/list_with_type_meta_method.h"
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
  
  void test_send_receive_list( void ) {
    // root_ objects are cleared before each run
    root_.adopt(new ListMetaMethod(".list"));
    Object * tmp = root_.adopt(new Object("monitor"));
    tmp->adopt(new DummyObject("mode", 0.0, SelectIO("rgb, yuv", "color mode", "This is a menu.")));
    tmp->adopt(new DummyObject("tint", 45.0, RangeIO(1, 127, "tint", "This is a slider from 1 to 127.")));
    
    send("/.list", "/monitor");
    assert_equal("[\"/.list\", [\"/monitor\", [\"mode\", \"tint\"]]]\n", reply());
  }
  
  void test_send_receive_list_on_empty( void ) {
    // root_ objects are cleared before each run
    root_.adopt(new ListMetaMethod(".list"));
    root_.adopt(new Object("monitor"));
    
    send("/.list", "/monitor");
    assert_equal("[\"/.list\", [\"/monitor\", []]]\n", reply());
  }
  
  void test_send_receive_list_with_type( void ) {
    // root_ objects are cleared before each run
    root_.adopt(new ListWithTypeMetaMethod(".list_with_type"));
    Object * tmp = root_.adopt(new Object("monitor"));
    tmp->adopt(new DummyObject("mode", 0.0, SelectIO("rgb, yuv", "color mode", "This is a menu.")));
    tmp->adopt(new DummyObject("tint", 45.0, RangeIO(1, 127, "tint", "This is a slider from 1 to 127.")));
    
    send("/.list_with_type", "/monitor");
    assert_equal("[\"/.list_with_type\", [\"/monitor\", [[\"mode\", [\"hello\", \"rgb, yuv\", \"color mode\", \"This is a menu.\"]], [\"tint\", [45, 1, 127, \"tint\", \"This is a slider from 1 to 127.\"]]]]]\n", reply());
  }
  
 private:
  void send(const char *url, Real real) {
    send(url, Value(real));
  }
  
  void send(const char *url, const char *str) {
    send(url, Value(str));
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
