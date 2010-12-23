--[[------------------------------------------------------

  zmq.Send
  --------

  Lets you send lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.SendSocket'
require 'worker'

local constr = zmq.SendSocket
local worker = worker
function zmq.Sender(location)
  local sender = constr(worker, zmq.PUSH)
  sender:bind(location)
  return sender
end
