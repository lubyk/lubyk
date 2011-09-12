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

local ZMQ_Poller_type = [[
struct ZMQ_Poller {
	zmq_pollitem_t *items;
	int    next;
	int    count;
	int    free_list;
	int    len;
};
]]

object "ZMQ_Poller" {
	-- store the `ZMQ_Poller` structure in Lua userdata object
	userdata_type = "embed",
	c_source(ZMQ_Poller_type),
	c_source[[

typedef struct ZMQ_Poller ZMQ_Poller;

#define FREE_ITEM_EVENTS_TAG ((short)0xFFFF)

#define ITEM_TO_INDEX(items, item) (item - (items))

static int poller_resize_items(ZMQ_Poller *poller, int len) {
	int old_len = poller->len;

	/* make sure new length is atleast as large as items count. */
	len = (poller->count <= len) ? len : poller->count;

	/* if the new length is the same as the old length, then don't try to resize. */
	if(old_len == len) return len;

	poller->items = (zmq_pollitem_t *)realloc(poller->items, len * sizeof(zmq_pollitem_t));
	poller->len = len;
	if(len > old_len) {
		/* clear new space. */
		memset(&(poller->items[old_len]), 0, (len - old_len) * sizeof(zmq_pollitem_t));
	}
	return len;
}

static int poller_find_sock_item(ZMQ_Poller *poller, ZMQ_Socket *sock) {
	zmq_pollitem_t *items;
	int count;
	int n;

	/* find ZMQ_Socket */
	items = poller->items;
	count = poller->count;
	for(n=0; n < count; n++) {
		if(items[n].socket == sock) return n;
	}
	/* not found. */
	return -1;
}

static int poller_find_fd_item(ZMQ_Poller *poller, socket_t fd) {
	zmq_pollitem_t *items;
	int count;
	int n;

	/* find fd */
	items = poller->items;
	count = poller->count;
	for(n=0; n < count; n++) {
		if(items[n].fd == fd) return n;
	}
	/* not found. */
	return -1;
}

static void poller_remove_item(ZMQ_Poller *poller, int idx) {
	zmq_pollitem_t *items;
	int free_list;
	int count;

	count = poller->count;
	/* no item to remove. */
	if(idx >= count || count == 0) return;

	items = poller->items;
	free_list = poller->free_list;

	/* link new free slot to head of free list. */
	if(free_list >= 0 && free_list < count) {
		/* use socket pointer for free list's 'next' field. */
		items[idx].socket = &(items[free_list]);
	} else {
		/* free list is empty mark poller slot as the end. */
		items[idx].socket = NULL;
	}
	poller->free_list = idx;
	/* mark poller slot as a free slot. */
	items[idx].events = FREE_ITEM_EVENTS_TAG;
	/* clear old revents. */
	items[idx].revents = 0;
}

static int poller_get_free_item(ZMQ_Poller *poller) {
	zmq_pollitem_t *curr;
	zmq_pollitem_t *next;
	int count;
	int idx;

	count = poller->count;
	idx = poller->free_list;
	/* check for a free slot in the free list. */
	if(idx >= 0 && idx < count) {
		/* remove free slot from free list. */
		curr = &(poller->items[idx]);
		/* valid free slot. */
		assert(curr->events == FREE_ITEM_EVENTS_TAG);
		/* is poller the last free slot? */
		next = ((zmq_pollitem_t *)curr->socket);
		if(next != NULL) {
			/* set next free slot as head of free list. */
			poller->free_list = ITEM_TO_INDEX(poller->items, next);
		} else {
			/* free list is empty now. */
			poller->free_list = -1;
		}
		/* clear slot */
		memset(curr, 0, sizeof(zmq_pollitem_t));
		return idx;
	}

	idx = count;
	poller->count = ++count;
	/* make room for new item. */
	if(count >= poller->len) {
		poller_resize_items(poller, poller->len + 10);
	}
	return idx;
}

static int poller_compact_items(ZMQ_Poller *poller) {
	zmq_pollitem_t *items;
	int count;
	int old_count;
	int next;

	count = poller->count;
	/* if no free slot, then return. */
	if(poller->free_list < 0) return count;
	old_count = count;

	items = poller->items;
	next = 0;
	/* find first free slot. */
	while(next < count && items[next].events != FREE_ITEM_EVENTS_TAG) {
		++next;
	}

	/* move non-free slots into free slot. */
	count = next;
	++next;
	while(next < old_count) {
		if(items[next].events != FREE_ITEM_EVENTS_TAG) {
			/* found non-free slot, move it to the current free slot. */
			items[count] = items[next];
			++count;
		}
		++next;
	}

	/* clear old used-space */
	memset(&(items[count]), 0, ((old_count - count) * sizeof(zmq_pollitem_t)));
	poller->count = count;
	poller->free_list = -1; /* free list is now empty. */

	assert(count <= poller->len);
	return count;
}

static int poller_poll(ZMQ_Poller *poller, long timeout) {
	int count;
	/* remove free slots from items list. */
	count = poller_compact_items(poller);
	/* poll for events. */
	return zmq_poll(poller->items, count, timeout);
}

]],
--
-- Define ZMQ_Poller type & function API for FFI
--
	ffi_cdef[[
typedef int socket_t;
typedef struct zmq_pollitem_t {
	ZMQ_Socket *socket;
	socket_t fd;
	short events;
	short revents;
} zmq_pollitem_t;

int zmq_poll(zmq_pollitem_t *items, int nitems, long timeout);
]],
	ffi_cdef(ZMQ_Poller_type),
	ffi_export_function "int" "poller_find_sock_item" "(ZMQ_Poller *poller, ZMQ_Socket *sock)",
	ffi_export_function "int" "poller_find_fd_item" "(ZMQ_Poller *poller, socket_t fd)",
	ffi_export_function "int" "poller_get_free_item" "(ZMQ_Poller *poller)",
	ffi_export_function "int" "poller_poll" "(ZMQ_Poller *poller, long timeout)",
	ffi_export_function "void" "poller_remove_item" "(ZMQ_Poller *poller, int idx)",

	constructor "new" {
		var_in{ "unsigned int", "length", is_optional = true, default = 10 },
		c_source[[
	ZMQ_Poller poller;
	${this} = &poller;
	${this}->items = (zmq_pollitem_t *)calloc(${length}, sizeof(zmq_pollitem_t));
	${this}->next = -1;
	${this}->count = 0;
	${this}->len = ${length};
	${this}->free_list = -1;
]],
	},
	destructor "close" {
		c_source[[
	free(${this}->items);
	${this}->items = NULL;
	${this}->next = -1;
	${this}->count = 0;
	${this}->len = 0;
	${this}->free_list = -1;
]],
	},
	method "add" {
		var_in{ "<any>", "sock" },
		var_in{ "short", "events" },
		var_out{ "int", "idx" },
		c_source "pre" [[
	zmq_pollitem_t *item;
	ZMQ_Socket *sock = NULL;
	socket_t fd = 0;
]],
		c_source[[
	if(lua_isuserdata(L, ${sock::idx})) {
		sock = obj_type_ZMQ_Socket_check(L, ${sock::idx});
	} else if(lua_isnumber(L, ${sock::idx})) {
		fd = lua_tonumber(L, ${sock::idx});
	} else {
		return luaL_typerror(L, ${sock::idx}, "number or ZMQ_Socket");
	}
	${idx} = poller_get_free_item(${this});
	item = &(${this}->items[${idx}]);
	item->socket = sock;
	item->fd = fd;
	item->events = ${events};
]],
	},
	method "modify" {
		var_in{ "<any>", "sock" },
		var_in{ "short", "events" },
		var_out{ "int", "idx" },
		c_source "pre" [[
	zmq_pollitem_t *item;
	ZMQ_Socket *sock = NULL;
	socket_t fd = 0;
]],
		c_source[[
	if(lua_isuserdata(L, ${sock::idx})) {
		sock = obj_type_ZMQ_Socket_check(L, ${sock::idx});
		/* find sock in items list. */
		${idx} = poller_find_sock_item(${this}, sock);
	} else if(lua_isnumber(L, ${sock::idx})) {
		fd = lua_tonumber(L, ${sock::idx});
		/* find fd in items list. */
		${idx} = poller_find_fd_item(${this}, fd);
	} else {
		return luaL_typerror(L, ${sock::idx}, "number or ZMQ_Socket");
	}
	if(${events} != 0) {
		/* add/modify. */
		if(${idx} < 0) {
			${idx} = poller_get_free_item(${this});
		}
		item = &(${this}->items[${idx}]);
		item->socket = sock;
		item->fd = fd;
		item->events = ${events};
	} else if(${idx} >= 0) {
		/* no events remove socket/fd. */
		poller_remove_item(${this}, ${idx});
	}
]],
	},
	method "remove" {
		var_in{ "<any>", "sock" },
		c_source "pre" [[
	ZMQ_Socket *sock;
	socket_t fd;
	int idx;
]],
		c_source[[
	/* ZMQ_Socket or fd */
	if(lua_isuserdata(L, ${sock::idx})) {
		sock = obj_type_ZMQ_Socket_check(L, ${sock::idx});
		/* find sock in items list. */
		idx = poller_find_sock_item(${this}, sock);
	} else if(lua_isnumber(L, ${sock::idx})) {
		fd = lua_tonumber(L, ${sock::idx});
		/* find fd in items list. */
		idx = poller_find_fd_item(${this}, fd);
	} else {
		return luaL_typerror(L, ${sock::idx}, "number or ZMQ_Socket");
	}
	/* if sock/fd was found. */
	if(idx >= 0) {
		poller_remove_item(${this}, idx);
	}
]],
	},
	method "poll" {
		var_in{ "long", "timeout" },
		var_out{ "ZMQ_Error", "err" },
		c_source[[
	/* poll for events */
	${err} = poller_poll(${this}, ${timeout});
	if(${err} > 0) {
		${this}->next = 0;
	} else {
		${this}->next = -1;
	}
]],
		ffi_source[[
	-- poll for events
	${err} = poller_poll(${this}, ${timeout})
	if(${err} > 0) then
		${this}.next = 0
	else
		${this}.next = -1
	end
]],
	},
	method "next_revents" {
		var_out{ "<any>", "sock" },
		var_out{ "short", "revents" },
		c_source "pre" [[
	zmq_pollitem_t *items;
	int count;
	int idx;
]],
		c_source[[
	${revents} = -1;
	idx = ${this}->next;
	if(idx >= 0) {
		count = ${this}->count;
		items = ${this}->items;
		/* find next item with pending events. */
		while(idx < count && items[idx].revents == 0) ++idx;
		/* did we find a pending event? */
		if(idx < count) {
			/* push the event's sock/fd. */
			if(items[idx].socket != NULL) {
				obj_type_ZMQ_Socket_push(L, items[idx].socket, 0);
			} else {
				lua_pushnumber(L, items[idx].fd);
			}
			${revents} = items[idx].revents;
			/* is this the last event. */
			++idx;
			${this}->next = (idx < count) ? idx : -1;
		}
	}
	if(${revents} < 0) {
		/* no more pending events. */
		lua_pushnil(L);
		${this}->next = -1;
	}
]],
		ffi_source[[
	local sock
	local idx = ${this}.next
	if (idx < 0) then return nil, -1 end
	local count = ${this}.count
	-- find next item with pending events.
	while (idx < count and ${this}.items[idx].revents == 0) do
		idx = idx + 1
		if (idx >= count) then
			idx = -1
			break
		end
	end
	-- did we find a pending event?
	if(idx >= 0) then
		-- push the event's sock/fd.
		if(${this}.items[idx].socket ~= nil) then
			sock = obj_type_ZMQ_Socket_push(${this}.items[idx].socket, 0)
		else
			sock = tonumber(${this}.items[idx].fd)
		end
		${revents} = ${this}.items[idx].revents
		-- is this the last event.
		idx = idx + 1
		if (idx >= count) then
			idx = -1
		end
		${this}.next = idx
		return sock, ${revents}
	end
	${this}.next = idx
]],
	},
	method "count" {
		var_out{ "int", "count" },
		c_source[[
	${count} = ${this}->count;
]],
		ffi_source[[
	${count} = ${this}.count;
]],
	},
}

