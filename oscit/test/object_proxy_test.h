#include "test_helper.h"
#include "oscit/root.h"
#include "oscit/root_proxy.h"
#include "mock/command_logger.h"
#include "mock/proxy_factory_logger.h"
#include "mock/object_proxy_logger.h"

class ObjectProxyTest : public TestHelper
{
public:

  void test_should_set_root_proxy_when_adopted( void ) {
    RootProxy proxy(Location("osc", "funky synth"));
    Logger logger;
    assert_equal("", logger.str());
    ObjectProxyLogger *object = new ObjectProxyLogger("foobar", Value("info"), &logger);
    object->set_stream(&logger);
    proxy.adopt(object);
    assert_equal("[foobar: set_root RootProxy]", logger.str());
    assert_equal(&proxy, object->root_proxy());
  }

  void test_should_receive_value_changed_from_rootproxy( void ) {
    RootProxy proxy(Location("osc", "funky synth"));
    Logger logger;
    proxy.adopt(new ObjectProxyLogger("seven", RangeIO(0.0, 2000.0, "bpm", "the sky is blue"), &logger));
    logger.str("");
    proxy.handle_reply(std::string("/seven"), Value(300.0));
    assert_equal("[seven: value_changed 300]", logger.str());
  }

  void test_update_remote_on_set_value( void ) {
    Logger logger;
    CommandLogger cmd("osc", &logger);
    RootProxy *proxy = cmd.adopt_proxy(new RootProxy(Location("osc", "funky synth")));
    ObjectProxyLogger *obj = proxy->adopt(new ObjectProxyLogger("seven", RangeIO(0.0, 2000.0, "bpm", "the sky is blue"), &logger));
    logger.str("");
    obj->set_value(Value(45.0));
    assert_equal("[osc: send osc://\"funky synth\" /seven 45]", logger.str());
  }

  void test_latency( void ) {
    Logger logger;
    CommandLogger cmd("osc", &logger);
    RootProxy *proxy = cmd.adopt_proxy(new RootProxy(Location("osc", "funky synth")));
    ObjectProxyLogger *obj = proxy->adopt(new ObjectProxyLogger("seven", RangeIO(0.0, 2000.0, "bpm", "the sky is blue"), &logger));
    logger.str("");
    obj->set_value(Value(45.0));
    millisleep(12);
    obj->handle_value_change(Value(45.0));
    assert_true( 11 <= obj->latency() && obj->latency() <= 13 );
  }

  void test_sync_should_call_list_with_type( void ) {
    Logger logger;
    CommandLogger cmd("osc", &logger);
    RootProxy *proxy = cmd.adopt_proxy(new RootProxy(Location("osc", "funky synth")));
    ObjectProxyLogger *obj = proxy->adopt(new ObjectProxyLogger("seven", RangeIO(0.0, 2000.0, "bpm", "the sky is blue"), &logger));
    logger.str("");
    obj->sync_children();
    assert_equal("[osc: send osc://\"funky synth\" /.list_with_type \"/seven\"]", logger.str());
  }

  void test_new_object_without_type_should_try_to_find_type( void ) {
    Logger logger;
    CommandLogger cmd("osc", &logger);
    RootProxy *proxy = cmd.adopt_proxy(new RootProxy(Location("osc", "funky synth")));
    logger.str("");
    proxy->adopt(new ObjectProxy("seven", gNilValue));
    assert_equal("[osc: send osc://\"funky synth\" /.type \"/seven\"]", logger.str());
  }

  void test_until_type_is_set_object_proxy_should_respond_false_to_is_connected( void ) {
    ObjectProxy o("name", gNilValue);
    assert_false( o.is_connected() );
    Value res = o.trigger(gNilValue);
    assert_true( res.is_nil() );
    o.set_type(RangeIO(0.0, 200.0, "bpm", "hop hop"));
    assert_true( o.is_connected() );
    res = o.trigger(gNilValue);
    assert_equal(Value(0.0), res);
  }
};
