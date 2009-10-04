#include "test_helper.h"
#include "oscit/root.h"
#include "mock/dummy_object.h"
#include "oscit/log_object.h"
#include "oscit/list_meta_method.h"
#include "oscit/list_with_type_meta_method.h"
#include "ip/UdpSocket.h"

/* Widgets */
class M


class DummyProxyFactory : ProxyFactory
{
  virtual ObjectProxy *build_object_proxy(const std::string &name, const Value &type) {
    
  }
  
  virtual RootProxy *build_root_proxy(IpEndpointName* end_point) {
    return new MyRoot();
  }
};

/*
root1 + OscCommand = local
root2 + OscCommand = remote
proxy = proxy of remote root2 in local context
*/

class OscProxyTest : public TestHelper
{
 public:
  void test_build( void ) {
    // prepare remote root
    Root root2;
    Object * tmp = root2.adopt(new Object("monitor"));
    tmp->adopt(new DummyObject("mode", 0.0, SelectIO("rgb, yuv", "color mode", "This is a menu.")));
    Object * tint = tmp->adopt(new DummyObject("tint", 45.0, RangeIO(1, 127, "tint", "This is a slider from 1 to 127.")));
    root2.adopt_command(new OscCommand(ROOT2_PORT));

    // prepare local root
    Root root1;
    OscCommand *cmd = root1.adopt_command(new OscCommand(ROOT1_PORT));

    // build proxy
    OscRootProxy *proxy = cmd->adopt_proxy(new OscRootProxy(IpEndpointName("localhost", ROOT2_PORT)));

    // sync first level (root children)
    proxy->sync();
    millisleep(1);
    Value res = proxy.list();
  }
};
