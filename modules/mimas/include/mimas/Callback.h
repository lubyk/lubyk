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
#include "lua_cpp_helper.h"

#include <QtCore/QObject>
#include <QtCore/QEvent>

#include "mimas/PushButton.h"

#include <string>

namespace mimas {

class Application;
/** Calls a lua function back.
 *
 * @dub lib_name:'Callback_core'
 *      ignore: 'callback'
 */
class Callback : public QObject, public rubyk::LuaCallback
{
  Q_OBJECT
public:
  // Our own custom event id
  static const QEvent::Type EventType;

  Callback(rubyk::Worker *worker)
   : rubyk::LuaCallback(worker),
     self_in_registry_idx_(-1) {}

  virtual ~Callback() {}

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

    rubyk::ScopedLock lock(worker_);

    // do not push self
    push_lua_callback(false);

    // lua_ = LuaCallback's thread state
    int status = lua_pcall(lua_, 0, 0, 0);
    if (status) {
      printf("Error in receive callback: %s\n", lua_tostring(lua_, -1));
    }

    delete_on_call();
  }

  void callback(double value) {
    if (!callback_set()) {
      fprintf(stderr, "Callback triggered when function not set.");
      return;
    }

    rubyk::ScopedLock lock(worker_);

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

  /** When called from Application, the stack is
   * 1. app
   * 2. function
   */
  void set_callback_from_app(lua_State *L) {
    // push self
    lua_pushclass<Callback>(L, this, "mimas.Callback");
    lua_pushvalue(L, 2);
    lua_remove(L, 1); // remove app
    lua_remove(L, 1); // remove function
    /* Stack is now
     * 1. self
     * 2. function
     */
    lua_pushvalue(L, 1);
    // avoid GC
    self_in_registry_idx_ = luaL_ref(L, LUA_REGISTRYINDEX);
    set_callback(L);
  }


  void delete_on_call() {
    if (self_in_registry_idx_ != -1) {
      // remove from registry and let Lua GC
      luaL_unref(lua_, LUA_REGISTRYINDEX, self_in_registry_idx_);
      self_in_registry_idx_ = -1;
    }
  }

  int self_in_registry_idx_;
};

} // mimas

#endif // RUBYK_INCLUDE_MIMAS_CALLBACK_H_