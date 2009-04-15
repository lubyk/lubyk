#ifndef _OSCIT_RECEIVE_H_
#define _OSCIT_RECEIVE_H_
#include "oscit/command.h"

#include <list>

namespace osc {
  class ReceivedMessage;
  class OutboundPacketStream;
}

class UdpSocket;
class UdpListeningReceiveSocket;
class IpEndpointName;
#include "osc/OscPacketListener.h"

namespace oscit {

#define MAX_TYPE_TAGS_LENGTH 10
#define OSC_OUT_BUFFER_SIZE 2048

class Root;
class ZeroConfRegister;

class OscCommand : public Command, public osc::OscPacketListener
{
public:
  OscCommand(uint port);
  
  virtual ~OscCommand();
  
  /** Add a new satellite to the list of observers. This method is the implementation
   *  of "/.register".
   * TODO: how to get the IP without checking for "/.register" in the 'receive' method ?
   * FIXME: implement TTL (time to live)
   * FIXME: avoid duplicates (??)
   */
  void register_observer(const IpEndpointName &observer);
  
  /** Send a notification to all observers. */
  virtual void notify_observers(const char *url, const Value &val) {
    printf("notify...\n");
    send_all(url, val);
  }
  
  /** Send an osc message. 
   *  @param remote_endpoint target host.
   */
  void send(const IpEndpointName &remote_endpoint, const std::string &url, const Value &val) {
    send(remote_endpoint, url.c_str(), val);
  }
  
  /** Send an osc message. 
   *  @param remote_endpoint target host.
   */
  void send(const IpEndpointName &remote_endpoint, const char *url, const Value &val);
  
  virtual void terminate(Thread *runner);
  
private:
  /** Start listening for incoming messages (runs in its own thread). */
  virtual void do_listen();
  
  /** Callback to process incoming messages. */
  virtual void ProcessMessage(const osc::ReceivedMessage &message, const IpEndpointName &remote_endpoint);
  
  /** Send reply to caller and notify observers. */
  void send_reply(const IpEndpointName *remote_endpoint, const std::string &url, const Value &val);
  
  /** Build osc message and send it to all observers. */
  void send_all(const char *url, const Value &val, const IpEndpointName *skip_end_point = NULL);
  
  /** Build a value from osc packet.
   *  @param message osc message.
   *  @return new value corresponding to the osc data.
   */
  static Value value_from_osc(const osc::ReceivedMessage &message);
  
  /** Build a message from a value. */
  static void build_message(const char *url, const Value &val, osc::OutboundPacketStream *message);
  
  UdpListeningReceiveSocket *socket_;   /**< Socket listening to udp packets. */
  
  ZeroConfRegister *zeroconf_register_; /** Zeroconf registration thread. */
  
  std::list<IpEndpointName> observers_; /**< List of satellites that have registered to get return values back. */
  
  char osc_buffer_[OSC_OUT_BUFFER_SIZE];     /** Buffer used to build osc packets. */
};

} // oscit

#endif // _OSCIT_RECEIVE_H_