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
#ifndef LUBYK_INCLUDE_MIMAS_TIMER_H_
#define LUBYK_INCLUDE_MIMAS_TIMER_H_

#include "mimas/mimas.h"

#include <QtCore/QTimer>

using namespace lubyk;

namespace mimas {

/** The Timer class calls the "timeout" callback at regular intervals.
 *
 * @dub lib_name:'Timer_core'
 *      super: 'QObject'
 */
class Timer : public QTimer, public ThreadedLuaObject {
  Q_OBJECT
public:

  /** Private constructor. Use MakeApplication instead.
   */
  Timer(int timeout)
      : QTimer(0)
  {
    setInterval(timeout);
    QObject::connect(this, SIGNAL(timeout()),
                     this, SLOT(timeoutSlot()));

    //printf("[%p] Timer\n", this);
    MIMAS_DEBUG_CC
  }

  virtual ~Timer() {
    //printf("[%p] ~Timer\n", this);
    MIMAS_DEBUG_GC
  }

  void start(int timeout) {
    QTimer::start(timeout);
  }

  void start() {
    QTimer::start();
  }

  void stop() {
    QTimer::stop();
  }

  void setInterval(int msec) {
    QTimer::setInterval(msec);
  }

private slots:
  void timeoutSlot() {
    lua_State *L = lua_;
    if (pushLuaCallback("timeout")) {
      // <func> <self>
      int status = lua_pcall(L, 1, 0, 0);

      if (status) {
        // cannot raise an error here...
        fprintf(stderr, "Error in 'timeout': %s\n", lua_tostring(L, -1));
      }
    } else {
      printf("NO CALLBACK!\n");
    }
  }
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_TIMER_H_

