#ifndef OSCIT_INCLUDE_OSCIT_ZEROCONF_BROWSER_H_
#define OSCIT_INCLUDE_OSCIT_ZEROCONF_BROWSER_H_
#include <string>

#include "oscit/mutex.h"
#include "oscit/location.h"
#include "oscit/root_proxy.h"

namespace oscit {

#define FOUND_DEVICE_HASH_SIZE 100

class ProxyFactory;
class Command;

/** This class let's you easily found applications providing a certain
 *  service.
 */
class ZeroConfBrowser : public Mutex {
 public:
  ZeroConfBrowser(const char *service_type);

  virtual ~ZeroConfBrowser();

  /** This method is called just after a new proxy has been added to the list.
   */
  virtual void added_proxy(RootProxy *proxy) {}

  /** This method is called just before the proxy is actually deleted.
   */
  virtual void removing_proxy(RootProxy *proxy) {}

  template<class T>
  T *adopt_proxy_factory(T *factory) {
    do_adopt_proxy_factory(factory);
    return factory;
  }

  void set_command(Command *command);

  bool get_location_from_name(const char *service_name, Location *location) const;

  /** @internal. */
  void add_device(const Location &location);

  /** @internal. */
  void remove_device(const char *name);

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

  THash<std::string, Location> found_devices_;
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_ZEROCONF_REGISTRATION_H_
