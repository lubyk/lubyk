#include "test_helper.h"
#include "oscit/root.h"
#include "mock/dummy_object.h"
#include "oscit/log_object.h"
#include "ip/UdpSocket.h"
#include "oscit/osc_map_command.h"

#define APP1_PORT 7016
#define APP2_PORT 7017

/** In these tests, we have the following setup:
 *  app1_  : contains /foo  + /map     (OscMapCommand)
 *  app2_  : contains /slider/1 + app2_sender_  (OscCommand)
 *  /map receives messages on port APP1_PORT sent by app2_
 *  app1_ notifies changes to app2_ on port APP2_PORT through /map command.
 */
class OscMapCommandTest : public TestHelper
{
 public:
  OscMapCommandTest() : app1_end_point_("localhot", APP1_PORT) {
                                                  // IN      // OUT
    map_cmd_ = app1_.adopt_command(new OscMapCommand(APP1_PORT, APP2_PORT));
    
    app2_sender_  = app2_.adopt_command(new OscCommand(APP2_PORT));
    Object *o = app2_.adopt(new Object("slider"));
    app2_log_ = o->adopt(new LogObject("1"));  //   /slider/1
  }
  
  void setUp() {
    app2_log_->clear();
    app1_.Object::clear(); // empty root but keep commands
  }
  
  void test_received_should_map( void ) {
    DummyObject * foo = app1_.adopt(new DummyObject("foo", 1.0));
    Value res = map_cmd_->script(Value("/slider/1 [0,1] --> /foo [10,20]"));
    assert_false(res.is_error());
    send("/slider/1", 0.5);
    assert_equal(15.0, foo->real());
    assert_equal("", reply());
  }
  
  void test_notifications_should_reverse_map( void ) {
    app1_.adopt(new DummyObject("foo", 1.0));
    Value res = map_cmd_->script(Value("/slider/1 [0,1] --> /foo [10,20]"));
    assert_false(res.is_error());
    app1_.call("/foo", Value(17.5), &context_);
    // notification should be sent to /slider/1 0.75
    assert_equal("0.75\n", reply());
  }
  
 private:
  void send(const char *url, Real real) {
    send(url, Value(real));
  }
  
  void send(const char *url, const Value &val) {
    app2_sender_->send(app1_end_point_, url, val);
    millisleep(50);
  }
  
  std::string reply() {
    millisleep(50);
    return app2_log_->str();
  }
  
  Mutex context_;
  IpEndpointName app1_end_point_;
  Root app1_;
  Root app2_;
  OscCommand *app2_sender_;
  OscMapCommand *map_cmd_;
  LogObject *app2_log_;
};
