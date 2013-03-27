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

#ifndef LUBYK_INCLUDE_LK_FIFO_H_
#define LUBYK_INCLUDE_LK_FIFO_H_

#include "dub/dub.h" // dub::Exception
#include "lk/Mutex.h"

#include <unistd.h> // pipe
#include <fcntl.h>

// lk::Fifo used by midi module

namespace lk {

/** This is a utility class that should be used to wrap objects that need to run
 * in a different OS thread. The fifo provides a file descriptor and a 'pop'
 * method to get the last message.
 *
 * This fifo supports multiple reader and writer threads.
 */
template<class T>
class Fifo {
  /** Current read position in the queue.
   */
  int read_idx_;

  /** Next write position in the queue.
   */
  int write_idx_;

  /** The buffer should never fill up so it does not need to be large
   */
  static const int BUFFER_SIZE = 1024;

  /** Heap allocated data.
   */
  T* data_[BUFFER_SIZE];

  /** File descriptors used by the pipe.
   */
  int pipe_fd_[2];

  /** Mutex to protect reads and writes and avoid relying on
   * volatile or other non-portable code.
   */
  Mutex mutex_;
public:
  Fifo()
      : read_idx_(0)
      , write_idx_(0) {
    // setup the pipe
    if (pipe(pipe_fd_) == -1) {
      throw dub::Exception("Could not setup pipe during Fifo creation (%s).", strerror(errno));
    }
    
    // Set pipe fd as beeing NON-BLOCKING
    fcntl(pipe_fd_[0], F_SETFL, fcntl(pipe_fd_[0], F_GETFL) | O_NONBLOCK);
    fcntl(pipe_fd_[1], F_SETFL, fcntl(pipe_fd_[1], F_GETFL) | O_NONBLOCK);
  }

  virtual ~Fifo() {
    char read_buffer[20];
    // empty buffer
    while (read_idx_ != write_idx_) {
      int next = (read_idx_ + 1) % BUFFER_SIZE;
      T *data = data_[read_idx_];
      delete data;
      read_idx_ = next;
      // get a char from pipe
      read(pipe_fd_[0], read_buffer, 1);
    }
  }

  /** Return a filedescriptor that signals whenever the queue fills up
   * (edge-trigger).
   */
  int fd() const {
    return pipe_fd_[0];
  }

  /** Pop an element from the queue. Before calling this method, the calling
   * code should make sure there is something to be read with 
   * 'sched:waitRead(fd)'.
   *
   * @return multiple values
   */
  LuaStackSize pop(lua_State *L) {
    ScopedLock lock(mutex_);
    static char read_buffer[20];
    if (read_idx_ == write_idx_) {
      // empty queue
      return 0;
    } else {
      int next = (read_idx_ + 1) % BUFFER_SIZE;
      T *data = data_[read_idx_];
      read_idx_ = next;
      // get a char from pipe
      read(pipe_fd_[0], read_buffer, 1);
      int ret = unpack(L, data);
      delete data;
      return ret;
    }
  }

  /** Return true if there are messages in the message queue.
   */
  bool hasMessage() {
    return read_idx_ != write_idx_;
  }

protected:

  /** Push data onto the lua stack.
   */
  virtual LuaStackSize unpack(lua_State *L, T *data) = 0;

  /** Push data in the queue.
   * @return false if the buffer is full.
   */
  bool push(T *data) {
    ScopedLock lock(mutex_);
    int next = (write_idx_ + 1) % BUFFER_SIZE;
    if (next != read_idx_) {
      data_[write_idx_] = data;
      write_idx_ = next;
      // Dummy message
      write(pipe_fd_[1], "", 1);
      return true;
    } else {
      // buffer full
      delete data;
      return false;
    }
  }
private:  
  // NonCopyable
  Fifo(const Fifo &other);
  Fifo &operator=(const Fifo &other);
};

} // lk

#endif // LUBYK_INCLUDE_LK_FIFO_H_


