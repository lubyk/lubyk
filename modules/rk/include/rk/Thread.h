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
#ifndef RUBYK_INCLUDE_RK_THREAD_H_
#define RUBYK_INCLUDE_RK_THREAD_H_

#include "rubyk.h"
#include "rubyk/thread.h"
#include "rubyk/worker.h"

namespace rk {
/** Starts a new OS Thread with a given function.
 * @dub lib_name:'Thread_core'
 */
class Thread : public rubyk::LuaCallback, public rubyk::Thread
{
public:
  Thread(rubyk::Worker *worker, int lua_func_idx)
    : rubyk::LuaCallback(worker, lua_func_idx) {
    start_thread<Thread, &Thread::run>(this, NULL);
  }

  ~Thread() {}

  void quit() {
    rubyk::Thread::quit();
  }

  void kill() {
    rubyk::Thread::kill();
  }

  void join() {
    rubyk::ScopedUnlock unlock(worker_);
    rubyk::Thread::join();
  }

  bool should_run() {
    return rubyk::Thread::should_run();
  }

private:
  void run(rubyk::Thread *runner) {
    // L = LuaCallback's thread state

    runner->thread_ready();

    rubyk::ScopedLock lock(worker_);

    // FIXME: when all is clean, we should not need this
    lua_settop(L, 0);

    push_lua_callback();

    int status = lua_pcall(L, 0, 0, 0);

    if (status) {
      printf("Error starting thread function: %s\n", lua_tostring(L, -1));
    }

    // FIXME: maybe we should release LuaCallback thread_idx_ to allow GC here
    // FIXME: how to ensure this rk.Thread is not GC during callback ?
  }
};

} // rk
#endif // RUBYK_INCLUDE_RK_THREAD_H_