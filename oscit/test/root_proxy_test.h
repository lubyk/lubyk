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
    assert_equal("[oscit: listen]", logger.str());
    logger.str("");
    cmd->adopt_proxy(factory.build_and_init_root_proxy(location));
    assert_equal("[factory: build_root_proxy oscit://\"my place\"][oscit: send oscit://\"my place\" /.list_with_type \"\"]", logger.str());
  }

  void test_route_reply_messages_to_object_proxies( void ) {
    RootProxy proxy(Location("osc", "funky synth"));
    Logger logger;
    proxy.adopt(new ObjectProxyLogger("seven", RangeIO(0.0, 2000.0, "bpm", "the sky is blue"), &logger));
    logger.str("");
    proxy.handle_reply(std::string("/seven"), Value(300.0));
    assert_equal("[seven: value_changed 300]", logger.str());
  }
};
