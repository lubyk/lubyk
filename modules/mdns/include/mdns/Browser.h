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

#include "lubyk.h"
#include "mdns/AbstractBrowser.h"

using namespace lubyk;

namespace mdns {

/** Browse for devices matching a give service type. Call a lua function
 * when devices are added or removed.
 *
 * @dub string_format:'%%s'
 *      string_args:'(*userdata)->serviceType()'
 *      lib_name:'Browser_core'
 */
class Browser : public AbstractBrowser, public LuaObject
{
public:
  Browser(const char *service_type)
      : AbstractBrowser(service_type) {}

  ~Browser() {}

  const char *serviceType() {
    return service_type_.c_str();
  }

  /** Return the mDNS browser's filedescriptor.
   */
  int fd() {
    return fd_;
  }

  /** Load found services (must be called after waitRead or it blocks).
   */
  bool getServices() {
    return AbstractBrowser::getServices();
  }

  /** Get next found service. Must call getServices after scheduler waitRead
   * and before nextService.
   * @return nil if there is nothing left.
   */
  LuaStackSize nextService(lua_State *L) {
    if (found_services_.empty()) {
      return 0;
    }
    Location loc = found_services_.front();
    found_services_.pop();

    // create table {op = 'add/remove', name = 'x', host = '10.0.0.34', port = 7500, interface = 2}
    lua_newtable(L);
    // op = 'add/remove'
    lua_pushstring(L, "op");
    if (loc.port()) {
      lua_pushstring(L, "add");
    } else {
      lua_pushstring(L, "remove");
    }
    lua_settable(L, -3);
    // name = 'xxxx'
    lua_pushstring(L, "name");
    lua_pushstring(L, loc.name());
    lua_settable(L, -3);
    // host = 'gaspard.local' / '10.3.4.5'
    lua_pushstring(L, "host");
    lua_pushstring(L, loc.host());
    lua_settable(L, -3);
    // ip = '10.3.4.5' / 'localhost'
    lua_pushstring(L, "ip");
    lua_pushstring(L, loc.name_from_ip(loc.ip()).c_str());
    lua_settable(L, -3);
    // port = 7500
    lua_pushstring(L, "port");
    lua_pushnumber(L, loc.port());
    lua_settable(L, -3);
    // interface = 2
    lua_pushstring(L, "interface");
    lua_pushnumber(L, loc.interface());
    lua_settable(L, -3);
    return 1;
  }
};
} // mdns

#endif // LUBYK_INCLUDE_MDNS_BROWSER_H_
