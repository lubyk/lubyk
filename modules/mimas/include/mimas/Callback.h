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
class Callback : public QObject, public lubyk::LuaObject
{
  Q_OBJECT
public:
  // Our own custom event id
  static const QEvent::Type EventType;

  Callback() :
    self_in_app_(-1) {}

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

public slots:
  void callback() {
    // lua_ = own Lua thread
    lua_State *L = lua_;

    lubyk::ScopedLock lock(worker_);

    pushLuaCallback("callback");
    // <func> <self>

    int status = lua_pcall(L, 1, 0, 0);
    if (status) {
      printf("Error in callback: %s\n", lua_tostring(L, -1));
    }

    deleteOnCall();
  }

  void callback(double value) {
    // lua_ = own Lua thread
    lua_State *L = lua_;

    lubyk::ScopedLock lock(worker_);

    pushLuaCallback("callback");
    // <func> <self>

    lua_pushnumber(L, value);
    // <func> <self> <number>

    int status = lua_pcall(L, 2, 0, 0);

    if (status) {
      fprintf(stderr, "Error in receive callback: %s\n", lua_tostring(lua_, -1));
    }

    deleteOnCall();
  }

  void callback(int value) {
    callback((double)value);
  }

private:
  friend class Application;
  friend class Slider;

  /** Define a callback and store a reference inside the application
   * so that it is not garbage collected too soon.
   */
  void setCallbackFromApp(lua_State *L) {
    // ... <app> <func> <clbk>
    lua_pushvalue(L, -2);
    // ... <app> <func> <clbk> <func>
    lua_pushlstring(L, "callback", 8);
    // ... <app> <func> <clbk> <func> <'callback'>
    lua_settable(L, -3);  // clbk.callback = func
    // ... <app> <func> <clbk>
    lua_pushvalue(L, -1);
    // ... <app> <func> <clbk> <clbk>
    self_in_app_ = luaL_ref(L, -4);
    // ... <app> <func> <clbk>


    // Now store 'app' in the callback to remove oneself on delete.
    lua_pushvalue(L, -3);
    // L    = ... <app> <func> <clbk> <app>
    lua_xmove(L, lua_, 1);
    // L    = ... <app> <func> <clbk>
    // lua_ = ... <app>
    app_in_thread_ = luaL_ref(lua_, LUA_REGISTRYINDEX);
    // lua_ = ...
    // L    = ... <app> <func> <clbk>
  }

  void deleteOnCall() {
    if (self_in_app_ != -1) {
      lua_rawgeti(lua_, LUA_REGISTRYINDEX, app_in_thread_);
      // ... <app>
      // cut link from app to callback: app->clbk
      luaL_unref(lua_, -1, self_in_app_);
      // Now the callback is not reachable and will be GC
      self_in_app_ = -1;
    }
  }

  /** To protect from garbage collection before app is deleted.
   */
  int self_in_app_;

  /** To unprotect self from garbage collection when deleted.
   */
  int app_in_thread_;
};

} // mimas

#endif // LUBYK_INCLUDE_MIMAS_CALLBACK_H_
