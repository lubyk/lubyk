--[[------------------------------------------------------

  zmq.Send
  --------

  Lets you push work encoded in lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.Socket'

function zmq.Push(location)
  local instance = zmq.Socket(zmq.PUSH)
  if location then
    instance:bind(location)
  else
    -- choose a random port with "tcp://*"
    instance:bind()
  end
  return instance
end
