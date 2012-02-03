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
  self.loop = func
  --- Only keep 500 messages in the send queue per subscriber (this avoids
  -- memory exhaustion when receiver is slow). By default, sending messages
  -- blocks (only the sending coroutine) until we can send again.
  -- HWM = AVAIL MEMORY / SUBSCRIBERS / MSG_SIZE = nb messages per subscriber
  self:setsockopt(zmq.HWM, 500)

  -- FIXME: ZMQ_NOBLOCK hangs ???
  self:setNonBlocking(true)

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
  if not super:send(...) then
    -- could not send
    sched:waitWrite(super)
    -- should be ok now
    super:send(...)
  end
end

function lib:request(...)
  self:send(...)
  return self:recv()
end

local zmq_POLLIN = zmq.POLLIN
function lib:recv()
  local super = self.super
  if not super:hasEvent(zmq_POLLIN) then
    self.recv_start = nil
    sched:waitRead(super)
  elseif self.recv_start then
    if self.recv_start + 500 < worker:now() then
      -- Force sleep to avoid DoS
      self.recv_start = nil
      sleep(1)
    end
  else
    self.recv_start = worker:now()
  end
  return super:recv()
end
