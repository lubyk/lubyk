--[[------------------------------------------------------

  zmq.Rep
  -------

  Waits for messages and sends a reply back.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.Socket'

function zmq.Rep(location, func)
  if not func then
    func = location
    location = nil
  end
  local instance = zmq.Socket(zmq.REP)
  if location then
    instance:bind(location)
  else
    -- choose a random port with "tcp://*"
    instance:bind_to_random_port()
  end
  instance:loop(function()
    -- receive, send, receive, ...
    instance:send(func(instance:recv()))
  end)
  return instance
end
