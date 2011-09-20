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
  local self = zmq.Socket(zmq.PULL, func)
  if location then
    if type(location) == 'number' then
      location = string.format('tcp://localhost:%i', location)
    end
    self:connect(location)
  else
    -- choose a random port with "tcp://*"
    self:bind()
  end
  return self
end
