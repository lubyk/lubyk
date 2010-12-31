--[[------------------------------------------------------

  zmq.Rep
  -------

  Waits for messages and sends a reply back.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.Socket'

function zmq.Rep(location, func)
  local instance = zmq.Socket(zmq.REP)
  instance:loop(function()
    -- receive, send, receive, ...
    instance:send(func(instance:recv()))
  end)
  return instance
end
