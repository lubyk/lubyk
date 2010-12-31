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
#ifndef RUBYK_INCLUDE_MIMAS_CALLBACK_H_
#define RUBYK_INCLUDE_MIMAS_CALLBACK_H_

#include "rubyk.h"

#include <QtCore/QObject>
#include <QtCore/QEvent>

#include "mimas/PushButton.h"

#include <string>

namespace mimas {
/** Calls a lua function back.
 *
 * @dub lib_name:'Callback_core'
 *      ignore: 'callback'
 */
class Callback : public QObject
{
  Q_OBJECT
public:
  // Our own custom event id
  static const QEvent::Type EventType;

  Callback(rubyk::Worker *worker, int lua_func_idx)
   : worker_(worker),
     func_idx_(lua_func_idx),
     delete_on_call_(false) {}

  virtual ~Callback() {
    // release function
    luaL_unref(worker_->lua_, LUA_REGISTRYINDEX, func_idx_);
  }

  void connect(QObject *obj, const char *method, const char *callback) {
    QObject::connect(
      obj,  method,
      this, callback);
  }

  void delete_on_call(bool should_delete) {
    delete_on_call_ = should_delete;
  }

  /** Simple event that should execute a callback on reception.
   */
  class Event : public QEvent
  {
    public:
      Callback *callback_;

      Event(Callback *clbk)
        : QEvent(Callback::EventType),
          callback_(clbk) {}
  };
public slots:
  void callback() {
    lua_State *L = worker_->lua_;

    rubyk::ScopedLock lock(worker_);
    // push LUA_REGISTRYINDEX on top
    lua_rawgeti(L, LUA_REGISTRYINDEX, func_idx_);

    // no params...

    int status = lua_pcall(L, 0, 0, 0);
    if (status) {
      printf("Error in receive callback: %s\n", lua_tostring(L, -1));
    }

    // clear stack
    lua_settop(L, 0);

    if (delete_on_call_) delete this;
  }

  void callback(double value) {
    lua_State *L = worker_->lua_;

    rubyk::ScopedLock lock(worker_);
    // push LUA_REGISTRYINDEX on top
    lua_rawgeti(L, LUA_REGISTRYINDEX, func_idx_);

    // 1 number param
    lua_pushnumber(L, value);

    int status = lua_pcall(L, 1, 0, 0);
    if (status) {
      printf("Error in receive callback(double): %s\n", lua_tostring(L, -1));
    }

    // clear stack
    lua_settop(L, 0);

    if (delete_on_call_) delete this;
  }

  void callback(int value) {
    lua_State *L = worker_->lua_;

    rubyk::ScopedLock lock(worker_);
    // push LUA_REGISTRYINDEX on top
    lua_rawgeti(L, LUA_REGISTRYINDEX, func_idx_);

    // 1 number param
    lua_pushnumber(L, value);

    int status = lua_pcall(L, 1, 0, 0);
    if (status) {
      printf("Error in receive callback(int): %s\n", lua_tostring(L, -1));
    }

    // clear stack
    lua_settop(L, 0);

    if (delete_on_call_) delete this;
  }

private:
  rubyk::Worker *worker_;
  int  func_idx_;
  bool delete_on_call_;
};

} // mimas

#endif // RUBYK_INCLUDE_MIMAS_CALLBACK_H_