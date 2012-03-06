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

#include "dub/dub.h"

#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


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
 * @dub destructor: 'luaDestroy'
 *      push: pushobject
 *      register: SelectCallback_core
 */
class SelectCallback : public dub::Thread {
protected:
  int fd_;
  double timeout_;
  bool should_read_;
  bool should_write_;
  bool updating_;
public:
  SelectCallback(int fd)
    : fd_(fd)
    , timeout_(-1)
    , updating_(false)
  {
  }

  virtual ~SelectCallback() {
    if (fd_ != 0 || timeout_ >= 0) {
      // Call to Lua to remove the file descriptor or timeout.
      remove();
      fd_ = 0;
      timeout_ = -1;
    }
  }

  /** This is the callback called when the file descriptor has the
   * required events. It should be reimplemented by the sub-classes.
   */
  virtual void callback(bool read, bool write, bool timeout) {}

  /** Destruction initiated by Lua. We invalidate 'remove' before
   * destruction.
   */
  void luaDestroy() {
    fd_ = 0;
    timeout_ = -1;
    delete this;
  }

  int fd() {
    return fd_;
  }
protected:
  /** Called by C++. In case the fd is level triggered, we
   * must empty it before inserting it in our edge-triggered
   * loop.
   */
  void update(bool read, bool write, double timeout) {

    should_read_  = read;
    should_write_ = write;
    timeout_      = timeout;

    // Doesn't seem to be needed for mdns. Enable via an option in
    // lk.SelectCallback ?
#if 0
    // This method is reintrant. Make sure we only update Lua
    // once.
    bool should_update = !updating_;
    updating_ = true;

    if (!should_update) {
      // Only run the select in the first update.
      return;
    }

    // Before inserting into the scheduler, make sure we empty
    // the file descriptor (we are edge triggered, not level).
    if (fd_ > 0 && (should_read_ || should_write_)) {
      fd_set base_set;
      FD_ZERO(&base_set);
      FD_SET(fd_, &base_set);

      fd_set read_set;
      fd_set write_set;
      struct timeval timeout;
      bool more = true;
      while (more && (should_read_ || should_write_)) {
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        if (should_read_) {
          read_set = base_set;
        } else {
          FD_ZERO(&read_set);
        }
        if (should_write_) {
          write_set = base_set;
        } else {
          FD_ZERO(&write_set);
        }
        int status = select(fd_+1, &read_set, &write_set, NULL, &timeout);
        if (status == -1) {
          printf("Select error in lk.SelectCallback (%s).", strerror(errno));
          // error
          break;
        } else if (status > 0) {
          printf("We have something: %i.\n", status);
          more = true;
          if (FD_ISSET(fd_, &read_set)) {
            callback(true, false, false);
          }
          if (FD_ISSET(fd_, &write_set)) {
            callback(false, true, false);
          }
        } else {
          // Nothing left: empty fd.
          printf("Empty fd %i.\n", fd_);
          break;
        }
      }
    }
    updating_ = false;
#endif

    if (!dub_pushcallback("update")) {
      throw dub::Exception("Update callback not set.");
    }
    lua_pushboolean(dub_L, should_read_);
    lua_pushboolean(dub_L, should_write_);
    lua_pushnumber(dub_L, timeout_);
    // <update> <self> <read> <write> <timeout>
    dub_call(4, 0);
  }
  
private:
  /** Triggered by C++ before delete.
   */
  void remove() {
    if (!dub_pushcallback("remove")) {
      throw dub::Exception("Remove callback not set.");
    }
    // <remove> <self>
    dub_call(1, 0);
  }
};

} // lk

#endif // LUBYK_INCLUDE_LK_SELECT_CALLBACK_H_

