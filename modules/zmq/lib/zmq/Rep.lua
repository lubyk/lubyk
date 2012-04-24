--[[------------------------------------------------------

  zmq.Rep
  -------

  Waits for messages and sends a reply back.

--]]------------------------------------------------------
require 'zmq.Socket'

local new = zmq.Socket.new
function zmq.Rep(location, func)
  if not func then
    func = location
    location = nil
  end
  local self = new(zmq.REP, func)
  if location then
    self:bind(location)
  else
    -- choose a random port with "tcp://*"
    self:bind()
  end
  return self
end
