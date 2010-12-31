--[[------------------------------------------------------

  zmq.Publish
  -----------

  Lets you send lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.SendSocket'

local constr = zmq.SendSocket
function zmq.Publisher(location)
  local sender = constr(zmq.PUB)
  sender:bind(location)
  return sender
end
