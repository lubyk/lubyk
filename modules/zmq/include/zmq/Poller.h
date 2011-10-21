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
#ifndef LUBYK_INCLUDE_ZMQ_POLLER_H_
#define LUBYK_INCLUDE_ZMQ_POLLER_H_


#include "lubyk.h"

#include <stdlib.h> // rand()
#include <time.h>   // time()
#include <string>

#include "../vendor/include/zmq.h"
#include "lubyk/msgpack.h"
#include "lubyk/time_ref.h"
#include "zmq/Socket.h"

using namespace lubyk;

namespace zmq {

/** ZeroMQ polling helper.
 *
 * @dub lib_name:'Poller_core'
 *      string_format:' (%%i)'
 *      string_args:'(*userdata)->count()'
 */
class Poller {
  /** Contiguous array of pollitems.
   */
  zmq_pollitem_t *pollitems_;

  /** Size of the pollitems array.
   */
  int pollitems_size_;

  /** Resolve idx to actual position in pollitems.
   * (needed because pollitems can move due to array compaction).
   */
  int *idx_to_pos_;

  /** Resolve pos to idx.
   */
  int *pos_to_idx_;

  /** Number of slots used in the pollitems.
   */
  int used_count_;

  /** Event count (set during poll operation).
   */
  int event_count_;

  /** Time reference for precise timing.
   */
  TimeRef time_;

  /** We set this to true when we receive a SIGINT. This enables
   * poll to return false.
   */
  bool interrupted_;
public:
  /** Create a zmq poller and reserve free slots.
   */
  Poller(int reserve=8)
      : pollitems_(NULL),
        pollitems_size_(0),
        idx_to_pos_(NULL),
        pos_to_idx_(NULL),
        used_count_(0),
        event_count_(0),
        interrupted_(false) {
    if (reserve < 0) reserve = 10;
    pollitems_ = (zmq_pollitem_t*)calloc(reserve, sizeof(zmq_pollitem_t));
    if (pollitems_ == NULL) {
      throw Exception("Could not pre-allocate %i pollitems", reserve);
    }

    idx_to_pos_ = (int*)calloc(reserve, sizeof(int));
    if (idx_to_pos_ == NULL) {
      free(pollitems_);
      throw Exception("Could not pre-allocate %i pollitems", reserve);
    }

    pos_to_idx_ = (int*)calloc(reserve, sizeof(int));
    if (pos_to_idx_ == NULL) {
      free(pollitems_);
      free(idx_to_pos_);
      throw Exception("Could not pre-allocate %i pollitems", reserve);
    }
    pollitems_size_ = reserve;

    memset(idx_to_pos_, -1, reserve * sizeof(int));
    memset(pos_to_idx_, -1, reserve * sizeof(int));

    setupInterruptHook();
  }

  ~Poller() {
    if (pollitems_) {
      free(pollitems_);
      free(idx_to_pos_);
      free(pos_to_idx_);
    }
  }

  /** Polls for new events and ensures that if we timeout, this is done
   * as precisely as possible (pads with nanosleep).
   * @return true on success, false on interruption.
   * @param timeout in milliseconds.
   */
  bool poll(float timeout) {
    // interruption can occur between poll operations
    if (interrupted_) return false;
    // printf("============================= %f\n", timeout);
    // for(int i=0; i<used_count_; ++i) {
    //   zmq_pollitem_t *item = pollitems_ + i;
    //   printf("%i: %2i / %p (%i)\n", i, item->fd, item->socket, item->events);
    // }
#if POLLER_JITTER_HACK
    if (timeout > 0) {
      double start = time_.elapsed();
      long t = timeout > 2 ? timeout - 2 : timeout;
      // zmq counts micro seconds
      event_count_ = zmq_poll(pollitems_, used_count_, t * 1000);
      if (event_count_ < 0) {
        // error or interruption
        event_count_ = 0;
        if (!interrupted_) {
          throw Exception("An error occured during zmq_poll (%s)", zmq_strerror(zmq_errno()));
        } else {
          return false;
        }
      } else if (!event_count_) {
        // timed out
        double remaining = timeout - time_.elapsed() + start;
        // TODO: We must measure that this 'hack' improves jitter.
        if (remaining > 0) Thread::millisleep(remaining);
      }
    } else {
      // no timeout or 0
      event_count_ = zmq_poll(pollitems_, used_count_, timeout);
      if (event_count_ < 0) {
        // error or interruption
        event_count_ = 0;
        if (!interrupted_) {
          throw Exception("An error occured during zmq_poll (%s)", zmq_strerror(zmq_errno()));
        } else {
          return false;
        }
      }
    }
#else
    event_count_ = zmq_poll(pollitems_, used_count_, timeout * 1000);
    // printf("===> %i\n", event_count_);
    if (event_count_ < 0) {
      // error or interruption
      event_count_ = 0;
      if (!interrupted_) {
        throw Exception("An error occured during zmq_poll (%s)", zmq_strerror(zmq_errno()));
      } else {
        return false;
      }
    }
#endif
    return true;
  }

  /** Return a table with all event idx or nil.
   */
  LuaStackSize events(lua_State *L) {
    if (!event_count_) return 0;
    lua_newtable(L);
    // <table>
    int pos = 0;
    for(int i=0; i < used_count_; ++i) {
      zmq_pollitem_t *item = pollitems_ + i;
      if (item->revents) {
        lua_pushnumber(L, pos_to_idx_[i]);
        // <table> <idx>
        lua_rawseti(L, -2, ++pos);
      }
      if (pos == event_count_) break;
    }
    event_count_ = 0;
    return 1;
  }

  int add(int fd, int events) {
    return addItem(fd, NULL, events);
  }

  int add(zmq::Socket *sock, int events) {
    return addItem(0, sock->socket_, events);
  }

  /** Modify an item's events by its id.
   */
  void modify(int idx, int events, lua_State *L) {
    assert(events);
    assert(idx < pollitems_size_ && idx >= 0);
    zmq_pollitem_t *item = pollitems_ + idx_to_pos_[idx];
    item->events = events;
    int top = lua_gettop(L);
    // <self> <idx> <events> <new_fd>
    if (top > 3) {
      if (lua_isuserdata(L, 4)) {
        zmq::Socket *sock = *((zmq::Socket **)dubL_checksdata(L, 4, "zmq.Socket"));
        item->fd = 0;
        item->socket = sock->socket_;
      } else {
        int fd = dubL_checkint(L, 4);
        item->fd = fd;
        item->socket = NULL;
      }
    }
  }

  /** Remove an item by its id.
   */
  void remove(int idx) {
    assert(idx < pollitems_size_ && idx >= 0);
    int last_pos = used_count_ - 1;
    int pos = idx_to_pos_[idx];
    idx_to_pos_[idx] = -1; // now free
    --used_count_;
    if (pos == last_pos) {
      // we removed the last element, we are done.
      return;
    }
    // move last item in the position where idx was
    int last_idx = pos_to_idx_[last_pos];
    pos_to_idx_[last_pos] = -1;
    idx_to_pos_[last_idx] = pos;
    pos_to_idx_[pos] = last_idx;
    // pollitems_[pos] <== pollitems_[last_pos];
    memcpy(pollitems_ + pos, pollitems_ + last_pos, sizeof(zmq_pollitem_t));
  }

  int count() {
    return used_count_;
  }

  /** Used for testing only.
   * @return pos or nil
   */
  LuaStackSize idxToPos(int idx, lua_State *L) {
    if (idx >= pollitems_size_ || idx < 0) return 0;
    lua_pushnumber(L, idx_to_pos_[idx]);
    return 1;
  }

  /** Used for testing only.
   * @return pos or nil
   */
  LuaStackSize posToIdx(int pos, lua_State *L) {
    if (pos >= pollitems_size_ || pos < 0) return 0;
    lua_pushnumber(L, pos_to_idx_[pos]);
    return 1;
  }

  /** Used for testing only.
   * @return fd or nil
   */
  LuaStackSize posToFd(int pos, lua_State *L) {
    if (pos >= used_count_ || pos < 0) return 0;
    lua_pushnumber(L, pollitems_[pos].fd);
    return 1;
  }
  
  /** Used for testing only.
   * @return fd or nil
   */
  LuaStackSize posToEvent(int pos, lua_State *L) {
    if (pos >= used_count_ || pos < 0) return 0;
    lua_pushnumber(L, pollitems_[pos].events);
    return 1;
  }


  static pthread_key_t sThisKey;
private:
  int addItem(int fd, void *socket, int events) {
    // printf("addItem fd:%i socket:%p\n", fd, socket);
    if (used_count_ >= pollitems_size_) {
      // we need more space: realloc
      int *sptr = (int*)realloc(idx_to_pos_, pollitems_size_ * 2 * sizeof(int));
      if (!sptr) {
        throw Exception("Could not reallocate %i pollitems.", pollitems_size_ * 2);
      }
      idx_to_pos_ = sptr;
      sptr = NULL;
      sptr = (int*)realloc(pos_to_idx_, pollitems_size_ * 2 * sizeof(int));
      if (!sptr) {
        throw Exception("Could not reallocate %i pollitems.", pollitems_size_ * 2);
      }
      pos_to_idx_ = sptr;
      zmq_pollitem_t *ptr = (zmq_pollitem_t*)realloc(pollitems_, pollitems_size_ * 2 * sizeof(zmq_pollitem_t));
      if (!ptr) {
        throw Exception("Could not reallocate %i pollitems.", pollitems_size_ * 2);
      }
      pollitems_ = ptr;
      // clear new space (same size as pollitems_size_ because we double).
      memset(idx_to_pos_+ used_count_, -1, pollitems_size_ * sizeof(int));
      memset(pos_to_idx_+ used_count_, -1, pollitems_size_ * sizeof(int));
      memset(pollitems_ + used_count_,  0, pollitems_size_ * sizeof(zmq_pollitem_t));
      pollitems_size_ = 2 * pollitems_size_;
    }
    int pos = used_count_;
    ++used_count_;
    zmq_pollitem_t *item = pollitems_ + pos;
    item->fd = fd;
    item->socket = socket;
    item->events = events;
    // Hasn't been moved yet: idx == position
    // find a free idx
    int idx;
    for(idx = 0; idx < pollitems_size_; ++idx) {
      if (idx_to_pos_[idx] < 0) break;
    }
    idx_to_pos_[idx] = pos;
    pos_to_idx_[pos] = idx;

    return idx;
  }

  static void sInterrupted(int i) {
    signal(i, SIG_DFL); // double interrupt == kill
    Poller *p = (Poller*)pthread_getspecific(sThisKey);
    p->interrupted_ = true;
    // continue
  }

  void setupInterruptHook() {
    Poller *p = (Poller*)pthread_getspecific(sThisKey);
    if (!p) {
      // only register first (main) Scheduler
      pthread_setspecific(sThisKey, (void*)this);
      signal(SIGINT, sInterrupted);
    }
  }
};
} // zmq

#endif // LUBYK_INCLUDE_ZMQ_POLLER_H_
