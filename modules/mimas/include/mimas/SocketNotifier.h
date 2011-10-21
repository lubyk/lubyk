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
#ifndef LUBYK_INCLUDE_MIMAS_SOCKET_NOTIFIER_H_
#define LUBYK_INCLUDE_MIMAS_SOCKET_NOTIFIER_H_

#include "mimas/mimas.h"

#include <QtCore/QSocketNotifier>

using namespace lubyk;

namespace mimas {

/** SocketNotifer wraps filedescriptors so that they can be used
 * in Qt's event loop. A SocketNotifer can only be used for a single
 * file descriptor and a single event (read or write).
 *
 * @dub lib_name:'SocketNotifier_core'
 *      super: 'QObject'
 */
class SocketNotifier : public QSocketNotifier, public ThreadedLuaObject {
  Q_OBJECT
public:
  enum EventType {
    Read  = QSocketNotifier::Read,
    Write = QSocketNotifier::Write,
  };

  /** Private constructor. Use MakeApplication instead.
   */
  SocketNotifier(int fd, int event_type)
      : QSocketNotifier(fd, (QSocketNotifier::Type)event_type) {
    QObject::connect(this, SIGNAL(activated(int)),
                     this, SLOT(activatedSlot(int)));

    //printf("[%p] SocketNotifier\n", this);
    MIMAS_DEBUG_CC
  }

  virtual ~SocketNotifier() {
    //printf("[%p] ~SocketNotifier\n", this);
    MIMAS_DEBUG_GC
  }

  int socket() const {
    return QSocketNotifier::socket();
  }


  /** Enable or disable a socket notifier.
   */
  void setEnabled(bool enabled) {
    QSocketNotifier::setEnabled(enabled);
  }

private slots:
  void activatedSlot(int socket) {
    //printf("[%p] activatedSlot %i (%s)\n", this, socket, isEnabled() ? "ON" : "OFF");
    // Callback
    // FIXME: ThreadedLuaObject stores L as lua_ (no need for a new thread)
    lua_State *L = lua_;
    // FIXME: We might need less lua threads and faster access to callbacks.
    // We could use:
    // 1. ref for global weak value table set on create (func_list_idx_)
    // lua_rawgeti(L, LUA_REGISTRYINDEX, func_list_idx_)
    // 2. ref for callback set in weak on create (luaL_ref)
    // lua_rawgeti(L, -1, clbk_idx_)
    //
    if (pushLuaCallback("callback")) {
      // <func> <self>
      int status = lua_pcall(L, 1, 0, 0);

      if (status) {
        // cannot raise an error here...
        fprintf(stderr, "Error in 'callback': %s\n", lua_tostring(L, -1));
      }
    } else {
      printf("NO CALLBACK!\n");
    }
  }
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_SOCKET_NOTIFIER_H_
