--[[------------------------------------------------------

  zmq.Socket
  ----------

  Lets you send and receive lua messages using 0MQ. Since
  a socket should only be used from a single thread, you
  can pass a function that will be executed in a new thread
  if you need to.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.Socket_core'

local worker = worker
local constr = zmq.Socket
local mt = zmq.Socket_

local function checkLoop(self)
  if not self.loop then
    error("Loop not set")
  end
end

function mt:shouldRun()
  checkLoop(self)
  return self.loop:shouldRun()
end

function mt:start()
  checkLoop(self)
  self.loop:start()
end

function mt:quit()
  checkLoop(self)
  self.loop:quit()
end

function mt:kill()
  checkLoop(self)
  self.loop:kill()
end

function mt:join()
  checkLoop(self)
  self.loop:join()
end

function zmq.Socket(type, func)
  local self = constr(type, worker)
  if func then
    self.loop = lk.Thread(function()
      func(self)
    end)
  end
  return self
end
