--[[------------------------------------------------------

  zmq.Publisher
  -------------

  Lets you send lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.Socket'

function zmq.Pub(location)
  local instance = zmq.Socket(zmq.PUB)
  if location then
    instance:bind(location)
  else
    -- choose a random port with "tcp://*"
    instance:bind_to_random_port()
  end
  return instance
end
