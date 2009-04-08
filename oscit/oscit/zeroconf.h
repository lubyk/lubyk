#ifndef _OSCIT_ZEROCONF_H_
#define _OSCIT_ZEROCONF_H_
#include <string>

struct _DNSServiceRef_t;
typedef struct _DNSServiceRef_t* DNSServiceRef;

#ifndef uint
typedef unsigned int uint;
#endif
typedef int32_t DNSServiceErrorType;

namespace oscit {

/** This class is a helper to initiate zeroconf operations. */
class ZeroConf
{
public:
  ZeroConf();
  
  virtual ~ZeroConf();
  
protected:
  /** Setup serviceRef and start listening. */
  virtual void start() = 0;
  
  /** Process events here. */
  void listen(DNSServiceRef service_ref);
  
  /** Start "run" loop in new thread. */
  static void * start_thread(void *);
  
  volatile bool   quit_;
  volatile int    timeout_;
  
  pthread_t listen_thread_id_;
};

class ZeroConfRegister : public ZeroConf
{
 public:
  ZeroConfRegister(const std::string &name, const std::string &service_type, uint port) : name_(name), service_type_(service_type), port_(port) {}
  
  /** Callback */
  void register_callback(DNSServiceErrorType error, const char * name, const char * service_type, const char * pDomain);
  
 private:
  /** Registration setup. */
  virtual void start();
  
  std::string name_;
  std::string service_type_;
  uint16_t    port_;
};

} // namespace oscit

#endif // _OSCIT_ZEROCONF_H_