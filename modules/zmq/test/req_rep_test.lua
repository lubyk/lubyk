--[[------------------------------------------------------

  zmq for Lua
  -----------

  zmq using msgpack to send/receive packets. The API is
  slightly different from ØMQ because of the callback
  nature of Rubyk bindings.

--]]------------------------------------------------------
require 'rubyk'


local should = test.Suite('zmq.Req/Rep')

function should.request_reply()
  local server = zmq.Rep(function(msg)
    return msg + 4
  end)

  local total = 0
  -- requester
  local req = zmq.Req()
  req:connect(string.format("tcp://localhost:%i", server:port()))
  assert_equal(9, req:request(5))
  server:kill()
end

test.all()