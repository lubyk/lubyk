/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher - Buma (http://teti.ch).

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
#ifndef RUBYK_INCLUDE_MDNS_BROWSER_H_
#define RUBYK_INCLUDE_MDNS_BROWSER_H_

#include "mdns/abstract_browser.h"

#include "rubyk.h"
using namespace rubyk;

namespace mdns {

class Browser : public AbstractBrowser
{
  rubyk::Worker *worker_;
  int func_idx_;
public:
  Browser(rubyk::Worker *worker, const char *service_type, int lua_func_idx) :
    AbstractBrowser(service_type),
    worker_(worker),
    func_idx_(lua_func_idx) {
    start();
  }

  ~Browser() {
    // release function
    stop();
    luaL_unref(worker_->lua_, LUA_REGISTRYINDEX, func_idx_);
  }

  virtual void add_device(const Location &location) {
    lua_State *L = worker_->lua_;
    ScopedLock lock(worker_);
    lua_rawgeti(L, LUA_REGISTRYINDEX, func_idx_);
    lua_pushstring(L, "add");
    lua_pushstring(L, location.name().c_str());
    int status = lua_pcall(L, 2, 0, 0);

    if (status) {
      printf("Error in add_device: %s\n", lua_tostring(L, -1));
    }
    // clear stack
    lua_settop(worker_->lua_, 0);
  }

  virtual void remove_device(const char *name) {
    lua_State *L = worker_->lua_;
    ScopedLock lock(worker_);
    lua_rawgeti(L, LUA_REGISTRYINDEX, func_idx_);
    lua_pushstring(L, "remove");
    lua_pushstring(L, name);
    int status = lua_pcall(L, 2, 0, 0);

    if (status) {
      printf("Error in remove_device: %s\n", lua_tostring(L, -1));
    }
    // clear stack
    lua_settop(worker_->lua_, 0);
  }

  const char *service_type() {
    return service_type_.c_str();
  }
};
} // mdns

#endif // RUBYK_INCLUDE_MDNS_BROWSER_H_