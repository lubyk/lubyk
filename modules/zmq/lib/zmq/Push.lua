--[[------------------------------------------------------

  zmq.Push
  --------

  Lets you push work encoded in lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.Socket'

function zmq.Push(location)
  local self = zmq.Socket(zmq.PUSH)
  if location then
    self:bind(location)
  else
    -- choose a random port with "tcp://*"
    self:bind()
  end
  return self
end
