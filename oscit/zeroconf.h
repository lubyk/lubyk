#ifndef _OSCIT_ZEROCONF_H_
#define _OSCIT_ZEROCONF_H_
#include <string>

struct _DNSServiceRef_t;
typedef struct _DNSServiceRef_t* DNSServiceRef;

typedef unsigned int uint;
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
  void listen (DNSServiceRef pServiceRef);
  
  /** Start "run" loop in new thread. */
  static void * start_thread(void *);
  
  volatile bool   mQuit;
  volatile int    mTimeOut;
  
  pthread_t mListenThreadId;
};

class ZeroConfRegister : public ZeroConf
{
public:
  ZeroConfRegister(const std::string& pName, const std::string& pServiceType, uint pPort) : mName(pName), mServiceType(pServiceType), mPort(pPort) {}
  
  /** Callback */
  void register_callback (DNSServiceErrorType pError, const char * pName, const char * pServiceType, const char * pDomain);
  
private:
  /** Registration setup. */
  virtual void start ();
  
  std::string mName;
  std::string mServiceType;
  uint16_t    mPort;
};

} // namespace oscit

#endif // _OSCIT_ZEROCONF_H_