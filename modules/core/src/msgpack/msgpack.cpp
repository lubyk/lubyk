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

#include "dub/dub.h" // dub::Exception
#include "msgpack.h"

#define MAX_DEPTH 10000
static void pack_lua(lua_State *L, msgpack_packer *pk, int index);

void free_msgpack_msg(void *data, void *buffer) {
  msgpack_sbuffer_free((msgpack_sbuffer*)buffer);
}

static void pack_string(lua_State *L, msgpack_packer *pk, int index) {
  size_t sz;
  const char* str = lua_tolstring(L, index, &sz);
  if (!str) {
    int type = lua_type(L, index);
    fprintf(stderr, "Could not serialize string at index %d (%s found)\n", index, lua_typename(L, type));
    return;
  }
  msgpack_pack_raw(pk, sz);
  msgpack_pack_raw_body(pk, str, sz);
}

static void pack_array(lua_State *L, msgpack_packer *pk, int index, int sz) {
  int i;
  msgpack_pack_array(pk, sz);

  for (i = 1; i <= sz; ++i) {
    lua_rawgeti(L, index, i);
    pack_lua(L, pk, -1);
    lua_pop(L, 1);
  }
}

static void pack_hash(lua_State *L, msgpack_packer *pk, int index) {
  size_t sz = 0;
  // get hash size
  for(lua_pushnil(L); lua_next(L, index) != 0; lua_pop(L, 1)) ++sz;
  msgpack_pack_map(pk, sz);
  // ... <table> ...
  for(lua_pushnil(L); lua_next(L, index) != 0; lua_pop(L, 1)) {
    // push key
    pack_lua(L, pk, -2);
    // push value
    pack_lua(L, pk, -1);
  }
}

static void pack_table(lua_State *L, msgpack_packer *pk, int index) {
  if (index > MAX_DEPTH) {
    // ERROR
    throw dub::Exception("Cannot send table (recursive or too large).");
  }
  int sz = luaL_getn(L, index);

  if (sz > 0) {
    pack_array(L, pk, index, sz);
  } else {
    pack_hash(L, pk, index);
  }
}

static void pack_lua(lua_State *L, msgpack_packer *pk, int index) {
  if (index < 0) {
    // resolve negative index so that it is consistent on recursive table handling
    index = lua_gettop(L) + index + 1;
  }
  int type = lua_type(L, index);
  switch (type) {
  case LUA_TNUMBER:
    msgpack_pack_double(pk, lua_tonumber(L, index));
    return;
  case LUA_TBOOLEAN:
    if (lua_toboolean(L, index)) {
      msgpack_pack_true(pk);
    } else {
      msgpack_pack_false(pk);
    }
    return;
  case LUA_TSTRING:
    pack_string(L, pk, index);
    return;
  case LUA_TNIL:
    msgpack_pack_nil(pk);
    return;
  case LUA_TTABLE:
    return pack_table(L, pk, index);
  case LUA_TUSERDATA:
    // TODO: support serialization (trying __serialize method?)
    // memory leak (msgpack_packer and buffer not freed if we use luaL_error)
  case LUA_TTHREAD:
  case LUA_TLIGHTUSERDATA:
  default:
    throw dub::Exception("Cannot pack message of type %s.", lua_typename(L, type));
  }
}

static int unpack_object(lua_State *L, msgpack_object *o, bool array_as_arglist) {

  switch(o->type) {
	case MSGPACK_OBJECT_NIL:
    lua_pushnil(L);
    return 1;

	case MSGPACK_OBJECT_BOOLEAN:
    lua_pushboolean(L, o->via.boolean);
    return 1;

	case MSGPACK_OBJECT_POSITIVE_INTEGER:
    lua_pushnumber(L, o->via.u64);
    return 1;

	case MSGPACK_OBJECT_NEGATIVE_INTEGER:
    lua_pushnumber(L, o->via.i64);
    return 1;

	case MSGPACK_OBJECT_DOUBLE:
    lua_pushnumber(L, o->via.dec);
    return 1;

	case MSGPACK_OBJECT_RAW:
    lua_pushlstring(L, o->via.raw.ptr, o->via.raw.size);
    return 1;

	case MSGPACK_OBJECT_ARRAY:
	  if (array_as_arglist) {
	    // we do not create a table but simply push values on the stack
      size_t i  = 0;
      size_t sz = o->via.array.size;
      for (i = 0; i < sz; ++i) {
        unpack_object(L, o->via.array.ptr + i, false);
      }
      return sz;
    } else {
      lua_newtable(L);
      size_t tbl_pos = lua_gettop(L);
      size_t i  = 0;
      size_t sz = o->via.array.size;
      for (i = 0; i < sz; ++i) {
        unpack_object(L, o->via.array.ptr + i, false);
        lua_rawseti(L, tbl_pos, i + 1);
      }
      return 1;
    }
	case MSGPACK_OBJECT_MAP:
    {
    lua_newtable(L);
    size_t tbl_pos = lua_gettop(L);
    size_t i  = 0;
    size_t sz = o->via.array.size;
    for (i = 0; i < sz; ++i) {
  	 	msgpack_object_kv *key_value = o->via.map.ptr + i;
      unpack_object(L, &key_value->key, false);
      unpack_object(L, &key_value->val, false);
      lua_rawset(L, tbl_pos);
    }
    return 1;
    }
	default:
    fprintf(stderr, "Unknown type in msg packet (%hu).\n", (unsigned short)o->type);
    return 0;
	}
}

void msgpack_lua_to_bin(lua_State *L, msgpack_sbuffer **buffer, int skip_index) {
  int top = lua_gettop(L);
  /* creates buffer and serializer instance. */
  *buffer = msgpack_sbuffer_new();
  msgpack_sbuffer_init(*buffer);
  msgpack_packer* pk = msgpack_packer_new(*buffer, msgpack_sbuffer_write);

  try {
    // copy from Lua to buffer
    // This is the only copy from Lua to binary data
    int rtop = top - skip_index;
    if (rtop == 0) {
      msgpack_pack_nil(pk);
    } else if (rtop == 1) {
      pack_lua(L, pk, 1 + skip_index);
    } else {
      // multiple objects are sent into an array
      msgpack_pack_array(pk, rtop);
      int i;
      for(i=1; i <= rtop; ++i) {
        pack_lua(L, pk, i + skip_index);
      }
    }
  } catch (dub::Exception e) {
    // Avoid memory leaks.
    msgpack_packer_free(pk);
    msgpack_sbuffer_free(*buffer);
    // Be clean with Lua
    lua_settop(L, top);
    throw;
  }
  msgpack_packer_free(pk);
}

int msgpack_bin_to_lua(lua_State *L, void *data, size_t msg_len) {
  //printf("[msgpack_bin_to_lua] L = %p, msg = %p\n", L, msg);
  // deserialize bin message to Lua.
  msgpack_unpacked values;
  msgpack_unpacked_init(&values);
  // FIXME: allocate zone in some msgpack context and reuse ?
  msgpack_zone zone;
  msgpack_zone_init(&zone, 512);
  msgpack_object obj;

  if (!msgpack_unpack((const char *)data, msg_len, NULL, &zone, &obj)) {
    msgpack_zone_destroy(&zone);
    throw dub::Exception("Could not unpack binary message...");
  }
#if 0
  printf("Receiving..: ");
  msgpack_object_print(stdout, obj);
  printf("\n");
#endif
  // true = array_as_arglist
  int arg_count;
  try {
    arg_count = unpack_object(L, &obj, true);
  } catch (dub::Exception e) {
    msgpack_zone_destroy(&zone);
    throw;
  }

  msgpack_zone_destroy(&zone);

  return arg_count;
}
