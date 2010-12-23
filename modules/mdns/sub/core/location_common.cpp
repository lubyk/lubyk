/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#include "mdns/location.h"

#include <netdb.h>     // gethostbyname
#include <arpa/inet.h> // inet_addr

#include <sstream>

#include "mdns/abstract_browser.h"



namespace mdns {

std::ostream &operator<<(std::ostream &out_stream, const Location &location) {
  if (location.name_ == "") return out_stream;
  if (location.protocol_ != "") {
    out_stream << location.protocol_ << "://";
  }

  if (location.name_ != "") {
    // TODO: escape double quotes in name_
    out_stream << "\"" << location.name_ << "\"";
  } else {
    out_stream << location.host_;
    if (location.port_ != Location::NO_PORT) {
      out_stream << ":" << location.port_;
    }
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

} // mdns