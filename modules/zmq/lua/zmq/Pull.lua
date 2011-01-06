--[[------------------------------------------------------

  zmq.Pull
  --------

  Lets you pull work encoded in lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.Socket'

function zmq.Pull(location, func)
  if not func then
    func = location
    location = nil
  end
  local instance = zmq.Socket(zmq.PULL, func)
  if location then
    instance:bind(location)
  else
    -- choose a random port with "tcp://*"
    instance:bind()
  end
  return instance
end
