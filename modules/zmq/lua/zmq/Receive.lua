--[[------------------------------------------------------

  zmq.Receive
  -----------

  Lets you receive lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq.Receive_core'
require 'worker'

local constr = zmq.Receive
local worker = worker
function zmq.Receive(location, func)
  return constr(worker, location, func)
end
