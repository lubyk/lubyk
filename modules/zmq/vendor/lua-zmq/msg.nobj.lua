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

object "zmq_msg_t" {
	-- store the `zmq_msg_t` structure in Lua userdata object
	userdata_type = "embed",
--
-- Define zmq_msq_t type & function API for FFI
--
	ffi_cdef[[

struct zmq_msg_t
{
	void *content;
	unsigned char flags;
	unsigned char vsm_size;
	unsigned char vsm_data [30]; /* that '30' is from 'MAX_VSM_SIZE' */
};

typedef void (zmq_free_fn) (void *data, void *hint);

int zmq_msg_init (zmq_msg_t *msg);
int zmq_msg_init_size (zmq_msg_t *msg, size_t size);
int zmq_msg_init_data (zmq_msg_t *msg, void *data, size_t size, zmq_free_fn *ffn, void *hint);

]],
	constructor "init" {
		var_out{ "ZMQ_Error", "err" },
		c_source[[
	zmq_msg_t tmp;
	${this} = &tmp;
	${err} = zmq_msg_init(${this});
]],
	},
	constructor "init_size" {
		var_in{ "size_t", "size" },
		var_out{ "ZMQ_Error", "err" },
		c_source[[
	zmq_msg_t tmp;
	${this} = &tmp;
	${err} = zmq_msg_init_size(${this}, ${size});
]],
	},
	constructor "init_data" {
		var_in{ "const char *", "data" },
		var_out{ "ZMQ_Error", "err" },
		c_source[[
	zmq_msg_t tmp;
	${this} = &tmp;
	${err} = zmq_msg_init_size(${this}, ${data_len});
	if(0 == ${err}) {
		/* fill message */
		memcpy(zmq_msg_data(${this}), ${data}, ${data_len});
	}
]],
	},
	destructor {
		c_method_call "ZMQ_Error" "zmq_msg_close" {}
	},
	method "close" {
		c_method_call "ZMQ_Error" "zmq_msg_close" {}
	},
	method "move" {
		c_method_call "ZMQ_Error" "zmq_msg_move" { "zmq_msg_t *", "src" }
	},
	method "copy" {
		c_method_call "ZMQ_Error" "zmq_msg_copy" { "zmq_msg_t *", "src" }
	},
	method "set_data" {
		var_in{ "const char *", "data" },
		var_out{ "ZMQ_Error", "err" },
		c_source[[
	/* check message data size. */
	if(zmq_msg_size(${this}) != ${data_len}) {
		/* need to resize message. */
		zmq_msg_close(${this}); /* close old message, to free old data. */
		${err} = zmq_msg_init_size(${this}, ${data_len}); /* re-initialize message. */
		if(0 != ${err}) {
			luaL_error(L, "set_data() failed: %s", get_zmq_strerror());
		}
	}
	/* copy data into message */
	memcpy(zmq_msg_data(${this}), ${data}, ${data_len});
]],
		ffi_source[[
	-- check message data size.
	if (C.zmq_msg_size(${this}) ~= ${data_len}) then
		-- need to resize message.
		C.zmq_msg_close(${this}); -- close old message, to free old data.
		${err} = C.zmq_msg_init_size(${this}, ${data_len}); -- re-initialize message.
		if (0 ~= ${err}) then
			error("set_data() failed: " .. get_zmq_strerror());
		end
	end
	-- copy data into message
	ffi.copy(C.zmq_msg_data(${this}), ${data}, ${data_len});
]],
	},
	method "data" {
		c_method_call "void *" "zmq_msg_data" {}
	},
	method "set_size" {
		var_in{ "size_t", "size" },
		var_out{ "ZMQ_Error", "err" },
		c_source[[
	/* check message data size. */
	if(zmq_msg_size(${this}) != ${size}) {
		/* need to resize message. */
		zmq_msg_close(${this}); /* close old message, to free old data. */
		${err} = zmq_msg_init_size(${this}, ${size}); /* re-initialize message. */
		if(0 != ${err}) {
			luaL_error(L, "set_size() failed: %s", get_zmq_strerror());
		}
	}
]],
		ffi_source[[
	-- check message data size.
	if (C.zmq_msg_size(${this}) ~= ${size}) then
		-- need to resize message.
		C.zmq_msg_close(${this}); -- close old message, to free old data.
		${err} = C.zmq_msg_init_size(${this}, ${size}); -- re-initialize message.
		if (0 ~= ${err}) then
			error("set_size() failed: " .. get_zmq_strerror());
		end
	end
]],
	},
	method "size" {
		c_method_call "size_t" "zmq_msg_size" {}
	},
	method "__tostring" {
		var_out{ "const char *", "data", has_length = true },
		c_source[[
	${data} = zmq_msg_data(${this});
	${data_len} = zmq_msg_size(${this});
]],
		ffi_source[[
	${data} = C.zmq_msg_data(${this});
	${data_len} = C.zmq_msg_size(${this});
]],
	},
}

