/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
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
#ifndef RUBYK_INCLUDE_MDNS_REGISTRATION_H_
#define RUBYK_INCLUDE_MDNS_REGISTRATION_H_

#include "mdns/abstract_registration.h"

#include "rubyk.h"
using namespace rubyk;


namespace mdns {

/** Register a service for a given service type.
 *
 * @dub string_format:'%%s'
 *      string_args:'(*userdata)->name()'
 *      lib_name:'Registration_core'
 *      constructor:'MakeInstance'
 */
class Registration : public AbstractRegistration, public LuaCallback
{
public:
  Registration(rubyk::Worker *worker, const char *service_type, const char *name, uint port) :
    AbstractRegistration(service_type, name, port),
    LuaCallback(worker) {}

  ~Registration() {
    stop();
  }

  static LuaStackSize MakeInstance(rubyk::Worker *worker, const char *service_type, const char *name, uint port, lua_State *L) {
    Registration *instance = new Registration(worker, service_type, name, port);
    luaL_checktype(L, -1, LUA_TFUNCTION);
    lua_pushclass<Registration>(L, instance, "mdns.Registration");
    lua_pushvalue(L, -2);
    // ... <self> <func>
    instance->set_lua_callback(L);
    lua_pop(L, 1);
    // ... <self>
    return 1;
  }

  virtual void registration_done() {
    lua_State *L = lua_;
    ScopedLock lock(worker_);

    // do not push self
    push_lua_callback(false);

    // create table {name = 'x', host = '10.0.0.34', port = 7500}
    lua_newtable(L);

    // name = 'xxxxx'
    lua_pushstring(L, "name");
    lua_pushstring(L, name_.c_str());
    lua_settable(L, -3);

    // host = 'xxxx'
    lua_pushstring(L, "host");
    lua_pushstring(L, host_.c_str());
    lua_settable(L, -3);

    // port = 7500
    lua_pushstring(L, "port");
    lua_pushnumber(L, port_);
    lua_settable(L, -3);

    int status = lua_pcall(L, 1, 0, 0);

    if (status) {
      printf("Error in registration_done: %s\n", lua_tostring(L, -1));
    }
  }
};
} // mdns

#endif // RUBYK_INCLUDE_MDNS_REGISTRATION_H_