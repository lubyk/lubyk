#ifndef OSCIT_INCLUDE_OSCIT_ZEROCONF_H_
#define OSCIT_INCLUDE_OSCIT_ZEROCONF_H_
#include <string>

namespace oscit {

/** This class let's you easily register an application as providing a certain type of
 *  service.
 */
class ZeroConfRegistration {
 public:
  ZeroConfRegistration(const std::string &name, const std::string &service_type, uint16_t port);
  
  virtual ~ZeroConfRegistration();
  
  virtual void registration_done() {}
  
 protected:
  std::string name_;
  std::string host_;
  std::string service_type_;
  uint16_t    port_;

 private:
  class Implementation;
  Implementation *impl_;
};

/** This class let's you easily register an application as providing a certain type of
 *  service.
 */
class ZeroConfBrowser {
 public:
  ZeroConfBrowser(const std::string &service_type);
  
  virtual ~ZeroConfBrowser();
  
  virtual void add_device(const char *name, const char *host, unsigned int port, bool more_coming) {
    printf("add_device %s @ %s:%i%s\n", name, host, port, more_coming ? " (more coming)" : "");
  }
  
  virtual void remove_device(const char *name, const char *host, unsigned int port, bool more_coming) {
    printf("remove_device %s @ %s:%i%s\n", name, host, port, more_coming ? " (more coming)" : "");
  }

 protected:
  std::string service_type_;
  
 private:
  class Implementation;
  Implementation *impl_;
};

} // namespace oscit

#endif // OSCIT_INCLUDE_OSCIT_ZEROCONF_H_
