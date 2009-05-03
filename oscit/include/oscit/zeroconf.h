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
  
  /** Setup serviceRef and start listening.
   */
  virtual void start() = 0;
  
  /** Process events here.
   */
  void listen(Thread *thread, DNSServiceRef service_ref);
  
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
  ZeroConfRegistration(const std::string &name, const std::string &service_type, uint16_t port) : name_(name), service_type_(service_type), port_(port) {
    start();
  }
  
  virtual void registration_done(const std::string &name) {
    name_ = name;
  }
 protected:
  
  /** Registration setup.
   */
  virtual void start();
 private:
  void do_start(Thread *thread);
  std::string name_;
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
  ZeroConfBrowser(const std::string &service_type) : service_type_(service_type) {
    start();
  }
  
  virtual void add_device(const std::string &name, const std::string &host, unsigned int port, bool more_coming) {
    printf("add_device %s @ %s:%i%s\n", name.c_str(), host.c_str(), port, more_coming ? " (more coming)" : "");
  }
  
  virtual void remove_device(const std::string &name, const std::string &host, unsigned int port, bool more_coming) {
    printf("remove_device %s @ %s:%i%s\n", name.c_str(), host.c_str(), port, more_coming ? " (more coming)" : "");
  }
 protected:
   
  /** Start browsing.
  */
  virtual void start();

 private:
  void do_start(Thread *thread);
  std::string service_type_;
};

} // namespace oscit

#endif // OSCIT_INCLUDE_OSCIT_ZEROCONF_H_