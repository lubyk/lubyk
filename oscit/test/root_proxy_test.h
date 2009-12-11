#include "test_helper.h"
#include "oscit/root.h"
#include "oscit/root_proxy.h"
#include "mock/command_logger.h"
#include "mock/proxy_factory_logger.h"
#include "mock/object_proxy_logger.h"

class RootProxyTest : public TestHelper
{
public:
  void test_should_send_commands_to_mirror_remote_tree( void ) {
    Root root;
    Logger logger;
    ProxyFactoryLogger factory("factory", &logger);
    Location location("oscit", "my place");
    CommandLogger *cmd = root.adopt_command(new CommandLogger("oscit", &logger));
    assert_equal("[oscit: listen][oscit: .]", logger.str());
    logger.str("");
    cmd->adopt_proxy(factory.build_and_init_root_proxy(location));
    assert_equal("[factory: build_root_proxy oscit://\"my place\"][oscit: send oscit://\"my place\" /.register null][oscit: send oscit://\"my place\" /.list_with_type \"\"]", logger.str());
  }

  void test_route_reply_messages_to_object_proxies( void ) {
    RootProxy proxy(Location("osc", "funky synth"));
    Logger logger;
    proxy.adopt(new ObjectProxyLogger("seven", RangeIO(0.0, 2000.0, "bpm", "the sky is blue"), &logger));
    logger.str("");
    proxy.handle_reply(std::string("/seven"), Value(300.0));
    assert_equal("[seven: value_changed 300]", logger.str());
  }

  void test_should_build_object_proxies_without_type( void ) {
    RootProxy proxy(Location("osc", "funky synth"));
    Logger logger;
    ProxyFactoryLogger factory("factory", &logger);
    proxy.set_proxy_factory(&factory);

    Object *object = proxy.object_at("/one/two");
    assert_true(object == NULL);
    object = proxy.object_at("/one");
    assert_true(object == NULL);

    Value error;
    Object *object2 = proxy.find_or_build_object_at(std::string("/one/two"), &error);
    assert_equal("[factory: build_object_proxy one null][factory: build_object_proxy two null]", logger.str());
    logger.str("");
    assert_false(object2 == NULL);

    object = proxy.object_at("/one/two");
    assert_equal(object2, object);
    assert_equal(object->type(), gNilValue);

    object = proxy.object_at("/one");
    assert_false(object == NULL);
    assert_equal(object->type(), gNilValue);
    assert_equal("", logger.str());
  }
};
