#ifndef _OSCIT_ZEROCONF_H_
#define _OSCIT_ZEROCONF_H_

#include "oscit/thread.h"

#include <string>

struct _DNSServiceRef_t;
typedef struct _DNSServiceRef_t* DNSServiceRef;

typedef int32_t DNSServiceErrorType;

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
  
  /** Setup serviceRef and start listening. */
  virtual void start() = 0;
  
  /** Should not be called directly (used by Thread). */
  void terminate(Thread *thread) {
    quit_ = true;
  }
  
protected:
  /** Process events here. */
  void listen(Thread *thread, DNSServiceRef service_ref);
  
  volatile bool quit_;
  volatile int  timeout_;
  Thread listen_thread_;
};

class ZeroConfRegister : public ZeroConf
{
 public:
  ZeroConfRegister(const std::string &name, const std::string &service_type, uint16_t port) : name_(name), service_type_(service_type), port_(port) {}
  
  /** Callback */
  void register_callback(DNSServiceErrorType error, const char *name, const char *service_type, const char *domain);
  
  /** Registration setup. */
  virtual void start();
  
 private:
  void do_start(Thread *thread);
  std::string name_;
  std::string service_type_;
  uint16_t    port_;
};

} // namespace oscit

#endif // _OSCIT_ZEROCONF_H_