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
    local clients = {}
    while self.thread do
      -- Run in a new thread so that errors do not
      -- halt the socket...
      -- TODO: should (and can) we reuse the coroutine ?
      local data = {self:recv()}
      clients[data] = lk.Thread(function()
        func(unpack(data))
        -- Garbage collect
        clients[data] = nil
      end)
    end
  end)
end
