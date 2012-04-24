--[[------------------------------------------------------

  zmq.Socket
  ----------

  Lets you send and receive lua messages using 0MQ. Since
  a socket should only be used from a single thread, you
  can pass a function that will be executed in a new thread
  if you need to.

--]]------------------------------------------------------
local lib  = zmq.Socket_core
zmq.Socket = lib
local ctx = zmq.Context()

local new = lib.new
function lib.new(type, func)
  local self = new(type, ctx)
  self.loop = func
  --- Only keep 500 messages in the send queue per subscriber (this avoids
  -- memory exhaustion when receiver is slow). By default, sending messages
  -- blocks (only the sending coroutine) until we can send again.
  -- HWM = AVAIL MEMORY / SUBSCRIBERS / MSG_SIZE = nb messages per subscriber
  self:setsockopt(zmq.HWM, 500)

  self:setNonBlocking(true)

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
  -- In case an unprotected error occurs in the thread, automatically
  -- restart:
  self.thread.restart = true
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

local send = lib.send
function lib:send(...)
  local super = self.super
  if not send(super, ...) then
    -- could not send
    sched:waitWrite(super)
    -- should be ok now
    send(super, ...)
  end
end

function lib:request(...)
  self:send(...)
  return self:recv()
end

local zmq_POLLIN = zmq.POLLIN
local recv = lib.recv
function lib:recv()
  local super = self.super
  if not super:hasEvent(zmq_POLLIN) then
    self.recv_start = nil
    sched:waitRead(super)
  elseif self.recv_start then
    if self.recv_start + 500 < elapsed() then
      -- Force sleep to avoid DoS
      self.recv_start = nil
      sleep(1)
    end
  else
    self.recv_start = elapsed()
  end
  return recv(super)
end
