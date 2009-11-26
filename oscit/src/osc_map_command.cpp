
#include "oscit/thread.h"
#include "oscit/root.h"
#include "oscit/osc_map_command.h"

namespace oscit {

//#define DEBUG_MAP_COMMAND

OscMapCommand::OscMapCommand() :
                  OscCommand("oscmap", "", Location::NO_PORT),
                  reply_port_(Location::NO_PORT),
                  mapper_(200) {
  time_ref_ = Thread::new_time_ref();
}

OscMapCommand::OscMapCommand(uint16_t port, uint16_t reply_port) :
                  OscCommand("oscmap", "", port),
                  reply_port_(reply_port),
                  mapper_(200) {
  time_ref_ = Thread::new_time_ref();
}

OscMapCommand::~OscMapCommand() {
  time_ref_ = Thread::delete_time_ref(time_ref_);
}

void OscMapCommand::receive(const Url &ext_url, const Value &ext_val) {
  if (handle_reply_message(ext_url, ext_val) || handle_register_message(ext_url, ext_val)) {
    return;
  }

  unlock();
    // release lock because we lock again during eval_script
    reload_script(Thread::real_time(time_ref_));
  lock();

  std::string path;
  Real  r;
  Value res;

#ifdef DEBUG_MAP_COMMAND
  std::cout << "resolving from: " << ext_url << "(" << ext_val << ")\n";
#endif
  // resolve mapping
  if (ext_val.is_real()) {
    if (mapper_.map(ext_url.path(), ext_val.r, &path, &r)) {
      res = root_->call(Url(ext_url.location(), path), Value(r), this);
#ifdef DEBUG_MAP_COMMAND
      std::cout << "to            : " << path << "(" << r << ")\n";
#endif
    } else {
      // no mapping found...
      // fprintf(stderr, "No mapping found for '%s'\n", ext_path.c_str());
    }
  } else {
    //fprintf(stderr, "Input value not supported (%s)\n", ext_val.to_json().c_str());
  }
}

void OscMapCommand::notify_observers(const char *url, const Value &val) {
  std::string ext_url;
  Real ext_val;
  std::list<unsigned long> to_remove;
  if (val.size() == 2  && val[0].is_string() && val[1].is_real()) {
    if (mapper_.reverse_map(val[0].c_str(), val[1].r, &ext_url, &ext_val)) {
      std::list<Location>::const_iterator it  = observers().begin();
      std::list<Location>::const_iterator end = observers().end();

      while (it != end) {
        try {
          send(*it, ext_url.c_str(), Value(ext_val));
        } catch (std::runtime_error e) {
          std::cerr << "Could not connect to observer '" << *it << "'.\n";
        }
        ++it;
      }
    } else {
      // could not reverse map
      std::cerr << "Could not reverse map '" << url << "'\n";
    }
  } else {
    std::cerr << "Output value not supported (" << val << ")\n";
  }
}

}  // oscit