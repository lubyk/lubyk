--[[------------------------------------------------------

  zmq.SimplePull
  --------------

  Pull server with a loop already in place.

--]]------------------------------------------------------
require 'zmq.Rep'

function zmq.SimplePull(location, func)
  if not func then
    func = location
    location = nil
  end
  return zmq.Pull(location, function(server)
    while server:should_run() do
      server:send(func(server:recv()))
    end
  end)
end
