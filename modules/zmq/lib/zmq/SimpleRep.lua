--[[------------------------------------------------------

  zmq.SimpleRep
  -------------

  Reply server with a loop already in place.

--]]------------------------------------------------------
require 'zmq.Rep'

local new = zmq.Rep
function zmq.SimpleRep(location, func)
  if not func then
    func = location
    location = nil
  end
  return new(location, function(self)
    while self.thread do
      self:send(func(self:recv()))
    end
  end)
end
