#ifndef OSCIT_INCLUDE_OSCIT_ZEROCONF_REGISTRATION_H_
#define OSCIT_INCLUDE_OSCIT_ZEROCONF_REGISTRATION_H_
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

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_ZEROCONF_REGISTRATION_H_
