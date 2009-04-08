#ifndef _OSCIT_RECEIVE_H_
#define _OSCIT_RECEIVE_H_
#include "oscit/values.h"

namespace osc {
  class ReceivedMessage;
}

class UdpListeningReceiveSocket;
class IpEndpointName;
#include "osc/OscPacketListener.h"

namespace oscit {

#define MAX_TYPE_TAGS_LENGTH 10

class Root;
class ZeroConfRegister;

class OscReceive : public osc::OscPacketListener
{
public:
  OscReceive(Root * root, uint port);
  
  virtual ~OscReceive();
  
  UdpListeningReceiveSocket *socket() { return socket_; }
  
private:  
  /** Process incoming messages. */
  virtual void ProcessMessage(const osc::ReceivedMessage &message, const IpEndpointName &remote_endpoint);
  
  /** Start "run" loop in new thread. */
  static void * start_thread(void *);
  
  /** Start listening for incoming messages. */
  void run ();
  
  pthread_t listen_thread_id_;
  
  UdpListeningReceiveSocket *socket_;
  
  ZeroConfRegister *zeroconf_register_; /** Zeroconf registration thread. */
  
  /** Write osc values inside the Value. 
    * @param pRes storage the value / multi value (allready allocated, type checked). 
    * @param message osc message. */
  static Value value_from_osc(const osc::ReceivedMessage &message);
  Root * root_;
};
} // namespace oscit
#endif // _OSCIT_RECEIVE_H_