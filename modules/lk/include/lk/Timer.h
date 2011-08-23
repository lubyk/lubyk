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
#ifndef LUBYK_INCLUDE_LK_TIMER_H_
#define LUBYK_INCLUDE_LK_TIMER_H_

#include "lubyk.h"
#include "lubyk/timer.h"
#include "lubyk/worker.h"

namespace lk {
/** Calls a lua function back at regular intervals. If the called function returns
 * a number, the number sets the new interval (0 = stop).
 * @dub string_format:'%%f (%%s)'
 *      string_args:'(float)(*userdata)->interval(), (float)(*userdata)->isRunning() ? "ON" : "OFF"'
 *      lib_name:'Timer_core'
 */
class Timer : public lubyk::LuaCallback2
{
public:
  Timer(float interval) :
    timer_(this, interval) {}

  ~Timer() {
    stop();
  }

  void stop() {
    timer_.stop();
  }

  void start(bool trigger_on_start = true) {
    timer_.start(trigger_on_start);
  }

  void join() {
    lubyk::ScopedUnlock unlock(worker_);
    timer_.join();
  }

  bool isRunning() {
    return timer_.isRunning();
  }

  time_t interval() {
    return timer_.interval();
  }

  void setInterval(float interval) {
    timer_.setInterval(interval);
  }

private:
  void tick() {
    // lua_ = LuaCallback's thread state

    // find function and call
    lubyk::ScopedLock lock(worker_);

    pushLuaCallback("tick", 4);

    // lua_ = LuaCallback's thread state
    // @return new interval or 0 to stop or nil to not change interval
    int status = lua_pcall(lua_, 1, 1, 0);

    if (status) {
      printf("Error triggering timer: %s\n", lua_tostring(lua_, -1));
    }

    if (lua_type(lua_, -1) == LUA_TNUMBER) {
      float interval = lua_tonumber(lua_, -1);
      if (interval == 0) {
        timer_.stopFromLoop();
      } else if (interval < 0) {
        luaL_error(lua_, "Timer interval must be a positive value (got %f)\n", interval);
      } else {
        timer_.setIntervalFromLoop(interval);
      }
    }

    lua_pop(lua_, 1);
  }

  lubyk::Timer<lk::Timer, &lk::Timer::tick> timer_;
};

} // lk

#endif // LUBYK_INCLUDE_LK_TIMER_H_
