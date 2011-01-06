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
  return zmq.Rep(location, function(server)
    while server:should_run() do
      server:send(func(server:recv()))
    end
  end)
end
