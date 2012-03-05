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
#ifndef LUBYK_INCLUDE_MDNS_BROWSER_H_
#define LUBYK_INCLUDE_MDNS_BROWSER_H_

#include "dub/dub.h"
#include "mdns/Service.h"

#include <stdlib.h> // atoi

namespace mdns {

class Context;

/** Browse for devices matching a give service type. Call a lua function
 * when devices are added or removed.
 *
 * @dub push: pushobject
 *      string_format: %%s
 *      string_args: self->serviceType()
 */
class Browser : public dub::Thread {
  /** Protocol used in communication (usually '_lubyk._tcp').
  */
  std::string   protocol_;

  /** Service-type to browse.
  */
  std::string   service_type_;
  
  /** Filedescriptor to listen for new/removed devices.
   */
  int fd_;

  class Implementation;
  Implementation *impl_;
public:
  Browser(mdns::Context *ctx, const char *service_type);

  ~Browser();

  const char *serviceType() {
    return service_type_.c_str();
  }

  /** Return the mDNS browser's filedescriptor.
   */
  int fd() {
    return fd_;
  }

  /** We have some data: get the service. Must be called after
   * waitRead on fd().
   *
   */
  LuaStackSize getService(lua_State *L);

private:
  void setProtocolFromServiceType();
};
} // mdns

#endif // LUBYK_INCLUDE_MDNS_BROWSER_H_
