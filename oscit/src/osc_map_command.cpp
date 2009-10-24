
#include "oscit/thread.h"
#include "oscit/root.h"
#include "oscit/osc_map_command.h"

namespace oscit {

// #define DEBUG_MAP_COMMAND

OscMapCommand::OscMapCommand() :
                  OscCommand("oscmap", "", Location::NO_PORT),
                  reply_port_(Location::NO_PORT),
                  observers_(50),
                  mapper_(200) {
  time_ref_ = Thread::new_time_ref();
}

OscMapCommand::OscMapCommand(uint16_t port, uint16_t reply_port) :
                  OscCommand("oscmap", "", port),
                  reply_port_(reply_port),
                  observers_(50),
                  mapper_(200) {
  time_ref_ = Thread::new_time_ref();
}

OscMapCommand::~OscMapCommand() {
  time_ref_ = Thread::delete_time_ref(time_ref_);
}

void OscMapCommand::process_message(const Location &remote_endpoint, const std::string &ext_url, const Value &ext_val) {
  unlock();
    // release lock because we lock again during eval_script
    reload_script(Thread::real_time(time_ref_));
  lock();

  const Location *remote_reply;

  // register observer
  if (!observers_.get(remote_endpoint.ip(), &remote_reply)) {
    observers_.set(remote_endpoint.ip(), Location(remote_endpoint.ip(), reply_port_));
  }

  std::string url;
  Real  r;
  Value res;

#ifdef DEBUG_MAP_COMMAND
  std::cout << "resolving from: " << ext_url << "(" << ext_val << ")\n";
#endif
  // resolve mapping
  if (ext_val.is_real()) {
    if (mapper_.map(ext_url, ext_val.r, &url, &r)) {
      res = root_->call(url, Value(r), this);
#ifdef DEBUG_MAP_COMMAND
      std::cout << "to            : " << url << "(" << r << ")\n";
#endif
    } else {
      // no mapping found...
      // fprintf(stderr, "No mapping found for '%s'\n", ext_url.c_str());
    }
  } else {
    //fprintf(stderr, "Input value not supported (%s)\n", ext_val.to_json().c_str());
  }


  // send reply to reply_port with reverse mapping
  if (!res.is_error()) {
    send_to_observers(url.c_str(), res, &remote_endpoint);
  }
}

void OscMapCommand::send_to_observers(const char *url, const Value &val, const Location *skip_end_point) {
  std::string ext_url;
  Real ext_val;
  std::list<unsigned long> to_remove;
  if (val.is_real()) {
    if (mapper_.reverse_map(url, val.r, &ext_url, &ext_val)) {
      std::list<unsigned long>::const_iterator it  = observers_.begin();
      std::list<unsigned long>::const_iterator end = observers_.end();
      const Location *remote;
      while (it != end) {
        if ((skip_end_point == NULL || *it != skip_end_point->ip()) && observers_.get(*it, &remote)) {
          try {
            send(*remote, ext_url, Value(ext_val));
          } catch (std::runtime_error e) {
            //char address[ IpEndpointName::ADDRESS_AND_PORT_STRING_LENGTH ];
            //remote->AddressAndPortAsString(address);
            std::cerr << "Could not connect to observer '" << remote << "'.\n";
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

}  // oscit