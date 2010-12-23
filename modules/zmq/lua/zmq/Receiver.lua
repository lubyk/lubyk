--[[------------------------------------------------------

  zmq.Receive
  -----------

  Lets you receive lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.ReceiveSocket'
require 'worker'

local constr = zmq.ReceiveSocket
local worker = worker
function zmq.Receiver(location, func)
  local receiver = constr(worker, zmq.PULL, func)
  receiver:connect(location)
  return receiver
end
