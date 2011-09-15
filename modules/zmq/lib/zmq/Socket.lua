--[[------------------------------------------------------

  zmq.Socket
  ----------

  Lets you send and receive lua messages using 0MQ. Since
  a socket should only be used from a single thread, you
  can pass a function that will be executed in a new thread
  if you need to.

--]]------------------------------------------------------
require 'zmq.Socket_core'
local constr = zmq.Socket
local lib  = lk.SubClass(zmq, 'Socket')
local worker = worker
zmq.Socket = lib

lib.type = 'zmq.Socket'

function lib.new(type, func)
  local self = constr(type, worker)
  self.fd = self:rawSock()
  self.loop = func
  setmetatable(self, lib)
  if self.loop then
    self:start()
  end
  return self
end

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
  local msg = super:send(...)
  if msg then
    -- could not send
    sched:waitWrite(super)
    super:rawSend(msg)
  end
end

function lib:request(...)
  self:send(...)
  return self:recv()
end

function lib:recv()
  sched:waitRead(self.super)
  return self.super:recv()
end
