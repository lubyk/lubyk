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
#ifndef LUBYK_INCLUDE_LUBYK_MSGPACK_H_
#define LUBYK_INCLUDE_LUBYK_MSGPACK_H_

#include "lubyk.h"

// This header only declares msgpack_bin_to_lua and msgpack_lua_to_bin with
// the minimal other information needed to use these.
// If you need to manually pack/unpack data, please include 'msgpack/msgpack.h'
// methods defined in msgpack/vendor/lubyk.c
extern "C" {
  typedef struct msgpack_sbuffer {
  	size_t size;
  	char* data;
  	size_t alloc;
  } msgpack_sbuffer;

  void msgpack_lua_to_bin(lua_State *L, msgpack_sbuffer **buffer, int skip_index = 0);
  int  msgpack_bin_to_lua(lua_State *L, void *msg, size_t msg_len);
  void free_msgpack_msg(void *data, void *buffer);

}


#endif // LUBYK_INCLUDE_LUBYK_MSGPACK_H_
