--[[------------------------------------------------------

  zmq.Send
  --------

  Lets you send lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.Socket'

function zmq.Push(location)
  local instance = zmq.Socket(zmq.PUSH)
  instance:bind(location)
  return instance
end
