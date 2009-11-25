#ifndef OSCIT_INCLUDE_OSCIT_ZEROCONF_BROWSER_H_
#define OSCIT_INCLUDE_OSCIT_ZEROCONF_BROWSER_H_
#include <string>

#include "oscit/mutex.h"
#include "oscit/location.h"

namespace oscit {

class ProxyFactory;
class Command;

/** This class let's you easily found applications providing a certain
 *  service.
 */
class ZeroConfBrowser : public Mutex {
 public:
  ZeroConfBrowser(const char *service_type);

  virtual ~ZeroConfBrowser();

  virtual void add_device(const Location &location) {
    add_proxy(location);
  }

  virtual void remove_device(const char *name) {
    remove_proxy(Location(protocol_.c_str(), name));
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

  void add_proxy(const Location &location);

  void remove_proxy(const Location &location);

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

#endif // OSCIT_INCLUDE_OSCIT_ZEROCONF_REGISTRATION_H_
