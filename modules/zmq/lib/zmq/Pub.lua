--[[------------------------------------------------------

  zmq.Pub
  -------

  Lets you publish lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq.Socket'

local new = zmq.Socket.new
function zmq.Pub(location)
  local self = new(zmq.PUB)
  if location then
    self:bind(location)
  else
    -- choose a random port with "tcp://*"
    self:bind()
  end
  return self
end
