--[[------------------------------------------------------

  zmq for Lua
  -----------

  zmq using msgpack to send/receive packets. The API is
  slightly different from ØMQ because of the callback
  nature of Rubyk bindings.

--]]------------------------------------------------------
require 'rubyk'


local should = test.Suite('zmq.Socket')

function should.bind_to_random_port()
  local socket = zmq.Socket(zmq.PUSH)
  local port = socket:bind_to_random_port()

  assert_true(port <= 20000)
  assert_true(port >= 2000)
  assert_equal(port, socket:port())

  local socket2 = zmq.Socket(zmq.PUSH)
  local port2 = socket2:bind_to_random_port()
  assert_true(port2 <= 20000)
  assert_true(port2 >= 2000)
  assert_equal(port2, socket2:port())
end

test.all()