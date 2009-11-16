#include "test_helper.h"
#include "oscit/root.h"
#include "oscit/root_proxy.h"
#include "mock/command_logger.h"
#include "mock/proxy_factory_logger.h"

class RootProxyTest : public TestHelper
{
public:
  void test_should_send_commands_to_mirror_remote_tree( void ) {
    Root root;
    std::ostringstream logger(std::ostringstream::out);
    ProxyFactoryLogger factory("factory", &logger);
    Location location("oscit", "my place");
    CommandLogger *cmd = root.adopt_command(new CommandLogger("oscit", &logger));
    assert_equal("[oscit: listen]", logger.str());
    logger.str("");
    cmd->adopt_proxy(factory.build_and_init_root_proxy(location));
    assert_equal("[factory: build_root_proxy oscit://\"my place\"][oscit: send oscit://\"my place\" /.list_with_type \"\"]", logger.str());
  }

  void test_should_propagate_root_proxy_when_building_object_proxy( void ) {

  }
};
