#include "test_helper.h"
#include "oscit/root.h"
#include "oscit/log_object.h"
#include "oscit/list_meta_method.h"
#include "oscit/list_with_type_meta_method.h"

#include "mock/dummy_object.h"
#include "mock/osc_command_logger.h"

class OscCommandTest : public TestHelper
{
 public:
  enum {
    RECEIVER_PORT = 7014,
    SENDER_PORT   = 7015
  };

  OscCommandTest() : remote_end_point_(Location::LOOPBACK, RECEIVER_PORT) {
    remote_.adopt_command(new OscCommandLogger(RECEIVER_PORT, "receiver", &reply_));

    sender_ = local_.adopt_command(new OscCommandLogger(SENDER_PORT, "sender", &reply_));
    // we need to register in order to get return values
    send("/.register", gNilValue);
  }

  void setUp() {
    sender_->clear_reply();
    reply_.str("");
    remote_.Object::clear(); // empty root but keep commands
  }

  void test_send_receive( void ) {
    DummyObject * foo = remote_.adopt(new DummyObject("foo", 1.0));

    send("/foo", 3.0);
    assert_equal(3.0, foo->real());
    assert_equal("[\"/foo\", 3]\n", reply());
  }

  void test_send_empty_should_receive_value( void ) {
    DummyObject * foo = remote_.adopt(new DummyObject("foo", 1.25));

    send("/foo", Value());
    assert_equal(1.25, foo->real());
    assert_equal("[\"/foo\", 1.25]\n", reply());
  }

  void test_send_nil_should_receive_value( void ) {
    DummyObject * foo = remote_.adopt(new DummyObject("foo", 1.25));

    send("/foo", gNilValue);
    assert_equal(1.25, foo->real());
    assert_equal("[\"/foo\", 1.25]\n", reply());
  }

  void test_send_receive_list( void ) {
    // remote_ objects are cleared before each run
    remote_.adopt(new ListMetaMethod(Url(LIST_PATH).name()));
    Object * tmp = remote_.adopt(new Object("monitor"));
    tmp->adopt(new DummyObject("mode", "rgb", SelectIO("rgb, yuv", "color mode", "This is a menu.")));
    tmp->adopt(new DummyObject("tint", 45.0, RangeIO(1, 127, "tint", "This is a slider from 1 to 127.")));

    send(LIST_PATH, "/monitor");
    assert_equal("[\"/.list\", [\"/monitor\", [\"mode\", \"tint\"]]]\n", reply());
  }

  void test_send_receive_list_on_empty( void ) {
    // remote_ objects are cleared before each run
    remote_.adopt(new ListMetaMethod(Url(LIST_PATH).name()));
    remote_.adopt(new Object("monitor"));

    send(LIST_PATH, "/monitor");
    assert_equal("[\"/.list\", [\"/monitor\", []]]\n", reply());
  }

  void test_send_receive_list_with_type( void ) {
    // remote_ objects are cleared before each run
    remote_.adopt(new ListWithTypeMetaMethod(Url(LIST_WITH_TYPE_PATH).name()));
    Object * tmp = remote_.adopt(new Object("monitor"));
    tmp->adopt(new DummyObject("mode", "rgb", SelectIO("rgb, yuv", "color mode", "This is a menu.")));
    tmp->adopt(new DummyObject("tint", 45.0, RangeIO(1, 127, "tint", "This is a slider from 1 to 127.")));

    send(LIST_WITH_TYPE_PATH, "/monitor");
    assert_equal("[\"/.list_with_type\", [\"/monitor\", [[\"mode\", [\"rgb\", \"rgb, yuv\", \"color mode\", \"This is a menu.\"]], [\"tint\", [45, 1, 127, \"tint\", \"This is a slider from 1 to 127.\"]]]]]\n", reply());
  }

 private:
  void send(const char *url, Real real) {
    send(url, Value(real));
  }

  void send(const char *url, const char *str) {
    send(url, Value(str));
  }

  void send(const char *url, const Value &val) {
    sender_->send(remote_end_point_, url, val);
    millisleep(10);
  }

  const std::string reply() {
    millisleep(10);
    return sender_->reply();
  }

  Location remote_end_point_;
  Logger reply_;
  Root remote_;
  Root local_;
  OscCommandLogger *sender_;
};
