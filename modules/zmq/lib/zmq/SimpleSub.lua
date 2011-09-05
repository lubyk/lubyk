--[[------------------------------------------------------

  zmq.SimpleSub
  --------------

  Sub client with a loop already in place. Beware to connect
  before starting the loop (sleep or other calls).

--]]------------------------------------------------------
require 'zmq.Sub'

function zmq.SimpleSub(filter, func)
  if not func then
    func = filter
    filter = nil
  end
  return zmq.Sub(filter, function(server)
    while server:shouldRun() do
      func(server:recv())
    end
  end)
end
