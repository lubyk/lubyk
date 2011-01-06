--[[------------------------------------------------------

  zmq.Req
  -------

  Remote call (sends a query and get reply). This should
  be done with the 'request' method.

  The 'request' method has been made to make it possible to
  share the socket between multiple threads (together with
  the 'connect' method). Other methods such as 'send' or
  'recv' do not support sharing across threads.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.Socket'

function zmq.Req()
  return zmq.Socket(zmq.REQ)
end

function zmq.Req(location)
  local instance = zmq.Socket(zmq.PUSH)
  if location then
    instance:bind(location)
  else
    -- choose a random port with "tcp://*"
    instance:bind()
  end
  return instance
end
