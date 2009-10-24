#ifndef OSCIT_INCLUDE_OSCIT_ZEROCONF_H_
#define OSCIT_INCLUDE_OSCIT_ZEROCONF_H_
#include <string>

#include "oscit/mutex.h"
#include "oscit/location.h"

namespace oscit {

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
    std::cout << "add_device " << location.name() << " @ " << location << std::endl;
  }

  virtual void remove_device(const char *name) {
    std::cout << "remove_device " << name << std::endl;
  }

 protected:
  /** This method *must* be called from sub-classes in their destructors to
   * make sure the callbacks (add_device, remove_device) are not called in the
	 * middle of a class destruction.
	 */
  virtual void stop();

  void get_protocol_from_service_type() {
    size_t dot_index = service_type_.find(".");
    if (dot_index != std::string::npos) {
      protocol_ = service_type_.substr(1, dot_index);
    } else {
      // Bad service type
      std::cerr << "Could not get protocol from service type: " << service_type_ << "\n";
    }
  }

  std::string protocol_;
  std::string service_type_;

 private:
  class Implementation;
  Implementation *impl_;
};

} // namespace oscit

#endif // OSCIT_INCLUDE_OSCIT_ZEROCONF_H_
