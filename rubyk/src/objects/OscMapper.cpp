#include <list>
#include <stdexcept> // runtime_error

#include "oscit.h"
#include "rubyk.h"
#include "mapper.h"

#include "ip/UdpSocket.h"

class RefCountedMapper : public Mapper, public ReferenceCounted {
public:
  RefCountedMapper(size_t hash_size) : Mapper(hash_size) {}
};

class OscMapper : public Script {
public:
  OscMapper() : protocol_("oscmap"), service_type_(""),
                port_(7001), reply_port_(7002), map_command_(NULL) {
    mapper_ = new RefCountedMapper(200);
  }
  
  const Value start() {
    create_map_command();
    return gNilValue;
  }
  
  virtual ~OscMapper() {
    // we do not delete map_command_: it was adopted by root_.
    if (map_command_) map_command_->osc_mapper_ = NULL;
    
    mapper_ = ReferenceCounted::release(mapper_);
  }
  
  // {4} set/change input port
  const Value port(const Value &val) {
    if (val.is_real() && val.r > 0) {
      port_ = val.r;
      recreate_map_command();
    }
    
    return Value(port_);
  }
  
  // {5} set/change input port
  const Value reply_port(const Value &val) {
    if (val.is_real() && val.r > 0) {
      reply_port_ = val.r;
      recreate_map_command();
    }
    
    return Value(reply_port_);
  }

 protected:

  /** Mappings compilation.
   */
  virtual const Value eval_script() {
    Value res;
    if (map_command_) map_command_->lock();
      if (mapper_->parse(script_.c_str())) {
        res = Value(script_);
      } else {
        res = Value(BAD_REQUEST_ERROR, mapper_->error());
      }
    if (map_command_) map_command_->unlock();
    return res;
  }
    
 private:
  /** Private command.
   * This is where unmapped osc commands are received.
   */
  class MapCommand : public oscit::OscCommand {
  public:
    MapCommand(OscMapper *osc_mapper, const char *protocol, const char *service_type, uint16_t port, 
                  uint16_t reply_port, RefCountedMapper *mapper) : oscit::OscCommand(protocol, service_type, port),
                  osc_mapper_(osc_mapper), reply_port_(reply_port),
                  observers_(30), mapper_(NULL) {
      mapper_ = ReferenceCounted::acquire(mapper);
    }
  
    virtual ~MapCommand() {
      osc_mapper_->map_command_ = NULL;
      mapper_ = ReferenceCounted::release(mapper_);
    }
  
    /** Executed within mutex lock from own thread. */
    virtual void process_message(const IpEndpointName &remote_endpoint, const std::string &ext_url, const Value &ext_val);
  
    /** Executed within mutex lock from root. */
    virtual void notify_observers(const char *url, const Value &val);
  
    OscMapper *osc_mapper_;
    /** Port where the applications connecting to here expect replies.
     */
    uint16_t reply_port_;
    THash<unsigned long, IpEndpointName> observers_;
    RefCountedMapper *mapper_;
  };
  
  void recreate_map_command() {
    if (map_command_ != NULL) {
      delete map_command_;
      create_map_command();
    }
  }
  
  void create_map_command() {
    map_command_ = root_->adopt_command(new MapCommand(this, protocol_.c_str(), service_type_.c_str(), port_, reply_port_, mapper_));
  }
  
  std::string protocol_;
  std::string service_type_;
  uint16_t port_;
  uint16_t reply_port_;
  MapCommand *map_command_;
  RefCountedMapper *mapper_;
};


void OscMapper::MapCommand::process_message(const IpEndpointName &remote_endpoint, const std::string &ext_url, const Value &ext_val) {
  if (osc_mapper_) {
    unlock();
      // we lock during eval_script
      osc_mapper_->reload_script();
    lock();
  }
  const IpEndpointName *remote_reply;
  
  // 0. register observer
  if (!observers_.get(remote_endpoint.address, &remote_reply)) {
    observers_.set(remote_endpoint.address, IpEndpointName(remote_endpoint.address, reply_port_));
  }
  
  // 1. resolve mapping
  std::string url;
  Real  r;
  Value res;
  
  // 2. root_->call... Value res = root_->call(url, val, NULL);  // NULL = external context
  std::cout << "resolving from: " << ext_url << "(" << ext_val << ")\n";
  
  
  if (ext_val.is_real()) {
    if (mapper_->map(ext_url, ext_val.r, &url, &r)) {
      res = root_->call(url, Value(r), this);
      std::cout << "to            : " << url << "(" << r << ")\n";
    } else {
      // no mapping found...
      // fprintf(stderr, "No mapping found for '%s'\n", ext_url.c_str());
    }
  } else {
    //fprintf(stderr, "Input value not supported (%s)\n", ext_val.to_json().c_str());
  }
  
  
  // 3. send reply to reply_port with reverse mapping
  if (!res.is_error()) {
    notify_observers(url.c_str(), res);
  }
}
  
void OscMapper::MapCommand::notify_observers(const char *url, const Value &val) {
  std::string ext_url;
  Real ext_val;
  std::list<unsigned long> to_remove;
  if (val.is_real()) {
    if (mapper_->reverse_map(url, val.r, &ext_url, &ext_val)) {
      std::list<unsigned long>::const_iterator it  = observers_.begin();
      std::list<unsigned long>::const_iterator end = observers_.end();
      const IpEndpointName *remote;
      while (it != end) {
        if (observers_.get(*it, &remote)) {
          try {
            send(*remote, url, Value(ext_val));
          } catch (std::runtime_error e) {
            char address[ IpEndpointName::ADDRESS_AND_PORT_STRING_LENGTH ];
            remote->AddressAndPortAsString(address);
            fprintf(stderr, "Could not connect to observer '%s'.\n", address);
            to_remove.push_back(*it);
          }
        }
        ++it;
      }
      it = to_remove.begin();
      end = to_remove.end();
      while (it != end) {
        observers_.remove(*it);
      }
    } else {
      // could not reverse map
      // fprintf(stderr, "Could not reverse map '%s'\n", url);
    }
  } else {
    // fprintf(stderr, "Output value not supported (%s)\n", val.to_json().c_str());
  }
}


extern "C" void init(Planet &planet) {
  CLASS (OscMapper, "Open udp ports and map calls from these ports.", "script: [mapping definitions] or file: [path to mapping file]")
  // {1}
  c->add_method<Script, &Script::file>("file", StringIO("path", "Set path to mappings definitions."));
  // {2}
  c->add_method<Script, &Script::script>("script", StringIO("mappings", "Mappings definitions."));
  // {3}
  c->add_method<Script, &Script::reload>("reload", RealIO("seconds", "How often shoudl we check file for relaod."));
  METHOD(OscMapper, port, RealIO("number", "Set incomming port number."))
  METHOD(OscMapper, reply_port, RealIO("number", "Set outgoing reply port number."))
}
