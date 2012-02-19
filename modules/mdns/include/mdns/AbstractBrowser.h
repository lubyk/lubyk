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

class Service;

/** This class let's you easily find applications providing a certain
 * service.
 */
class AbstractBrowser {
  class Implementation;
  Implementation *impl_;
protected:

  /** Protocol used in communication (usually 'lubyk').
  */
  std::string   protocol_;

  /** Service-type to browse.
  */
  std::string   service_type_;
  
  /** Filedescriptor to listen for new/removed devices.
   */
  int fd_;

  /** Last detected location.
   */
  Service *found_service_;

  /** This is true for 'add' and false for 'remove'.
   */
  bool is_add_;

  /** Once we have data in fd_, we must wait on the resolution file descriptor
   * to get device info.
   */
  int resolveFd();

public:
  AbstractBrowser(const char *service_type);

  virtual ~AbstractBrowser();

protected:
  void setProtocolFromServiceType();

  /** Once we have some data ready, we call this method to get the
   * new mdns::Service (caller is responsible for deallocation).
   */
  Service *getService();
};

} // mdns

#endif // MDNS_INCLUDE_MDNS_ABSTRACT_BROWSER_H_
