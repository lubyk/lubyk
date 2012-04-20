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

function zmq.Req(location)
  local self = zmq.Socket(zmq.REQ)
  if location then
    self:connect(location)
  end
  return self
end
