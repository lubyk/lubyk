#ifndef _OSC_RECEIVE_H_
#define _OSC_RECEIVE_H_
#include "value.h"
#include "root.h"
#include "osc/OscPacketListener.h"

namespace osc {
  class OscPacketListener;
  class ReceivedMessage;
}

class UdpListeningReceiveSocket;
class IpEndpointName;
class Planet;

class OscReceive : public osc::OscPacketListener
{
public:
  OscReceive(Planet * pPlanet, uint pPort);
  
  ~OscReceive();
  
  void start_thread();
  
  virtual void ProcessMessage( const osc::ReceivedMessage& pMsg, const IpEndpointName& remoteEndpoint );
  
private:
  pthread_t mListenThreadId;
  
  UdpListeningReceiveSocket * mSocket;
  
  Value value_from_osc(const osc::ReceivedMessage& pMsg);
  
  Planet * mPlanet;
};

#endif // _OSC_RECEIVE_H_