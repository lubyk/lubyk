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
#ifndef RUBYK_INCLUDE_MIMAS_CALLBACK_H_
#define RUBYK_INCLUDE_MIMAS_CALLBACK_H_

#include "rubyk.h"

#include <QtCore/QObject>

#include "mimas/PushButton.h"

namespace mimas {
/** Calls a lua function back.
 *
 * @dub lib_name:'Callback_core'
 */
class Callback : public QObject
{
  Q_OBJECT
public:
  Callback(rubyk::Worker *worker, int lua_func_idx)
   : worker_(worker),
     func_idx_(lua_func_idx) {}

  virtual ~Callback() {
    // release function
    luaL_unref(worker_->lua_, LUA_REGISTRYINDEX, func_idx_);
  }

  void connect(lua_State *L, const char *method) {
    // FIXME
    // get first parameter and make sure it is compatible with a
    // QObject and has the given callback...
//    QObject *obj = (QObject *)lua_touserdata(L, 1);
    PushButton *obj = *((PushButton**)luaL_checkudata(L, 1, "mimas.PushButton"));
    // TODO: get remote method signature (defines which callback we should use)
    //printf("connect '%s'\n", SIGNAL(clicked()));
    QObject::connect(
      obj,  method,
      this, SLOT(callback()));
  }

public slots:
  void callback() {
    rubyk::ScopedLock lock(worker_);
    // push LUA_REGISTRYINDEX on top
    lua_rawgeti(worker_->lua_, LUA_REGISTRYINDEX, func_idx_);

    // push params...

    int status = lua_pcall(worker_->lua_, 0, 0, 0);
    if (status) {
      printf("Error in receive callback: %s\n", lua_tostring(worker_->lua_, -1));
    }

    // clear stack
    lua_settop(worker_->lua_, 0);
  }

private:
  rubyk::Worker *worker_;
  int func_idx_;
};

} // mimas

#endif // RUBYK_INCLUDE_MIMAS_CALLBACK_H_