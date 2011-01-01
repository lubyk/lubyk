--[[------------------------------------------------------

  zmq for Lua
  -----------

  zmq using msgpack to send/receive packets. The API is
  slightly different from ØMQ because of the callback
  nature of Rubyk bindings.

--]]------------------------------------------------------
require 'rubyk'


local should = test.Suite('zmq')

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

function should.send_and_receive()
  local sender   = zmq.Push()
  local continue = false
  local received = nil
  local receiver = zmq.Pull(string.format("tcp://localhost:%i", sender:port()), function(...)
    continue = true
    received = arg
  end)

  local function send_and_receive(...)
    continue = false
    -- cannot use ... or arg is set to nil
    sender:send(unpack(arg))
    while not continue do
      worker:sleep(10)
    end

    for i, v in ipairs(arg) do
      assert_equal(arg[i], received[i])
    end
  end

  -- string
  send_and_receive("Hello Rubyk!")
  -- number, nil, bool
  send_and_receive(1.234567)
  send_and_receive(nil)
  send_and_receive(true)
  send_and_receive(false)
  -- array
  -- we cannot send an array like this
  -- send_and_receive({1, 2, 3}) ===> received as (1, 2, 3) (not {1, 2, 3})
  send_and_receive(1, {1, 2, 3})

  send_and_receive(1, {1, 2, {4, 5}})

  send_and_receive("1", {1, "zombie", {4.44, 5}})

  -- hash
  send_and_receive("/one/two", {one=2, three="four"})

  -- multi values
  send_and_receive(1,2,3)
  send_and_receive("/amp/gain", 3.5)
  send_and_receive("/amp/gain", {1, 2, {"foo", "bar", 5}})

  receiver:kill()
end

function should.push_pull_many_messages()
  local sender   = zmq.Push()
  local received = 0
  local receiver = zmq.Pull(string.format("tcp://localhost:%i", sender:port()), function(message)
    received = received + 1
  end)

  while received < 10 do
    sender:send("anything")
    worker:sleep(1)
  end

  assert_equal(10, received)
  receiver:kill()
end

function should.publish_and_subscribe()
  local sender   = zmq.Pub()
  local received = 0
  local receiver = zmq.Sub(function(message)
    received = received + 1
  end)

  receiver:connect(string.format("tcp://localhost:%i", sender:port()))

  while received < 10 do
    sender:send("anything")
    worker:sleep(1)
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
  worker:sleep(100)

  sender:send("anything")

  while received < 2 do
    worker:sleep(10)
  end

  assert_equal(2, received)
  receiver1:kill()
  receiver2:kill()
end

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