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


#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "zmq/vendor/include/zmq.h"
#include "msgpack.h"

#include <string.h>
#include <stdio.h>

static void pack_lua(lua_State *L, msgpack_packer *pk, int index);

static void free_msgpack_msg(void *data, void *buffer) {
  msgpack_sbuffer_free((msgpack_sbuffer*)buffer);
}

inline void pack_string(lua_State *L, msgpack_packer *pk, int index) {
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

inline void pack_array(lua_State *L, msgpack_packer *pk, int index, size_t sz) {
  msgpack_pack_array(pk, sz);

  size_t i;
  for (i = 1; i <= sz; ++i) {
    lua_rawgeti(L, index, i);
    pack_lua(L, pk, -1);
    lua_pop(L, 1);
  }
}

inline void pack_hash(lua_State *L, msgpack_packer *pk, int index) {
  size_t sz = 0;
  // get hash size
  for(lua_pushnil(L); lua_next(L, index) != 0; lua_pop(L, 1)) ++sz;

  msgpack_pack_map(pk, sz);

  for(lua_pushnil(L); lua_next(L, index) != 0; lua_pop(L, 1)) {
    // push key
    pack_lua(L, pk, -2);
    // push value
    pack_lua(L, pk, -1);
  }
}

inline void pack_table(lua_State *L, msgpack_packer *pk, int index) {
  size_t sz = lua_objlen(L, index);
  if (sz > 0) {
    pack_array(L, pk, index, sz);
  } else {
    pack_hash(L, pk, index);
  }
}

static void pack_lua(lua_State *L, msgpack_packer *pk, int index) {
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
    pack_table(L, pk, index);
    return;
  case LUA_TUSERDATA:
    // TODO: support serialization (trying __serialize method?)
    // memory leak (msgpack_packer and buffer not freed if we use luaL_error)
  case LUA_TTHREAD:
  case LUA_TLIGHTUSERDATA:
  default:
    fprintf(stderr, "Cannot pack message of type %s.", lua_typename(L, type));
    break;
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
	default:
    fprintf(stderr, "Unknown type in msg packet (%hu).\n", o->type);
    return 0;
	}
}

void msgpack_lua_to_zmq(lua_State *L, zmq_msg_t *msg, int skip_index) {
  /* creates buffer and serializer instance. */
  msgpack_sbuffer* buffer = msgpack_sbuffer_new();
  msgpack_sbuffer_init(buffer);

  msgpack_packer*  pk     = msgpack_packer_new(buffer, msgpack_sbuffer_write);

  // copy from Lua to buffer
  // This is the only copy from Lua to zmq
  int top = lua_gettop(L) - skip_index;
  if (top == 0) {
    msgpack_pack_nil(pk);
  } else if (top == 1) {
    pack_lua(L, pk, 1 + skip_index);
  } else {
    // multiple objects are sent into an array
    msgpack_pack_array(pk, top);
    int i;
    for(i=1; i <= top; ++i) {
      pack_lua(L, pk, i + skip_index);
    }
  }

  zmq_msg_init_data(msg, buffer->data, buffer->size, free_msgpack_msg, buffer);
  msgpack_packer_free(pk);
}

int msgpack_zmq_to_lua(lua_State *L, zmq_msg_t *msg) {
  // deserialize zmq message to Lua.
  msgpack_unpacked values;
  msgpack_unpacked_init(&values);
  // FIXME: allocate zone in worker_ and reuse ?
  msgpack_zone zone;
  msgpack_zone_init(&zone, 512);
  msgpack_object obj;

  if (!msgpack_unpack(zmq_msg_data(msg), zmq_msg_size(msg), NULL, &zone, &obj)) {
    luaL_error(L, "Could not unpack zmq message...");
    return 0;
  }
#if 0
  printf("Receiving..: ");
  msgpack_object_print(stdout, obj);
  printf("\n");
#endif
  // true = array_as_arglist
  int arg_count = unpack_object(L, &obj, true);

  msgpack_zone_destroy(&zone);

  return arg_count;
}
//
//int main(void) {
//    {
//    /* creates buffer and serializer instance. */
//    msgpack_sbuffer* buffer = msgpack_sbuffer_new();
//    msgpack_packer* pk = msgpack_packer_new(buffer, msgpack_sbuffer_write);
//
//    /* serializes ["Hello", "MessagePack"]. */
//    msgpack_pack_array(pk, 2);
//    msgpack_pack_raw(pk, 5);
//    msgpack_pack_raw_body(pk, "Hello", 5);
//    msgpack_pack_raw(pk, 11);
//    msgpack_pack_raw_body(pk, "MessagePack", 11);
//
//    /* deserializes it. */
//    msgpack_unpacked msg;
//    msgpack_unpacked_init(&msg);
//    bool success = msgpack_unpack_next(&msg, buffer->data, buffer->size, NULL);
//
//    /* prints the deserialized object. */
//    msgpack_object obj = msg.data;
//    msgpack_object_print(stdout, obj);  /*=> ["Hello", "MessagePack"] */
//
//    /* cleaning */
//    msgpack_sbuffer_free(buffer);
//    msgpack_packer_free(pk);
//    }
//
//}

//int msgpack_unpack_to_lua(lua_State *L) {
//  msgpack_unpacked msg;
//  msgpack_unpacked_init(&msg);
//  bool success = msgpack_unpack_next(&msg, buffer->data, buffer->size, NULL);
//  switch (msg.type) {
//  case type::NIL:
//    lua_pushnil(L);
//    break;
//
//  case type::BOOLEAN:
//    lua_pushboolean(L, msg.via.boolean);
//    break;
//
//    // Lua internally uses double to represent integer. In addition,
//    // lua_Integer is the alias of ptrdiff_t, which can be 32 bits.
//    // Therefore, pushing (u)int64_t as double is the best way.
//  case type::POSITIVE_INTEGER:
//    lua_pushnumber(L, msg.via.u64);
//    break;
//
//  case type::NEGATIVE_INTEGER:
//    lua_pushnumber(L, msg.via.i64);
//    break;
//
//  case type::DOUBLE:
//    lua_pushnumber(L, msg.via.dec);
//    break;
//
//  case type::RAW:
//    lua_pushlstring(L, msg.via.raw.ptr, msg.via.raw.size);
//    break;
//
//  case type::ARRAY:
//    unpackArray(msg.via.array);
//    break;
//
//  case type::MAP:
//    unpackTable(msg.via.map);
//    break;
//
//  default:
//    luaL_error(L, "invalid type for unpack: %d", msg.type);
//    return;
//  }
//}
//}

// Register namespace
static const struct luaL_Reg lib_functions[] = {
  {NULL, NULL},
};

int luaopen_msgpack_vendor(lua_State *L) {
  // register functions
  //luaL_register(L, "msgpack", lib_functions);
  return 0;
}
