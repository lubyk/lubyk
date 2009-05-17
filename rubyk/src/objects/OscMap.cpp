#include <list>
#include <stdexcept> // runtime_error

#include "oscit.h"
#include "rubyk.h"
#include "oscit/osc_map_command.h"

#include "ip/UdpSocket.h"

class OscMap : public Node, public OscMapCommand {
public:
  OscMap() {}
  
  virtual ~OscMap() {
    kill();
  }
  
  virtual const Value start() {
    // needed so that it's not Thread::start that is called...
    return gNilValue;
  }
};

extern "C" void init(Planet &planet) {
  CLASS (OscMap, "Open udp ports and map calls from these ports.", "script: [mapping definitions] or file: [path to mapping file]")
  // {1}
  c->add_method<Script, &Script::file>("file", StringIO("path", "Set path to mappings definitions."));
  // {2}
  c->add_method<Script, &Script::script>("script", StringIO("mappings", "Mappings definitions."));
  // {3}
  c->add_method<Script, &Script::reload>("reload", RealIO("seconds", "How often shoudl we check file for relaod."));
  
  c->add_method<OscMapCommand, &OscMapCommand::port>("number", RealIO("number", "Set incomming port number."));
  c->add_inlet<OscMapCommand,  &OscMapCommand::port>("number", RealIO("number", "Set incomming port number."));
  c->add_method<OscMapCommand, &OscMapCommand::reply_port>("number", RealIO("number", "Set outgoing reply port number."));
  c->add_inlet<OscMapCommand,  &OscMapCommand::reply_port>("number", RealIO("number", "Set outgoing reply port number."));
}
