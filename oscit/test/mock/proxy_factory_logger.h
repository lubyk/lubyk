#ifndef OSCIT_TEST_MOCK_COMMAND_NOTIFICATION_LOGGER_H_
#define OSCIT_TEST_MOCK_COMMAND_NOTIFICATION_LOGGER_H_

#include "oscit/location.h"
#include "oscit/proxy_factory.h"
#include "oscit/root_proxy.h"
#include "oscit/object_proxy.h"
#include "mock/logger.h"

template<class T, class U>
class TProxyFactoryLogger : public ProxyFactory, protected MockLogger {
public:
  TProxyFactoryLogger(const char *name,std::ostringstream *stream) :
                     ProxyFactory(),
                     MockLogger(name, stream) {}


  virtual RootProxy *build_root_proxy(const Location &end_point) {
    log("build_root_proxy", end_point);
    return new T(end_point, this);
  }

  virtual ObjectProxy *build_object_proxy(const std::string &name, const Value &type) {
    log("build_object_proxy", name, type);
    return new U(name, type);
  }
};

typedef TProxyFactoryLogger<RootProxy, ObjectProxy> ProxyFactoryLogger;

#endif // OSCIT_TEST_MOCK_COMMAND_NOTIFICATION_LOGGER_H_