--[[------------------------------------------------------

  zmq.SimpleRep
  -------------

  Reply server with a loop already in place.

--]]------------------------------------------------------
require 'zmq.Rep'

function zmq.SimpleRep(location, func)
  if not func then
    func = location
    location = nil
  end
  return zmq.Rep(location, function(self)
    while self.thread do
      self:send(func(self:recv()))
    end
  end)
end
