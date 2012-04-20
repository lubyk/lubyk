--[[------------------------------------------------------

  zmq.Pub
  -------

  Lets you publish lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.Socket'

function zmq.Pub(location)
  local self = zmq.Socket(zmq.PUB)
  if location then
    self:bind(location)
  else
    -- choose a random port with "tcp://*"
    self:bind()
  end
  return self
end
