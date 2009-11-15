// #include "test_helper.h"
// #include "oscit/root.h"
// #include "mock/dummy_object.h"
// #include "oscit/log_object.h"
// #include "oscit/list_meta_method.h"
// #include "oscit/list_with_type_meta_method.h"
//
// /* Widgets */
// class MyRootProxy : public RootProxy
// {
//   MyRoot(const Location &location) : RootProxy(location) {}
// };
//
// class MyObjectProxy : public ObjectProxy
// {
//   MyObjectProxy(const std::string &name, const Value &type) : ObjectProxy(name, type) {}
// };
//
//
// class MyProxyFactory : ProxyFactory
// {
//   virtual ObjectProxy *build_object_proxy(const std::string &name, const Value &type) {
//     return new MyObjectProxy(name, type);
//   }
//
//   virtual RootProxy *build_root_proxy(const Location &location) {
//     return new MyRootProxy(location);
//   }
// };
//
// /*
// root1 + OscCommand = local
// root2 + OscCommand = remote
// proxy = proxy of remote root2 in local context
// */
//
// class OscProxyTest : public TestHelper
// {
//  public:
//   void test_build( void ) {
//     // prepare remote root
//     Root root2;
//     Object * tmp = root2.adopt(new Object("monitor"));
//     tmp->adopt(new DummyObject("mode", 0.0, SelectIO("rgb, yuv", "color mode", "This is a menu.")));
//     Object * tint = tmp->adopt(new DummyObject("tint", 45.0, RangeIO(1, 127, "tint", "This is a slider from 1 to 127.")));
//     root2.adopt_command(new OscCommand(ROOT2_PORT));
//
//     // prepare local root
//     Root root1;
//     OscCommand *cmd = root1.adopt_command(new OscCommand(ROOT1_PORT));
//
//     // factory
//     MyProxyFactory factory;
//
//     // build proxy
//     MyRootProxy *proxy = (MyRootProxy*) factory.build_root_proxy(Location("root2", Location::ANY_IP, ROOT2_PORT));
//     cmd->adopt_proxy(proxy);
//
//     // sync first level (root children)
//     proxy->sync(); // ----> cmd ----> Root2 -----> cmd -----> "/.reply" ----> proxy
//     millisleep(1);
//     Value res = proxy.list();
//   }
// };
