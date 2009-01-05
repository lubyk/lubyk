#ifndef _OSCIT_RECEIVE_H_
#define _OSCIT_RECEIVE_H_

#include "value.h"
typedef unsigned int uint;

namespace osc {
  class ReceivedMessage;
}

class UdpListeningReceiveSocket;
class IpEndpointName;
#include "osc/OscPacketListener.h"

namespace oscit {

class Root;
class ZeroConfRegister;

class OscReceive : public osc::OscPacketListener
{
public:
  OscReceive(Root * pRoot, uint pPort);
  
  virtual ~OscReceive();
  
private:  
  /** Process incoming messages. */
  virtual void ProcessMessage( const osc::ReceivedMessage& pMsg, const IpEndpointName& remoteEndpoint );
  
  /** Start "run" loop in new thread. */
  static void * start_thread(void *);
  
  /** Start listening for incoming messages. */
  void run ();
  
  pthread_t mListenThreadId;
  
  UdpListeningReceiveSocket * mSocket;
  
  ZeroConfRegister * mRegisterZeroConf; /** Zeroconf registration thread. */
  
  Value value_from_osc(const osc::ReceivedMessage& pMsg);
  
  Root * mRoot;
};
} // namespace oscit
#endif // _OSCIT_RECEIVE_H_