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

#ifndef MDNS_INCLUDE_MDNS_ABSTRACT_BROWSER_H_
#define MDNS_INCLUDE_MDNS_ABSTRACT_BROWSER_H_
#include <queue>

#include "mdns/location.h"
#include "lubyk/mutex.h"

namespace mdns {

/** This class let's you easily find applications providing a certain
 * service.
 */
class AbstractBrowser : public lubyk::Mutex {
  class Implementation;
  Implementation *impl_;
protected:
  class Service : public Location {
  public:
    Service(const char *protocol,
            const char *service_name,
            const char *hostname,
            uint port,
            uint interface,
            const std::string &txt,
            bool is_add)
        : Location(protocol, service_name, hostname, port, interface),
          txt_(txt),
          is_add_(is_add) {}
    std::string txt_;
    bool is_add_;
  };

  /** Protocol used in communication (usually 'lubyk').
  */
  std::string   protocol_;

  /** Service-type to browse.
  */
  std::string   service_type_;
  
  /** Filedescriptor to listen for new/removed devices.
   */
  int fd_;

  /** Detected locations.
   */
  std::queue<Service> found_services_;

  /** This is true for 'add' and false for 'remove'.
   */
  bool is_add_;

  /** Once we have some data ready, we call this method to load the
   * service information into location_.
   */
  bool getServices();
public:
  AbstractBrowser(const char *service_type);

  virtual ~AbstractBrowser();

protected:
  void setProtocolFromServiceType();

};

} // mdns

#endif // MDNS_INCLUDE_MDNS_ABSTRACT_BROWSER_H_
