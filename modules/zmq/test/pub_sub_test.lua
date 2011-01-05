--[[------------------------------------------------------

  zmq for Lua
  -----------

  zmq using msgpack to send/receive packets. The API is
  slightly different from ØMQ because of the callback
  nature of Rubyk bindings.

--]]------------------------------------------------------
require 'rubyk'


local should = test.Suite('zmq.Pub/Sub')

function should.publish_and_subscribe()
  local sender   = zmq.Pub()
  local received = 0
  local receiver = zmq.Sub(function(message)
    received = received + 1
  end)

  receiver:connect(string.format("tcp://localhost:%i", sender:port()))

  while received < 10 do
    sender:send("anything")
    sleep(1)
  end

  assert_equal(10, received)
  receiver:kill()
end

function should.publish_and_subscribe_many()
  local sender   = zmq.Pub()
  local received = 0
  local function receive_callback(message)
    received = received + 1
  end
  local receiver1 = zmq.Sub(receive_callback)
  local receiver2 = zmq.Sub(receive_callback)

  receiver1:connect(string.format("tcp://localhost:%i", sender:port()))
  receiver2:connect(string.format("tcp://localhost:%i", sender:port()))

  -- make sure receivers are ready before starting to send
  sleep(100)

  sender:send("anything")

  while received < 2 do
    sleep(10)
  end

  assert_equal(2, received)
  receiver1:kill()
  receiver2:kill()
end

test.all()