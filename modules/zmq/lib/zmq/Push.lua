--[[------------------------------------------------------

  zmq.Push
  --------

  Lets you push work encoded in lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq.Socket'

local new = zmq.Socket.new
function zmq.Push(location)
  local self = new(zmq.PUSH)
  if location then
    if type(location) == 'number' then
      location = string.format('tcp://*:%i', location)
    end
    self:bind(location)
  else
    -- choose a random port with "tcp://*"
    self:bind()
  end
  return self
end
