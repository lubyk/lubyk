--[[------------------------------------------------------

  zmq.Socket
  ----------

  Lets you send and receive lua messages using 0MQ. Since
  a socket should only be used from a single thread, you
  can pass a function that will be executed in a new thread
  if you need to.

--]]------------------------------------------------------
require 'zmq'
local lib = {type = 'zmq.Socket'}
local sched = sched
lib.__index = lib
zmq.Socket  = lib

setmetatable(lib, {
  __call = function(lib, type, func)
  local self = { 
    super = zmq.ctx:socket(type),
    loop  = func,
  }
  setmetatable(self, lib)
  if self.loop then
    self:start()
  end
  return self
end})

function lib:shouldRun()
  -- FIXME: remove and simply check self.thread
  return self.thread
end

function lib:start()
  if not self.loop then
    error('Loop not set')
  end
  self.thread = lk.Thread(function()
    self.loop(self)
    self.thread = nil
  end)
end

function lib:quit()
  if self.thread then
    self.thread:quit()
    self.thread = nil
  end
end

function lib:kill()
  if self.thread then
    self.thread:kill()
  end
end

function lib:join()
  if self.thread then
    self.thread:join()
  end
end

function lib:send(...)
  local super = self.super
  local msg = super:sendMsg(...)
  if msg then
    -- could not send
    sched:waitWrite(super)
    super:send(msg)
  end
end

function lib:request(...)
  self:send(...)
  return self:recv()
end

function lib:connect(...)
  self.super:connect(...)
end

function lib:bind(location)
  if not location then
    self.port_ = self.super:bindAny()
  else
    self.super:bind(location)
    -- how do we get host/port ?
  end
  return self.port_
end

function lib:port()
  return self.port_
end

function lib:setopt(...)
  self.super:setopt(...)
end

function lib:recv()
  local super = self.super
  local msg = {super:recvMsg()}
  if msg[1] == nil then
    sched:waitRead(super)
    return super:recvMsg()
  else
    return unpack(msg)
  end
end
