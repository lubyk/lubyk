--[[------------------------------------------------------

  zmq.Subscriber
  --------------

  Lets you subscribe to publishers (zmq.Pub).

  WARNING: You cannot connect to a new publisher while
  receiving. To do this, you should use another Publisher
  using 'inproc' and subscribe to this publisher before
  entering the recv() loop (see lk.Service)

--]]------------------------------------------------------
require 'zmq.Socket'

local new = zmq.Socket.new
function zmq.Sub(filter, func)
  if not func then
    func   = filter
    filter = nil
  end
  local self = new(zmq.SUB, func)
  -- will not start until we sleep or unlock in another way
  -- so it is safe to setsockopt now
  self:setsockopt(zmq.SUBSCRIBE, filter)
  return self
end
