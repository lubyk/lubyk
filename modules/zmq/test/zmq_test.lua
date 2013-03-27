--[[------------------------------------------------------

  zmq for Lua
  -----------

  zmq using msgpack to send/receive packets. The API is
  slightly different from ØMQ because of the callback
  nature of Lubyk bindings.

--]]------------------------------------------------------
require 'lubyk'


local should = test.Suite 'zmq.Socket'

function should.haveSameFlagsAsLkPoller()
  -- Compatiblitity between lk.Poller and zmq.Poller. If zmq constants change,
  -- we must update lk's values.
  assertEqual(lk.Poller.READ_FLAG,  zmq.POLLIN)
  assertEqual(lk.Poller.WRITE_FLAG, zmq.POLLOUT)
end

function should.bind_to_random_port()
  local socket = zmq.Socket(zmq.PUSH)
  local port = socket:bind()

  assertTrue(port <= 20000)
  assertTrue(port >= 2000)
  assertEqual(port, socket:port())

  local socket2 = zmq.Socket(zmq.PUSH)
  local port2 = socket2:bind()
  assertTrue(port2 <= 20000)
  assertTrue(port2 >= 2000)
  assertEqual(port2, socket2:port())
end

test.all()
