--[[------------------------------------------------------

  zmq.Socket
  ----------

  Lets you send and receive lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.Socket_core'
require 'worker'

local constr = zmq.Socket
local worker = worker
function zmq.Socket(type)
  return constr(worker, type)
end
