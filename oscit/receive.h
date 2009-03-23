#ifndef _OSCIT_RECEIVE_H_
#define _OSCIT_RECEIVE_H_

#include "new_value.h"
typedef unsigned int uint;

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
  OscReceive(Root * root, uint pPort);
  
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
  
  /** Type tags as they are used in the application with automatic type casting from int/float to real, etc.
    * @param[out] retTypes transposed types.
    * @param  pTypes list of typetags characters.
    * @return hash value of the casted type tags. */
  static uint casted_type_tags(char * retTypes, const char * pTypes);
  
  /** Write osc values inside the Value. 
    * @param pRes storage the value / multi value (allready allocated, type checked). 
    * @param pMsg osc message. */
  static void values_from_osc(Value * pRes, const osc::ReceivedMessage& pMsg);
  Root * root_;
};
} // namespace oscit
#endif // _OSCIT_RECEIVE_H_