#ifndef OSCIT_INCLUDE_OSCIT_ZEROCONF_H_
#define OSCIT_INCLUDE_OSCIT_ZEROCONF_H_
#include <string>

#include "oscit/mutex.h"
#include "oscit/location.h"

namespace oscit {

class ProxyFactory;
class Command;

/** This class let's you easily register an application as providing a certain type of
 *  service.
 */
class ZeroConfRegistration : public Mutex {
 public:
  ZeroConfRegistration(const std::string &name, const char *service_type, uint16_t port);

  virtual ~ZeroConfRegistration();

  virtual void registration_done() {}

 protected:
  /** This method *must* be called from sub-classes in their destructors to
   * make sure the callback (registration_done) is not called in the middle of
	 * a class destruction.
	 */
  virtual void stop();

  std::string name_;
  std::string host_;
  std::string service_type_;
  uint16_t    port_;

 private:
  class Implementation;
  Implementation *impl_;
};

/** This class let's you easily found applications providing a certain
 *  service.
 */
class ZeroConfBrowser : public Mutex {
 public:
  ZeroConfBrowser(const char *service_type);

  virtual ~ZeroConfBrowser();

  virtual void add_device(const Location &location) {
    build_proxy(location);
    std::cout << "add_device " << location.name() << " @ " << location << std::endl;
  }

  virtual void remove_device(const char *name) {
    std::cout << "remove_device " << name << std::endl;
  }

  template<class T>
  T *adopt_proxy_factory(T *factory) {
    do_adopt_proxy_factory(factory);
    return factory;
  }

  void set_command(Command *command);

 protected:
  /** This method *must* be called from sub-classes in their destructors to
   * make sure the callbacks (add_device, remove_device) are not called in the
	 * middle of a class destruction.
	 */
  virtual void stop();

  void get_protocol_from_service_type();

  void build_proxy(const Location &location);

  std::string   protocol_;
  std::string   service_type_;
  Command      *command_;
  ProxyFactory *proxy_factory_;

 private:
  void do_adopt_proxy_factory(ProxyFactory *factory);

  class Implementation;
  Implementation *impl_;
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_ZEROCONF_H_
