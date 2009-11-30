#ifndef OSCIT_INCLUDE_OSCIT_OSC_COMMAND_H_
#define OSCIT_INCLUDE_OSCIT_OSC_COMMAND_H_
#include <list>

#include "oscit/command.h"

namespace oscit {

class Location;
class Root;

class OscCommand : public Command {
public:
  TYPED("Mutex.Thread.Command.OscCommand")

  OscCommand(uint16_t port);

  OscCommand(const char *protocol, const char *service_type, uint16_t port);

  virtual ~OscCommand();

  virtual void kill();

  virtual void notify_observers(const char *path, const Value &val);

protected:
  /** Create a reference to a remote object. */
  virtual Object *build_remote_object(const Url &url, Value *error);

  /** Send a Value to a remote endpoint at a given path using osc.
   */
  virtual void send_message(const Location &remote_endpoint, const char *path, const Value &val);

  /** Start listening for incoming messages (runs in its own thread).
   */
  virtual void listen();

  /** Listen to messages on a different port number.
   */
  void change_port(uint16_t port);

private:
  class Implementation;
  Implementation *impl_;
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_OSC_COMMAND_H_
