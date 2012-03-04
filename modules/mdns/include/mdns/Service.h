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

#ifndef LUBYK_INCLUDE_MDNS_SERVICE_H_
#define LUBYK_INCLUDE_MDNS_SERVICE_H_
#include <string>

#include "lubyk.h"
using namespace lubyk;

#include "mdns/Location.h"

namespace mdns {

/** This class holds a service as found by an mnds.Browser. 
 *
 * @dub string_format:'%%s'
 *      string_args:'(*userdata)->name()'
 *      lib_name:'core/Service_core'
 *      ignore:'set'
 */
class Service : public Location, public ThreadedLuaObject {
  std::string service_type_;
  std::string name_;
  std::string txt_;
  bool is_resolved_;
  bool is_add_;

  /** File descriptor used to register the resolution socket.
   */
  int fd_;

  /** @internal. Used by Linux.
   */
  int event_;

  class Implementation;
  Implementation *impl_;
 public:

  /** @internal.
   */
  Service(std::string service_type, const char *name, int interface_index, const char *type, const char *domain, bool is_add);

  virtual ~Service();

  LuaStackSize fd(lua_State *L) {
    if (fd_ > 0) {
      lua_pushnumber(L, fd_);
      return 1;
    } else {
      return 0;
    }
  }

  bool isAdd() {
    return is_add_;
  }

  const char *name() {
    return name_.data();
  }

  LuaStackSize serviceType(lua_State *L) {
    lua_pushlstring(L, service_type_.data(), service_type_.length());
    return 1;
  }

  /** Must call waitRead on fd() before calling this.
   */
  LuaStackSize info(lua_State *L) {
    // getInfo will hang if waitRead on fd() hasn't been called.
    if (!getInfo()) {
      return 0;
    }

    // create table {name = 'x', host = '10.0.0.34', port = 7500, interface = 2}
    lua_newtable(L);

    // op = 'add'/'remove'
    if (is_add_) {
      lua_pushstring(L, "add");
    } else {
      lua_pushstring(L, "remove");
    }
    lua_setfield(L, -2, "op");
    // name = 'xxxx'
    lua_pushstring(L, name());
    lua_setfield(L, -2, "name");
    // host = 'gaspard.local' / '10.3.4.5'
    lua_pushstring(L, host());
    lua_setfield(L, -2, "host");
    // ip = '10.3.4.5' / 'localhost'
    lua_pushstring(L, name_from_ip(ip_).c_str());
    lua_setfield(L, -2, "ip");
    // port = 7500
    lua_pushnumber(L, port());
    lua_setfield(L, -2, "port");
    // interface = 2
    lua_pushnumber(L, interface());
    lua_setfield(L, -2, "interface");

    // txt = {...}
    pushTxtRecord(L, txt_);
    lua_setfield(L, -2, "txt");
    return 1;
  }

  /** @internal.
   * Callback from Implementation
   */
  void set(const char *hostname, int port, std::string txt) {
    host_ = hostname;
    ip_   = ip_from_hostname(hostname);
    port_ = port;
    txt_  = txt;
  }

 protected:
  // Calls Implementation::getInfo --> calls back 'set'.
  bool getInfo();

  void pushTxtRecord(lua_State *L, const std::string &txt) {
    // [LEN] KEY ( EOF | '=' ) ( VALUE | EOF ) [LEN] ...
    lua_newtable(L);
    // ... <table>
    if (txt == "") return;
    size_t txt_len = txt.size();
    size_t pos = 0;
    while ( pos < txt_len ) {
      size_t l = txt.at(pos);
      ++pos;
      if (!l) break;
      std::string line = txt.substr(pos, l);
      size_t sep = line.find('=');
      if (sep == std::string::npos) {
        // boolean true
        lua_pushlstring(L, line.c_str(), line.size());
        lua_pushboolean(L, true);
      } else {
        std::string key, value;
        key   = line.substr(0, sep);
        value = line.substr(sep + 1);
        lua_pushlstring(L, key.c_str(), key.size());
        if (value.find_first_not_of("0123456789") == std::string::npos) {
          lua_pushnumber(L, atoi(value.c_str()));
        } else {
          lua_pushlstring(L, value.c_str(), value.size());
        }
      }
      lua_settable(L, -3);
      pos = pos + l;
    }
  }
};

} // mdns

#endif // LUBYK_INCLUDE_MDNS_SERVICE_H_

