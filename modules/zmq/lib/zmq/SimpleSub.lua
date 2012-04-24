--[[------------------------------------------------------

  zmq.SimpleSub
  --------------

  Sub client with a loop already in place. Beware to connect
  before starting the loop (sleep or other calls).

--]]------------------------------------------------------
require 'zmq.Sub'

local new = zmq.Sub
function zmq.SimpleSub(filter, func)
  if not func then
    func = filter
    filter = nil
  end
  return new(filter, function(self)
    while self.thread do
      func(self:recv())
    end
  end)
end
