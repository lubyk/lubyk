-- Copyright (c) 2011 by Robert G. Jakabosky <bobby@sharedrealm.com>
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in
-- all copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
-- THE SOFTWARE.

object "ZMQ_Socket" {
	error_on_null = "get_zmq_strerror()",
	c_source [[
/* detect zmq version >= 2.1.0 */
#define VERSION_2_1 0
#if defined(ZMQ_VERSION)
#if (ZMQ_VERSION >= ZMQ_MAKE_VERSION(2,1,0))
#undef VERSION_2_1
#define VERSION_2_1 1
#endif
#endif

/* detect really old ZeroMQ 2.0.x series. */
#if !defined(ZMQ_RCVMORE)
#error "Your version of ZeroMQ is too old.  Please upgrade to version 2.1 or to the latest 2.0.x"
#endif

typedef struct ZMQ_Socket ZMQ_Socket;

#ifdef _WIN32
#include <winsock2.h>
typedef SOCKET socket_t;
#else
typedef int socket_t;
#endif

/* socket option types. */
#define OPT_TYPE_NONE		0
#define OPT_TYPE_INT		1
#define OPT_TYPE_UINT32	2
#define OPT_TYPE_UINT64	3
#define OPT_TYPE_INT64	4
#define OPT_TYPE_STR		5
#define OPT_TYPE_FD			6

static const int opt_types[] = {
	OPT_TYPE_NONE,		/*  0 unused */
	OPT_TYPE_UINT64,	/*  1 ZMQ_HWM */
	OPT_TYPE_NONE,		/*  2 unused */
	OPT_TYPE_INT64,		/*  3 ZMQ_SWAP */
	OPT_TYPE_UINT64,	/*  4 ZMQ_AFFINITY */
	OPT_TYPE_STR,			/*  5 ZMQ_IDENTITY */
	OPT_TYPE_STR,			/*  6 ZMQ_SUBSCRIBE */
	OPT_TYPE_STR,			/*  7 ZMQ_UNSUBSCRIBE */
	OPT_TYPE_INT64,		/*  8 ZMQ_RATE */
	OPT_TYPE_INT64,		/*  9 ZMQ_RECOVERY_IVL */
	OPT_TYPE_INT64,		/* 10 ZMQ_MCAST_LOOP */
	OPT_TYPE_UINT64,	/* 11 ZMQ_SNDBUF */
	OPT_TYPE_UINT64,	/* 12 ZMQ_RCVBUF */
	OPT_TYPE_INT64,		/* 13 ZMQ_RCVMORE */

#if VERSION_2_1
	OPT_TYPE_FD,			/* 14 ZMQ_FD */
	OPT_TYPE_UINT32,	/* 15 ZMQ_EVENTS */
	OPT_TYPE_INT,			/* 16 ZMQ_TYPE */
	OPT_TYPE_INT,			/* 17 ZMQ_LINGER */
	OPT_TYPE_INT,			/* 18 ZMQ_RECONNECT_IVL */
	OPT_TYPE_INT,			/* 19 ZMQ_BACKLOG */
#endif
};
#if VERSION_2_1
#define MAX_OPTS ZMQ_BACKLOG
#else
#define MAX_OPTS ZMQ_RCVMORE
#endif

]],

	destructor "close" {
		c_method_call "ZMQ_Error"  "zmq_close" {}
	},
	method "bind" {
		c_method_call "ZMQ_Error"  "zmq_bind" { "const char *", "addr" }
	},
	method "connect" {
		c_method_call "ZMQ_Error"  "zmq_connect" { "const char *", "addr" }
	},
	ffi_cdef[[
int zmq_setsockopt (void *s, int option, const void *optval, size_t optvallen);
int zmq_getsockopt (void *s, int option, void *optval, size_t *optvallen);
]],
	ffi_source[[
local option_types = {
[zmq.HWM] = 'uint64_t[1]',
[zmq.SWAP] = 'int64_t[1]',
[zmq.AFFINITY] = 'uint64_t[1]',
[zmq.IDENTITY] = 'string',
[zmq.SUBSCRIBE] = 'string',
[zmq.UNSUBSCRIBE] = 'string',
[zmq.RATE] = 'int64_t[1]',
[zmq.RECOVERY_IVL] = 'int64_t[1]',
[zmq.MCAST_LOOP] = 'int64_t[1]',
[zmq.SNDBUF] = 'uint64_t[1]',
[zmq.RCVBUF] = 'uint64_t[1]',
[zmq.RCVMORE] = 'int64_t[1]',
[zmq.FD] = 'int[1]',
[zmq.EVENTS] = 'uint32_t[1]',
[zmq.TYPE] = 'int[1]',
[zmq.LINGER] = 'int[1]',
[zmq.RECONNECT_IVL] = 'int[1]',
[zmq.BACKLOG] = 'int[1]',
}
local option_len = {}
local option_tmps = {}
for k,v in pairs(option_types) do
  if v ~= 'string' then
    option_len[k] = ffi.sizeof(v)
    option_tmps[k] = ffi.new(v, 0)
  end
end

]],
	method "setopt" {
		var_in{ "uint32_t", "opt" },
		var_in{ "<any>", "val" },
		var_out{ "ZMQ_Error", "err" },
		c_source[[
	size_t val_len;
	const void *val;

#if VERSION_2_1
	socket_t fd_val;
#endif
	int int_val;
	uint32_t uint32_val;
	uint64_t uint64_val;
	int64_t int64_val;

	if(${opt} > MAX_OPTS) {
		return luaL_argerror(L, ${opt::idx}, "Invalid socket option.");
	}

	switch(opt_types[${opt}]) {
#if VERSION_2_1
	case OPT_TYPE_FD:
		fd_val = luaL_checklong(L, ${val::idx});
		val = &fd_val;
		val_len = sizeof(fd_val);
		break;
#endif
	case OPT_TYPE_INT:
		int_val = luaL_checklong(L, ${val::idx});
		val = &int_val;
		val_len = sizeof(int_val);
		break;
	case OPT_TYPE_UINT32:
		uint32_val = luaL_checklong(L, ${val::idx});
		val = &uint32_val;
		val_len = sizeof(uint32_val);
		break;
	case OPT_TYPE_UINT64:
		uint64_val = luaL_checklong(L, ${val::idx});
		val = &uint64_val;
		val_len = sizeof(uint64_val);
		break;
	case OPT_TYPE_INT64:
		int64_val = luaL_checklong(L, ${val::idx});
		val = &int64_val;
		val_len = sizeof(int64_val);
		break;
	case OPT_TYPE_STR:
		val = luaL_checklstring(L, ${val::idx}, &(val_len));
		break;
	default:
		printf("Invalid socket option type, this shouldn't happen.\n");
		abort();
		break;
	}
	${err} = zmq_setsockopt(${this}, ${opt}, val, val_len);
]],
		ffi_source[[
	local ctype = option_types[${opt}]
	local tval
	local tval_len = 0
	if ctype == 'string' then
		tval = tostring(${val})
		tval_len = #${val}
	else
		tval = option_tmps[${opt}]
		tval[0] = ${val}
		tval_len = option_len[${opt}]
	end
	${err} = C.zmq_setsockopt(${this}, ${opt}, tval, tval_len)
]],
	},
		ffi_source[[
local tmp_val_len = ffi.new('size_t[1]', 4)
]],
	method "getopt" {
		var_in{ "uint32_t", "opt" },
		var_out{ "<any>", "val" },
		var_out{ "ZMQ_Error", "err" },
		c_source[[
	size_t val_len;

#if VERSION_2_1
	socket_t fd_val;
#endif
	int int_val;
	uint32_t uint32_val;
	uint64_t uint64_val;
	int64_t int64_val;
#define STR_MAX 255
	char str_val[STR_MAX];

	if(${opt} > MAX_OPTS) {
		lua_pushnil(L);
		lua_pushliteral(L, "Invalid socket option.");
		return 2;
	}

	switch(opt_types[${opt}]) {
#if VERSION_2_1
	case OPT_TYPE_FD:
		val_len = sizeof(fd_val);
		${err} = zmq_getsockopt(${this}, ${opt}, &fd_val, &val_len);
		if(0 == ${err}) {
			lua_pushinteger(L, (lua_Integer)fd_val);
			return 1;
		}
		break;
#endif
	case OPT_TYPE_INT:
		val_len = sizeof(int_val);
		${err} = zmq_getsockopt(${this}, ${opt}, &int_val, &val_len);
		if(0 == ${err}) {
			lua_pushinteger(L, (lua_Integer)int_val);
			return 1;
		}
		break;
	case OPT_TYPE_UINT32:
		val_len = sizeof(uint32_val);
		${err} = zmq_getsockopt(${this}, ${opt}, &uint32_val, &val_len);
		if(0 == ${err}) {
			lua_pushinteger(L, (lua_Integer)uint32_val);
			return 1;
		}
		break;
	case OPT_TYPE_UINT64:
		val_len = sizeof(uint64_val);
		${err} = zmq_getsockopt(${this}, ${opt}, &uint64_val, &val_len);
		if(0 == ${err}) {
			lua_pushinteger(L, (lua_Integer)uint64_val);
			return 1;
		}
		break;
	case OPT_TYPE_INT64:
		val_len = sizeof(int64_val);
		${err} = zmq_getsockopt(${this}, ${opt}, &int64_val, &val_len);
		if(0 == ${err}) {
			lua_pushinteger(L, (lua_Integer)int64_val);
			return 1;
		}
		break;
	case OPT_TYPE_STR:
		val_len = STR_MAX;
		${err} = zmq_getsockopt(${this}, ${opt}, str_val, &val_len);
		if(0 == ${err}) {
			lua_pushlstring(L, str_val, val_len);
			return 1;
		}
#undef STR_MAX
		break;
	default:
		printf("Invalid socket option type, this shouldn't happen.\n");
		abort();
		break;
	}
	lua_pushnil(L);
]],
		ffi_source[[
	local ctype = option_types[${opt}]
	local val
	local val_len = tmp_val_len
	if ctype == 'string' then
		val_len[0] = 255
		val = ffi.new('uint8_t[?]', val_len[0])
		ffi.fill(val, val_len[0])
	else
		val = option_tmps[${opt}]
		val[0] = 0
		val_len[0] = option_len[${opt}]
	end
	${err} = C.zmq_getsockopt(${this}, ${opt}, val, val_len)
	if ${err} == 0 then
		if ctype == 'string' then
			val_len = val_len[0]
			return ffi.string(val, val_len)
		else
			return tonumber(val[0])
		end
	end
]],
	},
	ffi_source[[
-- temp. values for 'events' function.
local events_tmp = ffi.new('uint32_t[1]', 0)
local events_tmp_size = ffi.sizeof('uint32_t')
local events_tmp_len = ffi.new('size_t[1]', events_tmp_size)
local ZMQ_EVENTS = _M.EVENTS
]],
	method "events" {
		var_out{ "uint32_t", "events" },
		var_out{ "ZMQ_Error", "err" },
		c_source[[
#if VERSION_2_1
	size_t val_len = sizeof(${events});
	${err} = zmq_getsockopt(${this}, ZMQ_EVENTS, &(${events}), &val_len);
#else
	luaL_error(L, "'events' method only supported in 0MQ version >= 2.1");
#endif
]],
		ffi_source[[
	events_tmp_len[0] = events_tmp_size
	${err} = C.zmq_getsockopt(${this}, ZMQ_EVENTS, events_tmp, events_tmp_len);
	${events} = events_tmp[0]
]],
	},
	--
	-- zmq_send
	--
	method "send_msg" {
		c_method_call "ZMQ_Error" "zmq_send" { "zmq_msg_t *", "msg", "int", "flags?" },
	},
	-- create helper function for `zmq_send`
	c_source[[
static ZMQ_Error simple_zmq_send(ZMQ_Socket *sock, const char *data, size_t data_len, int flags) {
	ZMQ_Error err;
	zmq_msg_t msg;
	/* initialize message */
	err = zmq_msg_init_size(&msg, data_len);
	if(0 == err) {
		/* fill message */
		memcpy(zmq_msg_data(&msg), data, data_len);
		/* send message */
		err = zmq_send(sock, &msg, flags);
		/* close message */
		zmq_msg_close(&msg);
	}
	return err;
}
]],
	-- export helper function.
	ffi_export_function "ZMQ_Error" "simple_zmq_send"
		"(ZMQ_Socket *sock, const char *data, size_t data_len, int flags)",
	method "send" {
		var_in{ "const char *", "data" },
		var_in{ "int", "flags?" },
		var_out{ "ZMQ_Error", "err" },
		c_source[[
	${err} = simple_zmq_send(${this}, ${data}, ${data_len}, ${flags});
]],
		ffi_source[[
	${err} = simple_zmq_send(${this}, ${data}, ${data_len}, ${flags});
]],
	},
	--
	-- zmq_recv
	--
	method "recv_msg" {
		c_method_call "ZMQ_Error" "zmq_recv" { "zmq_msg_t *", "msg", "int", "flags?" },
	},
	ffi_source[[
local tmp_msg = ffi.new('zmq_msg_t')
]],
	method "recv" {
		var_in{ "int", "flags?" },
		var_out{ "const char *", "data", has_length = true },
		var_out{ "ZMQ_Error", "err" },
		c_source[[
	zmq_msg_t msg;
	/* initialize message */
	${err} = zmq_msg_init(&msg);
	if(0 == ${err}) {
		/* receive message */
		${err} = zmq_recv(${this}, &msg, ${flags});
		if(0 == ${err}) {
			${data} = zmq_msg_data(&msg);
			${data_len} = zmq_msg_size(&msg);
		}
	}
]],
		c_source "post" [[
	/* close message */
	zmq_msg_close(&msg);
]],
		ffi_source[[
	local msg = tmp_msg
	-- initialize blank message.
	if C.zmq_msg_init(msg) < 0 then
		return nil, get_zmq_strerror()
	end

	-- receive message
	${err} = C.zmq_recv(${this}, msg, ${flags})
	if 0 == ${err} then
		local data = ffi.string(C.zmq_msg_data(msg), C.zmq_msg_size(msg))
		-- close message
		C.zmq_msg_close(msg)
		return data
	end
]],
		ffi_source "ffi_post" [[
	-- close message
	C.zmq_msg_close(msg)
]],
	},
}

