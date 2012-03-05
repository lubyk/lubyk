/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher (http://teti.ch).

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

#ifndef LUBYK_INCLUDE_MDNS_ABSTRACT_REGISTRATION_H_
#define LUBYK_INCLUDE_MDNS_ABSTRACT_REGISTRATION_H_
#include <string>

#include "mdns/Location.h"

// TODO: Remove this class and keep everything in mdns::Registration.
namespace mdns {

/** This class let's you easily register an application as providing a certain type of
 *  service.
 */
class AbstractRegistration {
 public:
  AbstractRegistration(const char *service_type, const char *name, uint port, const char *txt);

  virtual ~AbstractRegistration();

  const char *name() {
    return name_.c_str();
  }

 protected:
  bool getService();

  std::string name_;
  std::string host_;
  std::string service_type_;
  uint        port_;
  std::string txt_;
  int fd_;

 private:
  class Implementation;
  Implementation *impl_;
};

} // mdns

#endif // LUBYK_INCLUDE_MDNS_ABSTRACT_REGISTRATION_H_
