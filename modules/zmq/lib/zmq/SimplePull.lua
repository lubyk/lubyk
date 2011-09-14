--[[------------------------------------------------------

  zmq.SimplePull
  --------------

  Pull server with a loop already in place.

--]]------------------------------------------------------
require 'zmq.Pull'

function zmq.SimplePull(location, func)
  if not func then
    func = location
    location = nil
  end
  return zmq.Pull(location, function(self)
    while self.thread do
      func(self.super:recv())
    end
  end)
end
