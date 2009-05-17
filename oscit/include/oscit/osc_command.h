#ifndef OSCIT_INCLUDE_OSCIT_OSC_COMMAND_H_
#define OSCIT_INCLUDE_OSCIT_OSC_COMMAND_H_
#include <list>

#include "oscit/command.h"

class IpEndpointName;

namespace oscit {

class Root;

class OscCommand : public Command {
public:
  TYPED("Mutex.Thread.Command.OscCommand")
  
  OscCommand(uint16_t port);
  
  OscCommand(const char *protocol, const char *service_type, uint16_t port);
  
  virtual ~OscCommand();
  
  virtual void kill();
  
  /** Send a notification to all observers.
   * Executed within mutex lock from root.
   */
  virtual void notify_observers(const char *url, const Value &val);
  
  /** Send an osc message. 
   *  @param remote_endpoint target host.
   *  TODO: Not sure if we really need this method...
   */
  void send(const IpEndpointName &remote_endpoint, const std::string &url, const Value &val);
  
  /** Send an osc message. 
   *  @param remote_endpoint target host.
   *  TODO: Not sure if we really need this method...
   */
  void send(const IpEndpointName &remote_endpoint, const char *url, const Value &val);
  
protected:
  /** Create a reference to a remote object. */
  virtual Object *build_remote_object(const Url &url, Value *error);
  
  /** Start listening for incoming messages (runs in its own thread).
   */
  virtual void listen();
  
  /** Do something useful with the received osc message.
   * Executed within mutex lock from our own thread.
   */
  virtual void process_message(const IpEndpointName &remote_endpoint, const std::string &url, const Value &val);
  
  /** Send a message to all observers.
   */
  virtual void send_to_observers(const char *url, const Value &val, const IpEndpointName *skip_end_point = NULL);
  
  /** Listen to messages on a different port number.
   */
  void change_port(uint16_t port);
  
private:
  class Implementation;
  Implementation *impl_;
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_OSC_COMMAND_H_
