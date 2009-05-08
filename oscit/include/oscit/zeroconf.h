#ifndef OSCIT_INCLUDE_OSCIT_ZEROCONF_H_
#define OSCIT_INCLUDE_OSCIT_ZEROCONF_H_
#include <string>

#include "oscit/mutex.h"

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
  const char *service_type_;
  uint16_t    port_;

 private:
  class Implementation;
  Implementation *impl_;
};

/** This class let's you easily register an application as providing a certain type of
 *  service.
 */
class ZeroConfBrowser : public Mutex {
 public:
  ZeroConfBrowser(const char *service_type);

  virtual ~ZeroConfBrowser();

  virtual void add_device(const char *name, const char *host, unsigned int port, bool more_coming) {
    printf("add_device %s @ %s:%i%s\n", name, host, port, more_coming ? " (more coming)" : "");
  }

  virtual void remove_device(const char *name, bool more_coming) {
    printf("remove_device %s%s\n", name, more_coming ? " (more coming)" : "");
  }

 protected:
  /** This method *must* be called from sub-classes in their destructors to
   * make sure the callbacks (add_device, remove_device) are not called in the
	 * middle of a class destruction.
	 */
  virtual void stop();

  const char *service_type_;

 private:
  class Implementation;
  Implementation *impl_;
};

} // namespace oscit

#endif // OSCIT_INCLUDE_OSCIT_ZEROCONF_H_
