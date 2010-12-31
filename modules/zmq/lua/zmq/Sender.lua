--[[------------------------------------------------------

  zmq.Send
  --------

  Lets you send lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.SendSocket'

local constr = zmq.SendSocket
function zmq.Sender(location)
  local sender = constr(zmq.PUSH)
  sender:bind(location)
  return sender
end
