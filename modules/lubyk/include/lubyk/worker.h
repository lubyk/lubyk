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

#ifndef LUBYK_INCLUDE_LUBYK_WORKER_H_
#define LUBYK_INCLUDE_LUBYK_WORKER_H_
/*
Structure of a running planet:

The Planet is an oscit::Root:
Root   <- Planet

It contains a single Worker that is passed to subnodes as context
Planet <>--- Worker
*/
#include "lubyk/lua.h"
#include "lubyk/thread.h"
#include "lubyk/rmutex.h"
#include "lubyk/time_ref.h"

namespace lubyk {

/** The worker should be created only once and is responsible for
 * timing and lua locking.
 *
 * @dub string_format:'%%f'
 *      string_args:'(*userdata)->now()'
 */
class Worker : public RMutex
{
  class Implementation;
  Implementation *impl_;
public:
  /** Time reference. All times are in milliseconds (as double) from this reference.
   * 0.0 = The worker's birthdate !
   */
  TimeRef time_ref_;

  /** Main Lua state in which the worker was required.
   */
  lua_State *lua_;

  /** Context use by zmq::Socket.
   */
  void *zmq_context_;

  /** Counts the number of zmq::Socket depending on the
   * socket.
   */
  size_t zmq_context_refcount_;

public:

  Worker(lua_State *L);

  ~Worker();

  /** Sleep for a given number of ms. Should not be used in nodes (risk of Lua State corruption).
   */
  void sleep(double duration) {
    ScopedUnlock unlock(this);
    Thread::millisleep(duration);
  }

  /** Wait (lock sleep) for a given number of ms.
   */
  void wait(double duration) {
    Thread::millisleep(duration);
  }

  /** Start a new process with the given Lua script.
   * @return new process id or nil on failure
   */
  LuaStackSize spawn(const char *script, lua_State *L);

  /** Wait for another process to finish.
   */
  int waitpid(int pid);

  /** Exit with a given status.
   */
  void exit(int status);

  /** Get the current executable's path.
   */
  LuaStackSize execPath(lua_State *L);

  /** Used for testing. */
  void test_lock() {
    lock();
  }

  /** Used for testing. */
  void test_unlock() {
    unlock();
  }

  void run();

  double now() {
    return time_ref_.elapsed();
  }

  static Worker *getWorker(lua_State *L);
};

} // lubyk

#endif // LUBYK_INCLUDE_LUBYK_WORKER_H_
