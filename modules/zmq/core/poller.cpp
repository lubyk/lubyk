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
#include "zmq/Poller.h"

namespace zmq {

pthread_key_t Poller::sThisKey = 0;

Poller::Poller(int reserve)
      : pollitems_(NULL)
      , pollitems_size_(0)
      , idx_to_pos_(NULL)
      , pos_to_idx_(NULL)
      , used_count_(0)
      , event_count_(0)
      , interrupted_(false) {
  // create a key to find 'lua_State' in current thread (used to handle
  // interrupts in Poller::poll.
  if (!sThisKey) pthread_key_create(&sThisKey, NULL);

  if (reserve <= 0) reserve = 10;
  pollitems_ = (zmq_pollitem_t*)calloc(reserve, sizeof(zmq_pollitem_t));
  if (pollitems_ == NULL) {
    throw dub::Exception("Could not pre-allocate %i pollitems", reserve);
  }

  idx_to_pos_ = (int*)calloc(reserve, sizeof(int));
  if (idx_to_pos_ == NULL) {
    free(pollitems_);
    pollitems_ = NULL;
    throw dub::Exception("Could not pre-allocate %i pollitems", reserve);
  }

  pos_to_idx_ = (int*)calloc(reserve, sizeof(int));
  if (pos_to_idx_ == NULL) {
    free(pollitems_);
    pollitems_ = NULL;
    free(idx_to_pos_);
    idx_to_pos_ = NULL;
    throw dub::Exception("Could not pre-allocate %i pollitems", reserve);
  }
  pollitems_size_ = reserve;

  memset(idx_to_pos_, -1, reserve * sizeof(int));
  memset(pos_to_idx_, -1, reserve * sizeof(int));

  setupInterruptHook();
}

} // zmq
