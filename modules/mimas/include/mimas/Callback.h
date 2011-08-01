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
#ifndef LUBYK_INCLUDE_MIMAS_CALLBACK_H_
#define LUBYK_INCLUDE_MIMAS_CALLBACK_H_

#include "mimas/mimas.h"

#include <QtCore/QObject>
#include <QtCore/QEvent>

#include "mimas/Button.h"

#include <string>

namespace mimas {

class Application;
class Slider;

/** Calls a lua function back.
 *
 * @dub lib_name:'Callback_core'
 *      ignore: 'callback'
 */
class Callback : public QObject, public lubyk::LuaCallback
{
  Q_OBJECT
public:
  // Our own custom event id
  static const QEvent::Type EventType;

  Callback(lubyk::Worker *worker)
   : lubyk::LuaCallback(worker),
     self_in_app_env_(-1) {}

  virtual ~Callback() {
    MIMAS_DEBUG_GC
  }

  void connect(QObject *obj, const char *method, const char *callback) {
    QObject::connect(
      obj,  method,
      this, callback);
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

  /** Set callback function.
   */
  void set_callback(lua_State *L) {
    set_lua_callback(L);
  }
public slots:
  void callback() {
    if (!callback_set()) {
      fprintf(stderr, "Callback triggered when function not set.");
      return;
    }

    lubyk::ScopedLock lock(worker_);

    // do not push self
    push_lua_callback(false);

    // lua_ = LuaCallback's thread state
    int status = lua_pcall(lua_, 0, 0, 0);
    if (status) {
      printf("Error in callback: %s\n", lua_tostring(lua_, -1));
    }

    delete_on_call();
  }

  void callback(double value) {
    if (!callback_set()) {
      fprintf(stderr, "Callback triggered when function not set.");
      return;
    }

    lubyk::ScopedLock lock(worker_);

    // do not push self
    push_lua_callback(false);

    // 1 number param
    lua_pushnumber(lua_, value);

    // lua_ = LuaCallback's thread state
    int status = lua_pcall(lua_, 1, 0, 0);

    if (status) {
      fprintf(stderr, "Error in receive callback: %s\n", lua_tostring(lua_, -1));
    }

    delete_on_call();
  }

  void callback(int value) {
    callback((double)value);
  }

private:
  friend class Application;
  friend class Slider;

  /** When called from Application, the stack is
   * 1. app or widget
   * 2. function
   */
  void set_callback_from_app(lua_State *L) {
    // push self
    // ... <app> <func>
    lua_getfenv(L, -2);
    // ... <app> <func> <env>
    lua_pushclass<Callback>(L, this, "mimas.Callback");
    // ... <app> <func> <env> <clbk>
    lua_pushvalue(L, -1);
    // ... <app> <func> <env> <clbk> <clbk>
    self_in_app_env_ = luaL_ref(L, -3);
    // ... <app> <func> <env> <clbk>
    lua_pushvalue(L, -3);
    // ... <app> <func> <env> <clbk> <func>
    set_callback(L);
    // ... <app> <func> <env> <clbk> <func>
    lua_pushvalue(L, -3);
    // ... <app> <func> <env> <clbk> <func> <env>
    lua_xmove(L, lua_, 1);
    // L = <app> <func> <env> <clbk> <func>
    // lua_ = ... <env>
    app_env_in_thread_ = luaL_ref(lua_, LUA_REGISTRYINDEX);
    // lua_ = ...
    lua_settop(L, -4);
    // ... <app> <func>
  }

  void delete_on_call() {
    if (self_in_app_env_ != -1) {
      lua_rawgeti(lua_, LUA_REGISTRYINDEX, app_env_in_thread_);
      // ... <env>
      luaL_unref(lua_, -1, self_in_app_env_);
      // Lua will GC this
      self_in_app_env_ = -1;
    }
  }

  /** To protect from garbage collection before app is deleted.
   */
  int self_in_app_env_;

  /** To unprotect self from garbage collection when deleted.
   */
  int app_env_in_thread_;
};

} // mimas

#endif // LUBYK_INCLUDE_MIMAS_CALLBACK_H_