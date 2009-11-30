#include <sstream>
#include "oscit/location.h"
#include "oscit/zeroconf_browser.h"

#include <netdb.h>     // gethostbyname
#include <arpa/inet.h> // inet_addr

namespace oscit {

std::ostream &operator<<(std::ostream &out_stream, const Location &location) {
  if (location.name_ == "") return out_stream;
  if (location.protocol_ != "") {
    out_stream << location.protocol_ << "://";
  }

  if (location.reference_by_hostname_) {
    out_stream << location.name_;
    if (location.port_ != Location::NO_PORT) {
      out_stream << ":" << location.port_;
    }
  } else {
    // TODO: escape double quotes in name_
    out_stream << "\"" << location.name_ << "\"";
  }
  return out_stream;
}

const std::string Location::inspect() const {
  std::ostringstream out;
  out << protocol_ << "://" << name_from_ip(ip_) << ":" << port_;
  return out.str();
}

const std::string Location::name_from_ip(unsigned long ip) {
  if (ip == NO_IP) {
    return std::string("");
	} else if (ip == ANY_IP) {
    return std::string("localhost");
  } else {
	  std::ostringstream out;
    out <<        ((ip >> 24) & 0xFF);
    out << "." << ((ip >> 16) & 0xFF);
    out << "." << ((ip >> 8)  & 0xFF);
    out << "." << ( ip        & 0xFF);
    return out.str();
  }
}

void Location::resolve_with(const ZeroConfBrowser *browser) {
  if (ip_ == NO_IP) {
    if (reference_by_hostname_) {
      ip_ = ip_from_hostname(name_.c_str());
    } else {
      if (!browser->get_location_from_name(name_.c_str(), this)) {
        std::cerr << "Could not resolve service '" << name_ << "'.\n";
      }
    }
  }
}

std::ostream &operator<< (std::ostream &out_stream, const std::list<Location> &locations) {
  std::list<Location>::const_iterator it;
  std::list<Location>::const_iterator begin = locations.begin();
  std::list<Location>::const_iterator end   = locations.end();
  for(it = begin; it != end; ++it) {
    if (it != begin) out_stream << ", ";
    out_stream << *it;
  }
  return out_stream;
}

} // oscit