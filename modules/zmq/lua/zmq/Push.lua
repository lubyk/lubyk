--[[------------------------------------------------------

  zmq.Send
  --------

  Lets you send lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.Socket'

function zmq.Push(location)
  local instance = zmq.Socket(zmq.PUSH)
  if location then
    instance:bind(location)
  else
    -- choose a random port with "tcp://*"
    instance:bind_to_random_port()
  end
  return instance
end
