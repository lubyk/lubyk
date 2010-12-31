--[[------------------------------------------------------

  zmq.Publisher
  -------------

  Lets you send lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.Socket'

function zmq.Pub(location)
  local instance = zmq.Socket(zmq.PUB)
  instance:bind(location)
  return instance
end
