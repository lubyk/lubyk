#include <sstream>
#include "oscit/location.h"

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


void Location::set_name_from_ip() {
  if (ip_ == NO_IP) {
    name_ = "";
	} else if (ip_ == ANY_IP) {
    name_ = "localhost";
  } else {
	  std::ostringstream out;
    out <<        ((ip_ >> 24) & 0xFF);
    out << "." << ((ip_ >> 16) & 0xFF);
    out << "." << ((ip_ >> 8)  & 0xFF);
    out << "." << ( ip_        & 0xFF);
    name_ = out.str();
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