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
#ifndef LUBYK_INCLUDE_MDNS_REGISTRATION_H_
#define LUBYK_INCLUDE_MDNS_REGISTRATION_H_

#include "dub/dub.h"

namespace mdns {

class Context;

/** Register a service for a given service type.
 *
 * @dub string_format: %%s
 *      string_args: self->name()
 *      push: pushobject
 *      
 */
class Registration : public dub::Thread {
  std::string name_;
  std::string host_;
  std::string service_type_;
  int         port_;
  std::string txt_;
  int fd_;

  class Implementation;
  Implementation *impl_;
public:

  Registration(Context *ctx, const char *service_type, const char *name, int port, const char *txt);

  ~Registration();

  int fd() {
    return fd_;
  }

  /** @internal.
   */
  void start();

  /** @internal.
   */
  void stop();

  const char *name() {
    return name_.c_str();
  }

  /** Get a table describing the service.
   */
  LuaStackSize getService(lua_State *L) {
    if (!getService()) {
      // TODO: Something went wrong, we should return nil, error ?
      return 0;
    }
    // create table {name = 'x', host = '10.0.0.34', port = 7500}
    lua_newtable(L);
    // name = 'xxxx'
    lua_pushstring(L, "name");
    lua_pushstring(L, name_.c_str());
    lua_settable(L, -3);
    // host = 'gaspard.local' / '10.3.4.5'
    lua_pushstring(L, "host");
    lua_pushstring(L, host_.c_str());
    lua_settable(L, -3);
    // port = 7890
    lua_pushstring(L, "port");
    lua_pushnumber(L, port_);
    lua_settable(L, -3);
    // <table>
    return 1;
  }

private:
  bool getService();
};

} // mdns

#endif // LUBYK_INCLUDE_MDNS_REGISTRATION_H_
