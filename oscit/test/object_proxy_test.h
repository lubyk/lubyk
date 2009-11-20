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
    RootProxy proxy(Location("osc", "funky synth"));
    Logger logger;
    CommandLogger cmd("osc", &logger);
    ObjectProxyLogger *obj = proxy.adopt(new ObjectProxyLogger("seven", RangeIO(0.0, 2000.0, "bpm", "the sky is blue"), &logger));
    proxy.set_command(&cmd);
    logger.str("");
    obj->set_value(Value(45.0));
    assert_equal("[osc: send osc://\"funky synth\" /seven 45]", logger.str());
  }

};
