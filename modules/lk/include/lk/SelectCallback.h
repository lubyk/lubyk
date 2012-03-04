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
#ifndef LUBYK_INCLUDE_LK_SELECT_CALLBACK_H_
#define LUBYK_INCLUDE_LK_SELECT_CALLBACK_H_

#include "lubyk.h"
using namespace lubyk;

namespace lk {
/** This class is used to help integrate foreign libraries inside our
 * coroutine based Scheduler. This class acts in a similar way to Qt's
 * QSocketNotifier.
 *
 * Sub-classes of this class are created in C++ and lua triggers the
 * callback() virtual when the file descriptor has appropriate events or
 * the timeout event is reached.
 *
 * The C++ code *must* push this class into Lua with
 *
 *     luaInit(L, this, "lk.SelectCallback");
 *
 * and then call :init() on the lua class and ensure it is not garbage
 * collected (keep a link to the object from Lua).
 * 
 * @dub lib_name: 'SelectCallback_core'
 *      destructor: 'finalize'
 */
class SelectCallback : public ThreadedLuaObject {
  int fd_;
  double timeout_;
public:
  SelectCallback(int fd)
    : fd_(fd)
    , timeout_(-1) {
  }

  virtual ~SelectCallback() {
    if (fd_ != 0 || timeout_ >= 0) {
      // Call to Lua to remove the file descriptor.
      remove();
      fd_ = 0;
      timeout_ = -1;
    }
  }

  /** This is the callback called when the file descriptor has the
   * required events. It should be reimplemented by the sub-classes.
   */
  virtual void callback(bool read, bool write, bool timeout) {}

  /** Destruction initiated by Lua. We should not update when this is
   * called.
   */
  void finalize() {
    fd_ = 0;
    timeout_ = -1;
    luaDestroy();
  }

  int fd() {
    return fd_;
  }
protected:
  /** Called by C++.
   */
  void update(bool read, bool write, double timeout) {
    lua_State *L = lua_;
    if (!pushLuaCallback("update")) {
      throw dub::Exception("Update callback not set.");
    }
    lua_pushboolean(L, read);
    lua_pushboolean(L, write);
    lua_pushnumber(L, timeout);
    // <update> <self> <read> <write> <timeout>
    int status = lua_pcall(lua_, 4, 0, 0);

    if (status) {
      printf("Error in 'update' function: %s\n", lua_tostring(lua_, -1));
    }
    timeout_ = timeout;
  }
  
private:
  /** Triggered by C++ before delete.
   */
  void remove() {
    lua_State *L = lua_;
    if (!pushLuaCallback("remove")) {
      throw dub::Exception("Remove callback not set.");
    }
    // <remove> <self>
    int status = lua_pcall(L, 1, 0, 0);

    if (status) {
      printf("Error in 'remove' function: %s\n", lua_tostring(lua_, -1));
    }
  }
};

} // lk

#endif // LUBYK_INCLUDE_LK_SELECT_CALLBACK_H_

