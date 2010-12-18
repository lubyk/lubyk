--[[------------------------------------------------------

  zmq.Send
  --------

  Lets you send lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq.Send_core'
require 'worker'

local constr = zmq.Send
local worker = worker
function zmq.Send(location)
  return constr(worker, location)
end
