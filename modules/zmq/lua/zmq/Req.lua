--[[------------------------------------------------------

  zmq.Rep
  -------

  Waits for messages and sends a reply back.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.Socket'

function zmq.Req()
  local instance = zmq.Socket(zmq.REQ)
  function instance:request(...)
    instance:send(...)
    return instance:recv()
  end
  return instance
end
