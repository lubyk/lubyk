#ifndef _OSCIT_OSC_COMMAND_H_
#define _OSCIT_OSC_COMMAND_H_
#include <list>

#include "oscit/command.h"

class IpEndpointName;

namespace oscit {

class Root;

class OscCommand : public Command {
public:
  TYPED("Mutex.Thread.Command.OscCommand")
  
  OscCommand(uint port);
  
  virtual ~OscCommand();
  
  virtual void kill();
  
  /** Send a notification to all observers. */
  virtual void notify_observers(const char *url, const Value &val);
  
  /** Create a reference to a remote object. */
  virtual Object *build_remote_object(const Url &url, Value *error);
  
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
  
private:
  friend class Root;
  class Implementation;
  Implementation *impl_;
  
  /** Start listening for incoming messages (runs in its own thread). */
  virtual void do_listen();
  
  /** Set root for this command and the command's implementation.
   *  (Should only be used by Root class).
   */
  virtual void set_root(Root *root);
};

} // oscit

#endif // _OSCIT_OSC_COMMAND_H_