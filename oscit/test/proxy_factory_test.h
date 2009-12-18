#include "test_helper.h"
#include "oscit/root.h"
#include "oscit/root_proxy.h"
#include "oscit/object_proxy.h"
#include "oscit/proxy_factory.h"
#include "mock/dummy_object.h"
#include "mock/object_proxy_logger.h"

/** Integration tests for the ProxyFactory pattern (local proxy kept in sync with remote values).
 */


/* Widgets */
class MyRootProxy : public RootProxy {
public:
  MyRootProxy(const Location &location) : RootProxy(location) {}
};

class MyObjectProxy : public ObjectProxyLogger {
public:
  MyObjectProxy(const std::string &name, const Value &type) : ObjectProxyLogger(name, type) {}
};


class MyProxyFactory : public ProxyFactory {
public:
  MyProxyFactory() : built_(10) {}

  virtual ObjectProxy *build_object_proxy(Object *parent, const std::string &name, const Value &type) {
    if (is_meta_method(name)) return NULL;
    ObjectProxyLogger *object_proxy = new MyObjectProxy(name, type);
    built_.set(name, object_proxy);
    return object_proxy;
  }

  virtual RootProxy *build_root_proxy(const Location &location) {
    return new MyRootProxy(location);
  }

  ObjectProxyLogger *find_by_name(const char *name) {
    ObjectProxyLogger *object_proxy = NULL;
    built_.get(std::string(name), &object_proxy);
    return object_proxy;
  }

private:
  THash<std::string, ObjectProxyLogger*> built_;
};

/*
local + OscCommand = local
remote + OscCommand = remote
proxy = proxy of remote remote in local context
*/

class ProxyFactoryTest : public TestHelper
{
 public:
  enum {
    REMOTE_PORT = 7019,
    LOCAL_PORT = 7018
  };

  void test_build_and_sync( void ) {
    // prepare remote root
    Root remote;
    remote.adopt_command(new OscCommand(REMOTE_PORT));
    Object *monitor = remote.adopt(new Object("monitor"));
    monitor->adopt(new DummyObject("mode", "rgb", SelectIO("rgb, yuv", "color mode", "This is a menu.")));
    monitor->adopt(new DummyObject("tint", 45.0, RangeIO(1, 127, "tint", "This is a slider from 1 to 127.")));

    // prepare local root
    Root local;
    OscCommand *cmd = local.adopt_command(new OscCommand(LOCAL_PORT));

    // factory
    MyProxyFactory factory;

    // build proxy
    MyRootProxy *proxy = (MyRootProxy*) factory.build_and_init_root_proxy(Location("oscit", Location::LOOPBACK, REMOTE_PORT));

    // sync first level (root children)
    cmd->adopt_proxy(proxy); // sync ----> cmd ----> remote -----> cmd -----> "/.reply" ----> proxy
    millisleep(100);
    Value res = proxy->list();
    assert_equal("[\"monitor\"]", res.to_json());

    // ProxyFactory "should set itself as factory when creating a RootProxy"
    // same factory used to build root and objects ==> OK
    ObjectProxy *object_proxy = factory.find_by_name("monitor");
    assert_true(object_proxy);

    // ProxyFactory "should build subclasses of ObjectProxy"
    assert_true(object_proxy->kind_of(ObjectProxy));

    res = object_proxy->list();
    assert_equal("[]", res.to_json());

    object_proxy->sync_children();
    millisleep(100);
    res = object_proxy->list();
    assert_equal("[\"mode\", \"tint\"]", res.to_json());
  }

  void test_local_cache_should_reflect_remote( void ) {
    Root local;
    Root remote;
    Logger logger;
    MyProxyFactory factory;
    build_foobar_local_and_remote(local, remote, factory, logger);

    Object *bar = proxy_->object_at("/bar");
    Value res = bar->trigger(gNilValue);
    assert_equal("45", res.to_json());

    // should have same type as remote
    assert_equal("[45, 1, 127, \"tint\", \"This is a slider from 1 to 127.\"]", bar->type().to_json());
    assert_equal("[45, 1, 127, \"tint\", \"This is a slider from 1 to 127.\"]", bar_->type().to_json());
  }

  void test_call_local_proxy_should_find_cached_value( void ) {
    Root local;
    Root remote;
    Logger logger;
    MyProxyFactory factory;
    build_foobar_local_and_remote(local, remote, factory, logger);

    assert_equal("", logger.str());
    Value res = proxy_->call("/bar");
    assert_equal("45", res.to_json());
  }

  void test_change_local_proxy_should_update_remote( void ) {
    Root local;
    Root remote;
    Logger logger;
    MyProxyFactory factory;
    build_foobar_local_and_remote(local, remote, factory, logger);

    Value res = proxy_->call("/bar", Value(33.0));
    assert_equal("45", res.to_json()); // receive cached value (async)
    millisleep(10);
    assert_equal("33", remote.call("/bar").to_json());     // remote is updated
    assert_equal("[bar: value_changed 33]", logger.str()); // local receives 'value_changed'

    assert_equal("33", proxy_->call("/bar").to_json()); // new value is in cache now
  }

private:

  /** Builds a setup where the remote tree contains real objects ('foo' and 'bar') and
   * the local tree has an OscCommand and builds the proxies.
   */
  void build_foobar_local_and_remote(Root &local, Root &remote, MyProxyFactory &factory,  Logger &logger) {

    remote.adopt_command(new OscCommand(REMOTE_PORT));
    foo_ = remote.adopt(new DummyObject("foo", "rgb", SelectIO("rgb, yuv", "color mode", "This is a menu.")));
    bar_ = remote.adopt(new DummyObject("bar", 45.0, RangeIO(1, 127, "tint", "This is a slider from 1 to 127.")));

    OscCommand *cmd = local.adopt_command(new OscCommand(LOCAL_PORT));


    // build proxy
    proxy_ = (MyRootProxy*) factory.build_and_init_root_proxy(Location("oscit", Location::LOOPBACK, REMOTE_PORT));

    // sync first level (root children)
    cmd->adopt_proxy(proxy_); // sync ----> cmd ----> remote -----> cmd -----> "/.reply" ----> proxy
    millisleep(10);

    ObjectProxyLogger *object_proxy = factory.find_by_name("foo");
    object_proxy->set_stream(&logger);

    object_proxy = factory.find_by_name("bar");
    object_proxy->set_stream(&logger);
  }

  DummyObject *foo_;
  DummyObject *bar_;
  MyRootProxy *proxy_;
};
