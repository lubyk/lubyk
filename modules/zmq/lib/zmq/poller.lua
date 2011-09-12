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

--
-- zmq.poller wraps the low-level zmq.ZMQ_Poller object.
--
-- This wrapper simplifies the event polling loop.
--

local zmq = require"zmq"

local setmetatable = setmetatable
local tonumber = tonumber
local assert = assert

local poller_mt = {}
poller_mt.__index = poller_mt

function poller_mt:add(sock, events, cb)
	self.poller:add(sock, events)
	self.callbacks[sock] = cb
end

function poller_mt:modify(sock, events, cb)
	if events ~= 0 and cb then
		self.callbacks[sock] = cb
		self.poller:modify(sock, events)
	else
		self:remove(sock)
	end
end

function poller_mt:remove(sock)
	self.poller:remove(sock)
	self.callbacks[sock] = nil
end

function poller_mt:poll(timeout)
	local poller = self.poller
	local status, err = poller:poll(timeout)
	if not status then
		return nil, err
	end
	local callbacks = self.callbacks
	local count = 0
	while true do
		local sock, revents = poller:next_revents()
		if not sock then
			break
		end
		local cb = callbacks[sock]
		if not cb then
			error("Missing callback for sock:" .. tostring(sock))
		end
		cb(sock, revents)
		count = count + 1
	end
	return count
end

function poller_mt:start()
	self.is_running = true
	while self.is_running do
		local status, err = self:poll(-1)
		if not status then
			return false, err
		end
	end
	return true
end

function poller_mt:stop()
	self.is_running = false
end

module(...)

function new(pre_alloc)
	return setmetatable({
		poller = zmq.ZMQ_Poller(pre_alloc),
		callbacks = setmetatable({}, {__mode="k"}),
	}, poller_mt)
end

setmetatable(_M, {__call = function(tab, ...) return new(...) end})

