#ifndef OSCIT_INCLUDE_OSCIT_ZEROCONF_H_
#define OSCIT_INCLUDE_OSCIT_ZEROCONF_H_
#include <string>
#include <list>
#include "oscit/thread.h"

struct _DNSServiceRef_t;
typedef struct _DNSServiceRef_t* DNSServiceRef;

namespace oscit {

/** This class is a helper to initiate zeroconf operations.
 *  TODO: could be a sub-class of Thread.
 */
class ZeroConf
{
public:
  ZeroConf();
  
  virtual ~ZeroConf() {}
  
  void quit() {
    listen_thread_.kill();
  }
  
protected:
  
  /** Process events here.
   */
  void listen(Thread *thread, DNSServiceRef service);
  
  volatile int  timeout_;
  Thread listen_thread_;
};


/** This class let's you easily register an application as providing a certain type of
 *  service.
 *  After creating the object, you need to call 'start' for the
 *  registration to take place.
 */
class ZeroConfRegistration : public ZeroConf
{
 public:
  ZeroConfRegistration(const std::string &name, const std::string &service_type, uint16_t port);
  
  virtual void registration_done() {}
  
  void finish_registration(const std::string &name, const std::string &host) {
    name_ = name;
    host_ = host;
    registration_done();
  }
  
 protected:
  void do_start(Thread *thread);

  std::string name_;
  std::string host_;
  std::string service_type_;
  uint16_t    port_;
};

/** This class let's you easily register an application as providing a certain type of
 *  service.
 *  After creating the object, you need to call 'start' for the
 *  registration to take place.
 */
class ZeroConfBrowser : public ZeroConf
{
 public:
  ZeroConfBrowser(const std::string &service_type);
  
  virtual void add_device(const char *name, const char *host, unsigned int port, bool more_coming) {
    printf("add_device %s @ %s:%i%s\n", name, host, port, more_coming ? " (more coming)" : "");
  }
  
  virtual void remove_device(const char *name, const char *host, unsigned int port, bool more_coming) {
    printf("remove_device %s @ %s:%i%s\n", name, host, port, more_coming ? " (more coming)" : "");
  }

 private:
  void do_start(Thread *thread);
  std::string service_type_;
};

} // namespace oscit

#endif // OSCIT_INCLUDE_OSCIT_ZEROCONF_H_