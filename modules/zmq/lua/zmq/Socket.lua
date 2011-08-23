--[[------------------------------------------------------

  zmq.Socket
  ----------

  Lets you send and receive lua messages using 0MQ. Since
  a socket should only be used from a single thread, you
  can pass a function that will be executed in a new thread
  if you need to.

--]]------------------------------------------------------
require 'zmq'
-- Do we need this require ?
require 'zmq.Socket_core'
local worker = worker
local constr = zmq.Socket
function zmq.Socket(type, func)
  local self = constr(type, worker)
  if func then
    self.run = func
    self:start()
  end
  return self
end
