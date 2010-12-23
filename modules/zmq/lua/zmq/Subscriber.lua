--[[------------------------------------------------------

  zmq.Subscribe
  -------------

  Lets you receive lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.ReceiveSocket'
require 'worker'

local constr = zmq.ReceiveSocket
local worker = worker
function zmq.Subscriber(func)
  local receiver = constr(worker, zmq.SUB, func)
  receiver:setsockopt(zmq.SUBSCRIBE) -- filter none
  return receiver
end
