--[[------------------------------------------------------

  zmq.SimplePull
  --------------

  Pull server with a loop already in place.

--]]------------------------------------------------------
require 'zmq.Pull'

local new = zmq.Pull
function zmq.SimplePull(location, func)
  if not func then
    func = location
    location = nil
  end
  return new(location, function(self)
    local clients = {}
    while self.thread do
      -- In case of unprotected errors, the thread is automatically
      -- restarted.
      func(self:recv())
    end
  end)
end
